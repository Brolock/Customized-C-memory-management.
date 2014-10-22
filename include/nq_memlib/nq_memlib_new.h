#ifndef NQ_MEMLIB_NEW_H_
# define NQ_MEMLIB_NEW_H_

# include <initializer_list>
# include <cassert>
# include <cstdint>

# include "nq_allocator.h"
# include "domains.h"
# include "alloc_strat.h"

# include "nq_memlib_tools.h"

// Allow to totally desactivate the logging lib
# ifndef WITH_NQ_MEMOFF
namespace nq { namespace memlib
{
    /*
    ** New/Delete/New_array/Delete_array are tools used to simulate the
    ** standard new behaviour for LOW LEVEL memory library code (like unique_ptr
    ** or shared_ptr).
    ** Allow to avoid the extra SubHeader allocation to keep the informations
    ** of Domain, __FILE__, __LINE__ that standard new needs (that's also why
    ** they shouldn't appears in sane code, the extra logging essential!).
    */

    /*
    ** New and Delete follows the same behaviour as any stl container for
    ** allocations
    */
    template <class T,
         class Domain = UnknownDomain,
         class AllocStrat = DefaultAlloc,
         class... Args>
    T* New(Args... Parameters)
    {
        T *ptr = memlib::allocate_log<T, Domain, AllocStrat>(1,
                Domain::header_size);
        memlib::construct(ptr, std::forward<Args>(Parameters)...);
        return ptr;
    }

    template <class T,
        class Domain = UnknownDomain,
        class AllocStrat = DefaultAlloc>
    void Delete(T *ptr)
    {
        memlib::destroy(ptr);
        memlib::deallocate_log<AllocStrat>(ptr, Domain::header_size,
                memlib::remove_elem_domain<Domain>);
    }

    /* The ArrayHeader is used by New_array and Delete_array */
    struct ArrayHeader
    {
        ArrayHeader()
            : size_(0)
        {}

        ArrayHeader(std::uint32_t size)
            : size_(size)
        {}

        std::uint32_t size_;
    };

    /*
    ** New array allocate a ArrayHeader to stock the number of elements
    ** to destruct when Delete_array is called
    */
    template <class T,
         class Domain = UnknownDomain,
         class AllocStrat = DefaultAlloc>
    T* New_array(std::size_t count, std::initializer_list<T> ilist = {})
    { // allocate a raw memory of size count initialized with ilist
        assert(ilist.size() <= count &&
                "Trying to initialize a newed array (nq::New_array) with an"
                " initializer_list of greater size than the array size");

        T *usr_ptr = memlib::allocate_log<T, Domain, AllocStrat>(count,
                Domain::header_size + sizeof (ArrayHeader));
        
        /*
        ** Stock the number of elements to be destroyed when
        ** memlib::Delete_array is called
        */
        {
            ArrayHeader *array_ptr = reinterpret_cast<ArrayHeader*>(
                    memlib::get_internal_ptr(usr_ptr, sizeof (ArrayHeader)));
            construct(array_ptr, count);
        }

        // construct the elements of the allocated array
        memlib::construct_from_range(usr_ptr, count, ilist.begin(), ilist.end());
        return usr_ptr;
    }

    /* Delete_array delete an array allocated with New_array */
    template <class T,
         class Domain = UnknownDomain,
         class AllocStrat = DefaultAlloc>
    void Delete_array(T *usr_ptr)
    {
        if (usr_ptr != nullptr)
        {
            /*
            ** Recover the number of elements to be destroyed in the
            ** ArrayHeader and use it to call destroy_from_range
            */
            {
                ArrayHeader *array_ptr = reinterpret_cast<ArrayHeader*>(
                        get_internal_ptr(usr_ptr, sizeof (ArrayHeader)));
                destroy_from_range(usr_ptr, array_ptr->size_);
            }

            memlib::deallocate_log<AllocStrat>(usr_ptr,
                    Domain::header_size + sizeof(ArrayHeader),
                    memlib::remove_elem_domain<Domain>);
        }
    }
}} // namespace nq::memlib

// nq::New Delete ... just forward to new and delete
# else // !WITH_NQ_MEMOFF
namespace nq { namespace memlib
{
    template <class T,
         class Domain = UnknownDomain,
         class AllocStrat = DefaultAlloc,
         class... Args>
    T* New(Args... Parameters)
    {
        return new T(std::forward<Args>(Parameters)...);
    }

    template <class T,
        class Domain = UnknownDomain,
        class AllocStrat = DefaultAlloc>
    void Delete(T *ptr)
    {
        delete ptr;
    }


    template <class T,
         class Domain = UnknownDomain,
         class AllocStrat = DefaultAlloc>
    T* New_array(std::size_t count, const std::initializer_list<T>& ilist = {})
    { // allocate a raw memory of size count initialized with ilist
        assert(ilist.size() <= count &&
                "Trying to initialize a newed array (nq::New_array) with an"
                " initializer_list of greater size than the array size");

        T *ptr = new T[count];
        memlib::construct_from_range(ptr, count, ilist.begin(), ilist.end());
        return ptr;
    }

    template <class T,
         class Domain = UnknownDomain,
         class AllocStrat = DefaultAlloc>
    void Delete_array(T *usr_ptr)
    {
        delete[] usr_ptr;
    }
}} // namespace nq::memlib
# endif // !WITH_NQ_MEMOFF

#endif // !NQ_MEMLIB_NEW_H_
