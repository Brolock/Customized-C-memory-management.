#ifndef NQ_MEMLIB_ALLOCATE_H_
# define NQ_MEMLIB_ALLOCATE_H_

# include <memory>

# include "alloc_strat.h"

namespace nq { namespace memlib
{
    template<class T,
        class AllocStrat = DefaultAlloc>
    T* allocate(size_t nb_elmt, size_t headers = 0)
    { // allocate nb_elmt * sizeof (T) memory and return a new pointer
        void *inter_ptr = AllocStrat().allocate(nb_elmt * sizeof (T) + headers);
        return static_cast<T*>(inter_ptr);
    }

    template<class AllocStrat = DefaultAlloc>
    void deallocate(void *ptr)
    { // deallocate with AllocStrat memory at ptr
        AllocStrat().deallocate(ptr);
    }
}} // namespace nq::memlib

#endif // NQ_MEMLIB_ALLOCATE_H_
