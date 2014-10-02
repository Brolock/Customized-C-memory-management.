#ifndef NQ_DELETER_H_
# define NQ_DELETER_H_

#include <type_traits>

#include "nq_allocator.h"

/* Maybe some work to do with the unique_ptr deleter to use it as allocator ?? */

namespace nq
{
	template <typename T,
		typename Domain = UnknownDomain,
		class AllocStrat = DefaultAlloc>
	struct deleter
	{
		deleter()
		{ // default construct
		}

		/* In the template declaration list, (it seems) the last one is a
		** "template way" to check is types are convertible and so to avoid
		** unwanted compilation (copy constructing an unconvertible type) */
		template <class U,
			typename U_Domain,
			class U_AllocStrat>
		deleter(const deleter<U, U_Domain, U_AllocStrat>&,
			    typename std::enable_if<
                  std::is_convertible<U*, T*>::value>::type* = nullptr)
		{ // construct from another deleter
		}

		void operator()(T *ptr) const
		{ //delete the ptr with allocstrat
			allocator<T, Domain, AllocStrat>().deallocate(ptr, 0);
		}
	};

	
	/* This deleter is to think about since it's only used for unique_ptr[]
	** which hasnt the same behviaviour as our allocator one */
	template <typename T,
		typename Domain,
		class AllocStrat>
	struct deleter <T[], Domain, AllocStrat>
	{
		deleter()
		{ // default construct
		}

		template <typename U>
		void operator()(U *ptr) const = delete;

		void operator()(T *ptr) const
		{
            memlib::Delete_array<T, Domain, AllocStrat>(ptr);
		}
	};
}

#endif // !NQ_DELETER_H_
