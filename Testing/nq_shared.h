#ifndef NQ_SHARED_H_
# define NQ_SHARED_H_

# include <memory>

# include "nq_allocator.h"
# include "nq_deleter.h"
# include "domains.h"
# include "alloc_strat.h"

/* weak allocation ? where how ? weak ref count */

namespace nq
{
	template<typename T,
		typename Domain = UnknownDomain,
	    class AllocStrat = DefaultAlloc>
	class shared_ptr : public std::shared_ptr<T>
	{
	public:
		/* Allocator used to allocate the ref_count of the shared_ptr */
		typedef nq::allocator<T, SharedPtrRefCountDomain, AllocStrat> count_alloc;
		/* The deleter */
		typedef nq::deleter<T, Domain, AllocStrat> deleter;

		shared_ptr()
			: std::shared_ptr<T>(nullptr, deleter{}, count_alloc{})
		{ // construct with nullptr deleter{} and alloc{} 
		}

		 shared_ptr(std::nullptr_t)
			 :std::shared_ptr<T>(nullptr, deleter{}, count_alloc{})
		 { // construct with nullptr, deleter{}, count_alloc{}
		 }

		template<class Deleter>
		shared_ptr(std::nullptr_t, Deleter del)
		    : std::shared_ptr<T>(nullptr, del, count_alloc{})
		{ // construct with nullptr, del and count_alloc{}
		}

		template<class Deleter,
			 class Allocator>
		shared_ptr(std::nullptr_t, Deleter del, Allocator alloc)
		    : std::shared_ptr<T>(nullptr, del, alloc)
		{ // construct with nullptr, del and alloc
		}

		template<typename Y>
		explicit shared_ptr(Y *ptr)
			: std::shared_ptr<T>(ptr, deleter{}, count_alloc{})
		{ // construct with ptr deleter{} and count_alloc{}
		}

		template<typename Y,
			class Deleter>
		shared_ptr(Y *ptr, Deleter del)
		    : std::shared_ptr<T>(ptr, del, count_alloc{})
		{ // construct with ptr, del and count_alloc{}
		}

		template<typename Y,
		     class Deleter,
			 class Allocator>
		shared_ptr(Y *ptr, Deleter del, Allocator alloc)
		    : std::shared_ptr<T>(ptr, del, alloc)
		{ // construct with ptr, del and alloc
		}
	};
}

#endif // !NQ_SHARED_H_
