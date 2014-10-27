#ifndef NQ_NEW_H_
# define NQ_NEW_H_

# include "nq_memlib_allocate.h"
# include "nq_memlib_tools.h"

# include "lib_domains.h"

# ifndef WITH_NQ_MEMOFF
/*
** NQ_NEW is logged with its file line for easier leak detections
** The Domain is also logged to be recovered by operator delete
*/
#  define NQ_NEW(Domain) new (Domain::getInstance(), __LINE__, __FILE__)

class NewedType
{
    char size;
};

template<class Domain>
void* operator new(size_t count,
        const Domain&, size_t line, const char *file) noexcept
{
    return nq::memlib::allocate_log<NewedType, Domain>(count,
            Domain::sub_header_size, file, line);
}

template<class Domain>
void* operator new[](size_t count,
        const Domain& domain, size_t line, const char *file) noexcept
{
    return (operator new(count, domain, line, file));
}

namespace nq { namespace memlib {
    void remove_header_operator_delete(void *ptr);
}} // namespace nq::memlib

/** This specific NEW is only reserved for internal implementations **/
# define INTERNAL_NQ_NEW(Domain) new (Domain, 0, nullptr)

// No more logging NQ_NEW
# else // WITH_NQ_MEMOFF defined

#  define NQ_NEW(Domain) new

/** This specific NEW is only reserved for internal implementations **/
#  define INTERNAL_NQ_NEW(Domain) new

# endif // !WITH_NQ_MEMOFF

#endif // !NQ_NEW_H_
