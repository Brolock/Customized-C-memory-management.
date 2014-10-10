#ifndef NQ_ALLOCATOR_H_
# define NQ_ALLOCATOR_H_

# include <cassert>
# include <cstdlib>
# include <cstddef>
# include <limits>
# include <iostream>
# include <memory>

# include "nq_memlib_tools.h"
# include "nq_memlib_allocate.h"

# include "domains.h"
# include "alloc_strat.h"

# undef max

namespace nq
{
	template<class T,
		class Domain = UnknownDomain,
		class AllocStrat = DefaultAlloc>
	struct allocator : public std::allocator<T>
	{
		/* Member types */
		typedef T value_type;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;

		typedef AllocStrat allocator_strategy;

		/* Allocator convertor from T to U */
		template<class U>
		struct rebind
		{
			typedef allocator<U, Domain, AllocStrat> other;
		};

		/* constructors */
		explicit allocator() {}
		allocator(const allocator&) {}

		template <class U>
			allocator(const allocator<U, Domain, AllocStrat>&) {}

		template<class U>
		allocator& operator=(const allocator<U, Domain, AllocStrat>&)
        { return *this; }

		allocator& operator=(const allocator& other)
        { return *this; }

		~allocator() {};

		/* Adress */
		pointer adress(reference r) const
		{ return &r; }
		const_pointer adress(const_reference r) const
		{ return &r; }

		/* Memory allocation */
		pointer allocate(size_type count,
                std::allocator<void>::const_pointer = 0)
		{ // allocate memory with alloc_strat
# ifdef WITH_NQ_MEMLOG
            return memlib::allocate_log<T, Domain, AllocStrat>(count,
                                                    Domain::header_size);
# else
            return static_cast<pointer>(AllocStrat().allocate(sizeof (T)));
# endif
		}

		void deallocate(pointer usr_ptr, size_type)
		{ // deallocate memory pointer by usr_ptr with alloc_strat
# ifdef WITH_NQ_MEMLOG
            memlib::deallocate_log<AllocStrat>(usr_ptr, Domain::header_size,
                    memlib::remove_elem_domain<Domain>);
# else // WITH_NQ_MEMLOG
            AllocStrat().deallocate(usr_ptr);
# endif // !WITH_NQ_MEMLOG
			/* size_type ? */
		}

		size_type max_size() const
		{
			return std::numeric_limits<size_type>::max();
		}

		void construct(pointer ptr, const_reference val)
		{
# ifdef WITH_NQ_MEMLOG
            memlib::construct(ptr, val);
# else // WITH_NQ_MEMLOG
            new(ptr) T(val);
# endif // !WITH_NQ_MEMLOG
		}

		template <class U,
			class... Args>
		void construct(U* ptr, Args&&... args)
		{
#ifdef WITH_NQ_MEMLOG
            memlib::construct(ptr, std::forward<Args>(args)...);
#else // WITH_NQ_MEMLOG
            new(ptr) T(std::forward<Args>(args)...);
#endif // !WITH_NQ_MEMLOG
		}

		/* destroy(pointer p) and destroy(U* p) are the same.*/
		template <class U>
		void destroy(U* ptr)
		{
#ifdef WITH_NQ_MEMLOG
            memlib::destroy(ptr);
#else // WITH_NQ_MEMLOG
            ptr->~U();
#endif // !WITH_NQ_MEMLOG
		}
	};


	template<class T1,
		class Domain1,
		class T2,
		class Domain2>
	bool operator==(const allocator<T1, Domain1>& lhs,
            const allocator<T2, Domain2>& rhs)
	{
		return true;
	}

	template<class T1,
		class Domain1,
		class T2,
		class Domain2>
	bool operator!=(const allocator<T1, Domain1>& lhs,
            const allocator<T2, Domain2>& rhs)
	{
		return !operator==(lhs, rhs);
	}
}

#endif // !NQ_ALLOCATOR_H_
