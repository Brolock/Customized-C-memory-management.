#ifndef NQ_DELETER_H_
# define NQ_DELETER_H_

# include <type_traits>
# include <memory>

# include "nq_allocator.h"
# include "nq_memlib_new.h"
# include "nq_new.h"

# ifndef WITH_NQ_MEMOFF
/* 
** The deleter class is used in RAII classes to know the way a given
** pointer needs to be deleted.
** It's the operator() associated with this class that define the way the
** pointer should be deleted
*/

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

    /*
    ** nqNew_deleter used by to delete NQ_NEWed pointeurs 
    */
    template <typename T>
    struct nqNew_deleter
    {
        nqNew_deleter()
        { // default construct
        }

        template <class U>
        nqNew_deleter(const nqNew_deleter<U>&,
                typename std::enable_if<
                  std::is_convertible<U*, T*>::value>::type* = nullptr)
        { // construct from another deleter
        }

        void operator()(T *ptr) const
        { //delete the ptr with allocstrat
            nqDelete(ptr);
        }
    };

    template <typename T>
    struct nqNew_deleter <T[]>
    {
        nqNew_deleter()
        { // default construct
        }

        template <typename U>
        void operator()(U *ptr) const = delete;

        void operator()(T *ptr) const
        {
            nqDeleteArray(ptr);
        }
    };

    /*
    ** new_deleter used to call operator delete
    */
    template <typename T>
    struct new_deleter
    {
        new_deleter()
        { // default construct
        }

        template <class U>
        new_deleter(const new_deleter<U>&,
                typename std::enable_if<
                  std::is_convertible<U*, T*>::value>::type* = nullptr)
        { // construct from another deleter
        }

        void operator()(T *ptr) const
        { //delete the ptr with allocstrat
            delete ptr;
        }
    };

    template <typename T>
    struct new_deleter <T[]>
    {
        new_deleter()
        { // default construct
        }

        template <typename U>
        void operator()(U *ptr) const = delete;

        void operator()(T *ptr) const
        {
            delete[](ptr);
        }
    };
}

/************************/
/* Memlib is turned off */
/************************/

// deleters are simple std::default_delete
# else // WITH_NQ_MEMOFF defined
namespace nq
{
    template<class T,
        class Domain = UnknownDomain,
        class AllocStrat = DefaultAlloc>
    using deleter = std::default_delete<T>;

    template<class T>
    using nqNew_deleter = std::default_delete<T>;

    template<class T>
    using new_deleter = std::default_delete<T>;
}
# endif // !WITH_NQ_MEMOFF

#endif // !NQ_DELETER_H_
