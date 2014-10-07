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
		pointer allocate(size_type n,
                std::allocator<void>::const_pointer = 0)
		{ // allocate memory with alloc_strat
            // TODO To replace
			if (n == 0)
				return nullptr;

            pointer internal_ptr = memlib::allocate<T, AllocStrat>
                    (n, Domain::header_size);

			if (internal_ptr == nullptr)
				throw std::bad_alloc();

			Domain::getInstance().add(internal_ptr, n * sizeof(T));

            pointer usr_ptr =
                memlib::get_usr_ptr(internal_ptr, Domain::header_size);
			return usr_ptr;
            // !TODO
		}

		void deallocate(pointer usr_ptr, size_type)
		{ // deallocate memory pointer by usr_ptr with alloc_strat
            // TODO To replace
            /*
			if (usr_ptr != nullptr)
			{
				void *internal_ptr =
                     memlib::get_internal_ptr(usr_ptr, Domain::header_size);

				Domain::getInstance().remove(internal_ptr);

                memlib::deallocate<AllocStrat>(internal_ptr);
			}*/
            memlib::deallocate_log(usr_ptr, Domain::header_size,
                    nq::remove_elem_domain<Domain>);
                /*
                    std::bind(&Domain::remove,
                        &Domain::getInstance(), std::placeholders::_1));
                        */
			/* size_type ? */
		}

		size_type max_size() const
		{
			return std::numeric_limits<size_type>::max();
		}

		void construct(pointer p, const_reference val)
		{
            memlib::construct(p, val);
		}

		template <class U,
			class... Args>
		void construct(U* p, Args&&... args)
		{
            memlib::construct(p, std::forward<Args>(args)...);
		}

		/* destroy(pointer p) and destroy(U* p) are the same.*/
		template <class U>
		void destroy(U* p)
		{
            memlib::destroy(p);
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
