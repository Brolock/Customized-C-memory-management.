#ifndef NQ_NEW_H_
# define NQ_NEW_H_

# include "nq_allocator.h"
# include "domains.h"
# include "alloc_strat.h"

namespace nq
{
	template <typename T,
		 typename Domain,
		 typename... Args>
	T* New(Args... Parameters)
	{
		T *ptr = nq::allocator<T, Domain, DefaultAlloc>{}.allocate(1);
		nq::allocator<T, Domain, DefaultAlloc>{}.construct(ptr, std::forward<Args>(Parameters)...);
		return ptr;
	}

	template <typename T,
		 typename Domain,
		 class AllocStrat,
		 typename... Args>
	T* New(Args... Parameters)
	{
		T *ptr = nq::allocator<T, Domain, AllocStrat>{}.allocate(1);
		nq::allocator<T, Domain, AllocStrat>{}.construct(ptr, std::forward<Args>(Parameters)...);
		return ptr;
	}

	template <typename T,
		typename Domain>
	void Delete(void *ptr)
	{
		nq::allocator<T, Domain, DefaultAlloc>{}.destroy(ptr);
		nq::allocator<T, Domain, DefaultAlloc>{}.deallocate(ptr, 0);
	}

	template <typename T,
		typename Domain,
		class AllocStrat>
	void Delete(void *ptr)
	{
		nq::allocator<T, Domain, AllocStrat>{}.destroy(ptr);
		nq::allocator<T, Domain, AllocStrat>{}.deallocate(ptr, 0);
	}
}

#endif /* !NQ_NEW_H_ */
