#ifndef NQ_UNIQUE_H_
# define NQ_UNIQUE_H_

#include <memory>

# include "nq_deleter.h"
# include "alloc_strat.h"

# include "nq_memlib_new.h"

namespace nq
{
	template<class T,
		class Domain = UnknownDomain,
        class AllocStrat = DefaultAlloc>
	class unique_ptr
    : public std::unique_ptr<T, nq::deleter<T, Domain, AllocStrat>>
	{
        typedef nq::deleter<T, Domain, AllocStrat> deleter_type;
		typedef std::unique_ptr<T, deleter_type> parent;

        typedef typename parent::element_type element_type;
        typedef typename parent::pointer pointer;

        /*** Constructors ***/

    public:
		unique_ptr() noexcept
			: parent()
		{ // default construct
        }

        unique_ptr(std::nullptr_t ptr) noexcept
            : parent(ptr)
        { // construct unique_ptr with nullptr (equivalent to default)
        }

        explicit unique_ptr(pointer ptr) noexcept
            : parent(ptr)
        { // construct unique_ptr with T*
        }
        
        unique_ptr(pointer ptr,
                typename std::conditional<std::is_reference<deleter_type>::value,
                  deleter_type, const deleter_type&>::type del) noexcept
            : parent(ptr, del)
        {}

        unique_ptr(T *ptr,
                typename std::remove_reference<deleter_type>::type&& del) noexcept
            : parent(std::move(ptr), std::move(del))
        {}

        /*** Move constructors ***/
        
        unique_ptr(unique_ptr&& other) noexcept
            : parent(std::move(other))
        { // construct unique_ptr object that takes resource from other
        }

        template<class Y,
            class Y_Domain,
            class Y_Alloc>
        unique_ptr(unique_ptr<T, Y_Domain, Y_Alloc>&& other) noexcept
            : parent(std::move(other))
        { // construct unique_ptr object that takes resource from other
        }

        /*** Assigment operator ***/
        
        unique_ptr& operator=(unique_ptr&& rhs) noexcept
        {
            this->parent::operator=(std::move(rhs));
            return *this;
        }

        template<class Y,
            class Y_Domain,
            class Y_Alloc>
        unique_ptr& operator=(unique_ptr<Y, Y_Domain, Y_Alloc>&& rhs) noexcept
        {
            this->parent::operator=(std::move(rhs));
            return *this;
        }
        
        unique_ptr& operator=(std::nullptr_t) noexcept
        {
            this->parent::operator=(nullptr);
            return *this;
        }

    public:
        /*
        // convert to std from nq
        unique_ptr(std::unique_ptr<T, deleter_type>&& other) noexcept
            : parent(std::move(other))
        {}

        //convert to nq from str
        operator std::unique_ptr<T, deleter_type>()
        {
            return *this;
        }
        */
	};
    
	template<class T,
		class Domain,
        class AllocStrat>
	class unique_ptr<T[], Domain, AllocStrat>
    : public std::unique_ptr<T[], nq::deleter<T[], Domain, AllocStrat>>
	{
        typedef nq::deleter<T[], Domain, AllocStrat> deleter_type;
		typedef std::unique_ptr<T[], deleter_type> parent;

        typedef typename parent::element_type element_type;
        typedef typename parent::pointer pointer;

        /*** Constructors ***/

    public:
		unique_ptr() noexcept
			: parent()
		{ // default construct
        }

        unique_ptr(std::nullptr_t ptr) noexcept
            : parent(ptr)
        { // construct unique_ptr with nullptr (equivalent to default)
        }

        explicit unique_ptr(pointer ptr) noexcept
            : parent(ptr)
        { // construct unique_ptr with T*
        }
        
        unique_ptr(pointer ptr,
                typename std::conditional<std::is_reference<deleter_type>::value,
                  deleter_type, const deleter_type&>::type del) noexcept
            : parent(ptr, del)
        {}

        unique_ptr(T *ptr,
                typename std::remove_reference<deleter_type>::type&& del) noexcept
            : parent(std::move(ptr), std::move(del))
        {}

        /*** Move constructors ***/
        
        unique_ptr(unique_ptr&& other) noexcept
            : parent(std::move(other))
        { // construct unique_ptr object that takes resource from other
        }

        template<class Y,
            class Y_Domain,
            class Y_Alloc>
        unique_ptr(unique_ptr<Y[], Y_Domain, Y_Alloc>&& other) noexcept
            : parent(std::move(other))
        { // construct unique_ptr object that takes resource from other
        }

        /*** Assigment operator ***/
        
        unique_ptr& operator=(unique_ptr&& rhs) noexcept
        {
            this->parent::operator=(std::move(rhs));
            return *this;
        }

        template<class Y,
            class Y_Domain,
            class Y_Alloc>
        unique_ptr& operator=(unique_ptr<Y[], Y_Domain, Y_Alloc>&& rhs) noexcept
        {
            this->parent::operator=(std::move(rhs));
            return *this;
        }
        
        unique_ptr& operator=(std::nullptr_t) noexcept
        {
            this->parent::operator=(nullptr);
            return *this;
        }
	};

    /*** Non member functions ***/

    template<class T, class Domain, class AllocStrat>
    struct Unique_if
    {
        typedef unique_ptr<T, Domain, AllocStrat> single_object;
    };

    template<class T, class Domain, class AllocStrat>
    struct Unique_if<T[], Domain, AllocStrat>
    {
        typedef unique_ptr<T[], Domain, AllocStrat> unknown_bound;
    };

    template<class T, size_t N, class Domain, class AllocStrat>
    struct Unique_if<T[N], Domain, AllocStrat>
    {
        typedef void known_bound;
    };

    template<class T,
        class Domain = UnknownDomain,
        class AllocStrat = DefaultAlloc,
        class... Args>
    typename Unique_if<T, Domain, AllocStrat>::single_object
    make_unique(Args&&... args) 
    {
        return unique_ptr<T, Domain, AllocStrat>(
                memlib::New<T, Domain, AllocStrat>(
                                        std::forward<Args>(args)...));
    }

    template<class T,
        class Domain = UnknownDomain,
        class AllocStrat = DefaultAlloc>
    typename Unique_if<T, Domain, AllocStrat>::unknown_bound
    make_unique(std::size_t size)
    {
        typedef typename std::remove_extent<T>::type U;
        return unique_ptr<T, Domain, AllocStrat>(
                memlib::New_array<U, Domain, AllocStrat>(size));
    }

    template<class T,
        class Domain = UnknownDomain,
        class AllocStrat = DefaultAlloc,
        class... Args>
    typename Unique_if<T, Domain, AllocStrat>::known_bound
    make_unique(Args&&...) = delete;
}

#endif // !NQ_UNIQUE_H_
