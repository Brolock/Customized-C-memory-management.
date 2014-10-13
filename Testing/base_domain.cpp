#include "base_domain.h"

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

    if (padding_ == 1)
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
}
void BaseDomain::add(void* internal_ptr, std::size_t size,
        const char *file, size_t line, BaseDomain *dom)
{
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
    count_++;
}

void BaseDomain::remove(void *internal_ptr)
{
    Header* ptr = static_cast<Header*>(internal_ptr);
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
    count_--;
}

void BaseDomain::print(std::ostream& os = std::cout) const
{
    if (begin_ != nullptr)
    {
        os << domain_name() << " memory log:\n=> "
            << count_ << " allocated objets\n";
        begin_->print(os);
        os << "--------------------" << std::endl;
    }
    else
    {
        os << "No memory allocated in " << domain_name() << std::endl;
    }
}
#endif // WITH_NQ_MEMLOG
