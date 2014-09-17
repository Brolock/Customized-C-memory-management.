#ifndef NQ_NEW_H_
# define NQ_NEW_H_

# include "nq_allocator.h"
# include "domains.h"
# include "alloc_strat.h"

namespace nq
{
	template <typename T,
		 typename Domain = UnknownDomain,
		 class AllocStrat = DefaultAlloc,
		 typename... Args>
	T* New(Args... Parameters)
	{
		T *ptr = nq::allocator<T, Domain, AllocStrat>{}.allocate(1);
		nq::allocator<T, Domain, AllocStrat>{}.construct(ptr, std::forward<Args>(Parameters)...);
		return ptr;
	}

	template <typename T,
		 typename Domain = UnknownDomain,
		 class AllocStrat = DefaultAlloc,
		 typename... Args>
	T* New(Args... Parameters)
    {}


	template <typename T,
		typename Domain = UnknownDomain,
		class AllocStrat = DefaultAlloc>
	void Delete(T *ptr)
	{
		nq::allocator<T, Domain, AllocStrat>{}.destroy(ptr);
		nq::allocator<T, Domain, AllocStrat>{}.deallocate(ptr, 0);
	}
}

#endif /* !NQ_NEW_H_ */
