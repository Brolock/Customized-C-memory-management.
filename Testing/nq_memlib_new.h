#ifndef NQ_MEMLIB_NEW_H_
# define NQ_MEMLIB_NEW_H_

# include <initializer_list>
# include <cassert>

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
		nq::allocator<T>().construct(ptr, std::forward<Args>(Parameters)...);
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

	template <class T,
		 class Domain = UnknownDomain,
		 class AllocStrat = DefaultAlloc>
    T* New_array(std::size_t count, std::initializer_list<T> ilist = {})
    {
        assert(ilist.size() <= count &&
                "Trying to initialize a newed array (nq::New_array) with an"
                " initializer_list of greater size than the array size");

		T *ptr = nq::allocator<T, Domain, AllocStrat>().allocate(count);

        // construct the elements of the allocated array
        construct_from_range(ptr, count, ilist.begin(), ilist.end());
        return ptr;
    }

	template <class T,
		 class Domain = UnknownDomain,
		 class AllocStrat = DefaultAlloc>
    T* Delete_array(T *ptr)
    {
    //    destroy_from_range(ptr, )
    }
}} // namespace nq::memlib

#endif // !NQ_MEMLIB_NEW_H_
