#include "../include/nq_memlib/base_domain.h"

#include <thread>

#ifdef WITH_NQ_MEMLOG
void BaseDomain::Header::add(Header* next)
{
    next_ = next;
    next->prev_ = this;
}
void BaseDomain::Header::remove()
{
    if (next_ != nullptr)
        next_->prev_ = prev_;
    if (prev_ != nullptr)
        prev_->next_ = next_;
}
BaseDomain::Header* BaseDomain::Header::remove_begin()
{
    if (next_ != nullptr)
        next_->prev_ = prev_;
    return next_;
}
BaseDomain::Header* BaseDomain::Header::remove_end()
{
    if (prev_ != nullptr)
        prev_->next_ = next_;
    return prev_;
}

void BaseDomain::Header::print(std::ostream& os = std::cout) const
{
    os << "size: " << size_ << std::endl;

    /* If padding_ is 1 then we are in the case of a sub_header
     * If the file logged if null then it is an internal implementation use
     * of new so it shouldn't be logged
     */
    if (padding_ == 1 && (static_cast<const SubHeader*>(this))->get_file())
    {
        os << "Is a new, @ File: "
            << (static_cast<const SubHeader*>(this))->get_file()
            << ", Line: " << (static_cast<const SubHeader*>(this))->get_line()
            << std::endl;
    }
    if (next_ != nullptr)
        next_->print(os);
}
void BaseDomain::add(void* internal_ptr, std::size_t size)
{
    /* basic mutex locking */
    std::lock_guard<std::mutex> locker(mutex_);

    /* Construct a Header to the internal_ptr */
    Header* head = new (internal_ptr)Header(size);
    /* When adding the first element we initialize begin_ and end_ */
    if (begin_ == nullptr)
    {
        end_ = head;
        begin_ = end_;
    }
    /* otherwise we push back our element and put end_ up-to-date */
    else
    {
        end_->add(head);
        end_ = head;
    }
    count_++;
    size_ += size;
}
void BaseDomain::add(void* internal_ptr, std::size_t size,
        const char* file, size_t line, BaseDomain *dom)
{
    /* basic mutex locking */
    std::lock_guard<std::mutex> locker(mutex_);

    /* Construct a Header to the internal_ptr */

    Header *head = new (internal_ptr)SubHeader(size, file, line, dom);
    /* When adding the first element we initialize begin_ and end_ */
    if (begin_ == nullptr)
    {
        end_ = head;
        begin_ = end_;
    }
    /* otherwise we push back our element and put end_ up-to-date */
    else
    {
        end_->add(head);
        end_ = head;
    }
    // increment domain specific infos
    count_++;
    size_ += size;
}

void BaseDomain::remove(void *internal_ptr)
{
    /* basic mutex locking */
    std::lock_guard<std::mutex> locker(mutex_);
    Header* ptr = static_cast<Header*>(internal_ptr);

    // decrement domain specific infos
    count_--;
    size_ -= ptr->size();

    /* Check if tmp is the begin or the end of the Domain list and
     * call the appropriate remove in consequences */
    if (ptr == begin_)
        begin_ = ptr->remove_begin();
    if (ptr == end_)
        end_ = ptr->remove_end();
    if (ptr != begin_ && ptr != end_ && ptr != nullptr)
        ptr->remove();
    /* Destructor called */
    ptr->~Header();
}

void BaseDomain::print(std::ostream& os, size_t tree_height) const
{
    /* basic mutex locking */
    std::lock_guard<std::mutex> locker(mutex_);

        os << "--------------------" << std::endl;
        os << domain_name() << std::endl;
        std::tuple<int, int> tree_tuple = Super::get_branch_infos();

        std::string tabs = "";
        std::generate_n(std::back_inserter(tabs), tree_height,
                [](){return '\t';});

        os << tabs << "nb_alloc with sons: " << std::get<0>(tree_tuple)
                << "  (nb_alloc : " << count_ << ")\n"
                << tabs << "size_alloc with sons: " << std::get<1>(tree_tuple)
                << "  (size_alloc : " << size_ << ")\n";



    if (begin_ != nullptr)
        begin_->print(os);
    os << "--------------------" << std::endl;

    if (Super::sons_ != nullptr)
        Super::sons_->print(os, tree_height + 1);
    if (Super::brothers_ != nullptr)
        Super::brothers_->print(os, tree_height);
}
#endif // WITH_NQ_MEMLOG
