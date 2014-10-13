#ifndef NQ_NEW_H_
# define NQ_NEW_H_

# include "nq_memlib_allocate.h"
# include "nq_memlib_tools.h"

# include "domains.h"

/*
** NQ_NEW is logged with its file line for easier leak detections
** The Domain is also logged to be recovered by operator delete
*/
# define NQ_NEW(Domain) new (Domain::getInstance(), __FILE__, __LINE__)

class NewedType
{
    char size;
};

template<class Domain>
void* operator new(size_t count,
        const Domain&, const char *file, size_t line) noexcept
{
    return nq::memlib::allocate_log<NewedType, Domain>(count,
            Domain::sub_header_size, file, line);
}

template<class Domain>
void* operator new[](size_t count,
        const Domain& domain, const char *file, size_t line) noexcept
{
    return (operator new(count, domain, file, line));
}

namespace nq { namespace memlib {
    void remove_header_operator_delete(void *ptr);
}} // namespace nq::memlib

#endif // !NQ_NEW_H_
