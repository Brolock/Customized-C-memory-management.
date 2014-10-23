#ifndef NQ_ALLOCATOR_H_
# define NQ_ALLOCATOR_H_

# include <cassert>
# include <limits>
# include <memory>

# include "nq_memlib_tools.h"
# include "nq_memlib_allocate.h"

# include "domains.h"
# include "alloc_strat.h"

# undef max

// Allow to totally desactivate the logging lib
# ifndef WITH_NQ_MEMOFF
namespace nq
{
    template<class T,
        class Domain = UnknownDomain,
        class AllocStrat = DefaultAlloc>
    struct allocator
    {
        /* Member types */
        typedef T value_type;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T& reference;
        typedef const T& const_reference;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;

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
            return memlib::allocate_log<T, Domain, AllocStrat>(count,
                                                    Domain::header_size);
        }

        void deallocate(pointer usr_ptr, size_type)
        { // deallocate memory pointer by usr_ptr with alloc_strat
            memlib::deallocate_log<AllocStrat>(usr_ptr, Domain::header_size,
                    memlib::remove_elem_domain<Domain>);
            /* size_type ? */
        }

        size_type max_size() const
        {
            return std::numeric_limits<size_type>::max();
        }

        void construct(pointer ptr, const_reference val)
        {
            memlib::construct(ptr, val);
        }

        template <class U,
            class... Args>
        void construct(U* ptr, Args&&... args)
        {
            memlib::construct(ptr, std::forward<Args>(args)...);
        }

        /* destroy(pointer p) and destroy(U* p) are the same.*/
        template <class U>
        void destroy(U* ptr)
        {
            memlib::destroy(ptr);
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

// nq::allocator is a simple std::allocator
# else // WITH_NQ_MEMOFF defined
namespace nq
{
    template<class T,
        class Domain = UnknownDomain,
        class AllocStrat = DefaultAlloc>
    using allocator = std::allocator<T>;
}
# endif // !WITH_NQ_MEMOFF

#endif // !NQ_ALLOCATOR_H_
