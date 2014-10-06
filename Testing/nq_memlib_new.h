#ifndef NQ_MEMLIB_NEW_H_
# define NQ_MEMLIB_NEW_H_

# include <initializer_list>
# include <cassert>
# include <cstdint>

# include "nq_allocator.h"
# include "domains.h"
# include "alloc_strat.h"

# include "nq_memlib_tools.h"

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
    ** New and Delete follows the same behaviour as any stl container
    ** so we use nq::allocator
    */
	template <class T,
		 class Domain = UnknownDomain,
		 class AllocStrat = DefaultAlloc,
		 class... Args>
	T* New(Args... Parameters)
	{
		T *ptr = nq::allocator<T, Domain, AllocStrat>().allocate(1);
		memlib::construct(ptr, std::forward<Args>(Parameters)...);
		return ptr;
	}

	template <class T,
		class Domain = UnknownDomain,
		class AllocStrat = DefaultAlloc>
	void Delete(T *ptr)
	{
        memlib::destroy(ptr);
		nq::allocator<T, Domain, AllocStrat>().deallocate(ptr, 0);
	}

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

        // TODO To replace
        if (count == 0)
            return nullptr;
        T *internal_ptr =
            nq::memlib::allocate<T>(count * sizeof (T),
                                Domain::header_size + sizeof (ArrayHeader));
        if (internal_ptr == nullptr)
            throw std::bad_alloc();

        Domain::getInstance().add(internal_ptr, count * sizeof (T));

        T *usr_ptr = nq::memlib::get_usr_ptr(
                internal_ptr, Domain::header_size + sizeof (ArrayHeader));
        // !TODO
        
        /* Adding the number of elements to be deleted with nq::Delete_array */
        ArrayHeader *array_ptr =
            reinterpret_cast<ArrayHeader*>
                (nq::memlib::get_internal_ptr(usr_ptr, sizeof (ArrayHeader)));
		construct(array_ptr, count);

        // construct the elements of the allocated array
        construct_from_range(usr_ptr, count, ilist.begin(), ilist.end());
        return usr_ptr;
    }

    /* Delete_array delete a Newed array */
	template <class T,
		 class Domain = UnknownDomain,
		 class AllocStrat = DefaultAlloc>
    void Delete_array(T *usr_ptr)
    {
        if (usr_ptr != nullptr)
        {
            ArrayHeader *array_ptr = reinterpret_cast<ArrayHeader*>(
                    get_internal_ptr(usr_ptr, sizeof (ArrayHeader)));
            destroy_from_range(usr_ptr, array_ptr->size_);
            // TODO To replace
            void *internal_ptr = nq::memlib::get_internal_ptr(
                    usr_ptr, BaseDomain::header_size + sizeof (ArrayHeader));

            Domain::getInstance().remove(internal_ptr);

            nq::memlib::deallocate(internal_ptr);
            //!TODO
        }
    }
}} // namespace nq::memlib

#endif // !NQ_MEMLIB_NEW_H_
