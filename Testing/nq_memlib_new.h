#ifndef NQ_MEMLIB_NEW_H_
# define NQ_MEMLIB_NEW_H_

# include <initializer_list>

# include "nq_allocator.h"
# include "domains.h"
# include "alloc_strat.h"

# include "nq_memlib_tools.h"

namespace nq
{
	template <class T,
		 class Domain = UnknownDomain,
		 class AllocStrat = DefaultAlloc,
		 class... Args>
	T* New(Args... Parameters)
	{
		T *ptr = nq::allocator<T, Domain, AllocStrat>().allocate(1);
		nq::allocator<T, Domain, AllocStrat>().
            construct(ptr, std::forward<Args>(Parameters)...);
		return ptr;
	}

	template <class T,
		 class Domain = UnknownDomain,
		 class AllocStrat = DefaultAlloc>
    T* New_array(std::size_t count, std::initializer_list<T> ilist = {})
    {
        assert (ilist.size() <= count);
		T *ptr = nq::allocator<T, Domain, AllocStrat>().allocate(count);

        // construct the elements of the allocated array
        construct_from_range(ptr, count, ilist.begin(), ilist.end());
        return ptr;
    }

	template <class T,
		class Domain = UnknownDomain,
		class AllocStrat = DefaultAlloc>
	void Delete(T *ptr)
	{
		nq::allocator<T, Domain, AllocStrat>().destroy(ptr);
		nq::allocator<T, Domain, AllocStrat>().deallocate(ptr, 0);
	}
}

#endif // !NQ_MEMLIB_NEW_H_
