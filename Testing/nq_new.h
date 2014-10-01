#ifndef NQ_NEW_H_
# define NQ_NEW_H_

# include "nq_memlib_allocate.h"
# include "nq_memlib_tools.h"

# include "nq_allocator.h"
# include "domains.h"

class NewedType
{
    char size;
};

template<class Domain>
void *operator new(size_t count,
        const Domain&, const char *file, size_t line) noexcept
{
    // TODO To replace
    if (count == 0)
        return nullptr;
    void *internal_ptr =
        nq::memlib::allocate<NewedType>(count, Domain::sub_header_size);
    if (internal_ptr == nullptr)
        throw std::bad_alloc();

    Domain::getInstance().add(internal_ptr, count,
            file, line, &(Domain::getInstance()));
 
    void *usr_ptr = 
        nq::memlib::get_usr_ptr(internal_ptr, Domain::sub_header_size);
    return usr_ptr;
}

template<class Domain>
void *operator new[](size_t count,
        const Domain& domain, const char *file, size_t line) noexcept
{
    return (operator new(count, domain, file, line));
}

namespace nq { namespace memlib {
    void remove_header_operator_delete(void *ptr);
}} // namespace nq::memlib

#endif // !NQ_NEW_H_
