#ifndef NQ_UNIQUE_H_
# define NQ_UNIQUE_H_

#include <memory>

# include "nq_deleter.h"
# include "alloc_strat.h"

# include "nq_new.h"

namespace nq
{

    /*
    ** nq::unique have two implementations types:
    ** => nq::unique<T, nq::new_deleter, DefaultAlloc, bool_is_Domain = false>
    **   This one is used by default when someone declare a nq::unique<T>
    **   so it follows the standard behaviour where :
    **      nq::unique<T> j(new T());
    **      nq::unique<T> j = nq::make_unique<T>(T_args);
    ** => nq::unique<T, Domain, DefaultAlloc, bool_is_Domain = true>
    **    This one is used as the other library member when you want to log
    **    your memory in a specific Domain :
    **      nq::unique<T, Domain> j(NQ_NEW(Domain) T());
    **      nq::unique<T, Domain> j = nq::make_unique<T, Domain>(T_args);
    **
    ** The use of auto helps to avoid confusion on which type nq::unique
    ** should be declared with
    */

    /* Implementation specified Domain: unique<T, Domain, AllocStrat, true> */
    template<class T,
        class DeleterOrDomain = nq::new_deleter<T>,
        class AllocStrat = DefaultAlloc,
        bool = std::is_convertible<DeleterOrDomain*, BaseDomain*>::value>
    class unique_ptr
    : public std::unique_ptr<T, nq::new_deleter<T>>
    //: public std::unique_ptr<T, nq::deleter<T, DeleterOrDomain, AllocStrat>>
    {
        //typedef nq::deleter<T, DeleterOrDomain, AllocStrat> deleter_type;
        typedef nq::new_deleter<T> deleter_type;
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
                typename std::conditional<
                  std::is_reference<deleter_type>::value,
                    deleter_type, const deleter_type&>::type del) noexcept
            : parent(ptr, del)
        {}

        unique_ptr(T *ptr,
                typename std::remove_reference<
                  deleter_type>::type&& del) noexcept
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
    
    /* Implementation specified Domain: unique<T[], Domain, AllocStrat, true> */
    template<class T,
        class Domain,
        class AllocStrat>
    class unique_ptr<T[], Domain, AllocStrat, true>
    : public std::unique_ptr<T[], nq::new_deleter<T[]>>
    //: public std::unique_ptr<T[], nq::deleter<T[], Domain, AllocStrat>>
    {
        typedef nq::new_deleter<T[]> deleter_type;
        //typedef nq::deleter<T[], Domain, AllocStrat> deleter_type;
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

    /*
    ** Implementation not specified Domain:
    **      unique<T, Deleter, AllocStrat, false>
    */
    template<class T,
        class Deleter,
        class AllocStrat>
    class unique_ptr<T, Deleter, AllocStrat, false>
    : public std::unique_ptr<T, nq::new_deleter<T>>
    {
        typedef nq::new_deleter<T> deleter_type;
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
                typename std::conditional<
                  std::is_reference<deleter_type>::value,
                    deleter_type, const deleter_type&>::type del) noexcept
            : parent(ptr, del)
        {}

        unique_ptr(T *ptr,
                typename std::remove_reference<
                  deleter_type>::type&& del) noexcept
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
    
    /*
    ** Implementation not specified Domain:
    **      unique<T[], Deleter, AllocStrat, false>
    */
    template<class T,
        class Deleter,
        class AllocStrat>
    class unique_ptr<T[], Deleter, AllocStrat, false>
    : public std::unique_ptr<T[], nq::new_deleter<T[]>>
    {
        typedef Deleter deleter_type;
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
                typename std::conditional<
                  std::is_reference<deleter_type>::value,
                    deleter_type, const deleter_type&>::type del) noexcept
            : parent(ptr, del)
        {}

        unique_ptr(T *ptr,
                typename std::remove_reference<
                    deleter_type>::type&& del) noexcept
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


    /*** nq::make_unique implementation ***/
    /* nq::make_unique follows the standard implementation of C++14 */

    /*****************************************************/
    /** Implementation for specified Domain make_unique **/
    /*****************************************************/

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
        class Domain,
        class AllocStrat = DefaultAlloc,
        class... Args>
    typename Unique_if<T, Domain, AllocStrat>::single_object
    make_unique(Args&&... args) 
    {
        /*
        return unique_ptr<T, Domain, AllocStrat>(
                memlib::New<T, Domain, AllocStrat>(
                                        std::forward<Args>(args)...));
        */
        return unique_ptr<T, Domain, AllocStrat>(
                INTERNAL_NQ_NEW(
                    Domain::getInstance(), __LINE__, "make_unique implem")
                    T(std::forward<Args>(args)...));
    }

    template<class T,
        class Domain,
        class AllocStrat = DefaultAlloc>
    typename Unique_if<T, Domain, AllocStrat>::unknown_bound
    make_unique(std::size_t size)
    {
        /*
        typedef typename std::remove_extent<T>::type U;
        return unique_ptr<T, Domain, AllocStrat>(
                memlib::New_array<U, Domain, AllocStrat>(size));
        */
        typedef typename std::remove_extent<T>::type U;
        return unique_ptr<T, Domain, AllocStrat>(
                INTERNAL_NQ_NEW(
                    Domain::getInstance(), __LINE__, "make_unique implem")
                    U[size]);
    }

    template<class T,
        class Domain,
        class AllocStrat = DefaultAlloc,
        class... Args>
    typename Unique_if<T, Domain, AllocStrat>::known_bound
    make_unique(Args&&...) = delete;

    /*********************************************************/
    /** Implementation for NOT specified Domain make_unique **/
    /*********************************************************/

    template<class T>
    struct Unique_if_NoDomain
    {
        typedef unique_ptr<T> single_object;
    };

    template<class T>
    struct Unique_if_NoDomain<T[]>
    {
        typedef unique_ptr<T[]> unknown_bound;
    };

    template<class T, size_t N>
    struct Unique_if_NoDomain<T[N]>
    {
        typedef void known_bound;
    };

    template<class T,
        class... Args>
    typename Unique_if_NoDomain<T>::single_object
    make_unique(Args&&... args) 
    {

        return unique_ptr<T>(INTERNAL_NQ_NEW(
                  UnknownDomain::getInstance(), __LINE__, "make_unique implem")
                  T(std::forward<Args>(args)...));
    }

    template<class T>
    typename Unique_if_NoDomain<T>::unknown_bound
    make_unique(std::size_t size)
    {

        typedef typename std::remove_extent<T>::type U;
        return unique_ptr<T>(INTERNAL_NQ_NEW(
                  UnknownDomain::getInstance(), __LINE__, "make_unique implem")
                  U[size]);
    }

    template<class T,
        class... Args>
    typename Unique_if_NoDomain<T>::known_bound
    make_unique(Args&&...) = delete;
}

#endif // !NQ_UNIQUE_H_
