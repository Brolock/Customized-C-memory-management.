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
	void* New(Args... Parameters)
	{
		void *ptr = nq::allocator<T, Domain, DefaultAlloc>{}.allocate(1);
		nq::allocator<T, Domain, AllocStrat>{}.construct(ptr, std::forward<Args>(Parameters)...);
		return ptr;
	}

	template <typename T,
		 typename Domain,
		 class AllocStrat,
		 typename... Args>
	void* New(Args... Parameters)
	{
		void *ptr = nq::allocator<T, Domain, AllocStrat>{}.allocate(1);
		nq::allocator<T, Domain, AllocStrat>{}.construct(ptr, std::forward<Args>(Parameters)...);
		return ptr;
	}
}

#endif /* !NQ_NEW_H_ */
