#ifndef NQ_MEMLIB_TOOLS_H_
# define NQ_MEMLIB_TOOLS_H_

# include <utility>
# include <functional>

# include "alloc_strat.h"
# include "domains.h"
# include "nq_memlib_allocate.h"

namespace nq { namespace memlib
{ 
    /*
    ** construct() and destroy() construct and destroy an element of type T
    ** at pointer ptr
    */
    template<class T,
        class... Args>
    void construct(T *ptr, Args&&... args)
    {
        new (static_cast<void*>(ptr)) T(std::forward<Args>(args)...);
    }

    template<class T>
    void construct(T *ptr, const T& val)
    {
        new (static_cast<void*>(ptr)) T(val);
    }

    template<class T>
    void destroy(T *ptr)
    {
        ptr->~T();
    }

    /*
    ** Construct at pointer ptr, count number of type T objects by copying them
    ** from the elements in the range begin - end.
    ** To use with destroy_from_range(T*, size_t)
    */
    template<class T,
        class Iter>
    void construct_from_range(T *ptr, std::size_t count,
            const Iter& begin, const Iter& end)
    {
        size_t i = 0;
        for(Iter it = begin; it != end; ++it)
        {
            construct<T>(ptr + i, *it);
            ++i;
        }
        for(; i < count; ++i)
        {
            construct<T>(ptr + i);
        }
    }

    /* 
    ** Destroy from pointer ptr count object T.
    ** To use with construct_from_range(T*, size_t, Iter, Iter)
    */
    template<class T>
    void destroy_from_range(T *ptr, std::size_t count)
    {
        for(std::size_t i = 0; i < count; ++i)
        {
            destroy<T>(ptr + i);
        }
    }

    /*
    ** get_usr_ptr and get_internal_ptr, hide the pointer arithmetic done
    ** when recovering or placing a Header
    */
    template<typename T>
    T* get_usr_ptr(T *internal_ptr, std::size_t headers)
    {
        char *arithmetic_ptr = reinterpret_cast<char*>(internal_ptr);
        arithmetic_ptr += headers;
        return reinterpret_cast<T*>(arithmetic_ptr);
    }

    template<typename T>
    T* get_internal_ptr(T* usr_ptr, std::size_t headers)
    {
        char *arithmetic_ptr = reinterpret_cast<char*>(usr_ptr);
        arithmetic_ptr -= headers;
        return reinterpret_cast<T*>(arithmetic_ptr);
    }

    /* allocate_log and deallocate_log are used by every allocating class */
    /*
    ** allocate with AllocStrat a new pointer of count elements T, and
    ** log it in Domain.
    */
    template<class T,
        class Domain = UnknownDomain,
        class AllocStrat = DefaultAlloc>
    T* allocate_log(size_t count, size_t headers)
    {
        if (count == 0)
            return nullptr;
        T *internal_ptr = memlib::allocate<T, AllocStrat>(count, headers);

        if (internal_ptr == nullptr)
            throw std::bad_alloc();

        Domain::getInstance().add(internal_ptr, count * sizeof (T));

        return memlib::get_usr_ptr(internal_ptr, headers);
    }

    /* This allocate_log overload is used by operator new */
    template<class T,
        class Domain = UnknownDomain,
        class AllocStrat = DefaultAlloc>
    T* allocate_log(size_t count, size_t headers,
            const char *file, size_t line)
    {
        if (count == 0)
            return nullptr;
        T *internal_ptr = memlib::allocate<T, AllocStrat>(count, headers);

        if (internal_ptr == nullptr)
            throw std::bad_alloc();

        Domain::getInstance().add(internal_ptr, count,
                file, line, &Domain::getInstance());

        return memlib::get_usr_ptr(internal_ptr, headers);
    }

    /*
    ** deallocate with AllocStrat pointer ptr, and unlog it in Domain.
    */
    template<class AllocStrat = DefaultAlloc>
    void deallocate_log(void *usr_ptr, size_t headers,
            std::function<void(void*)> remove_header_fun)
    {
        if (usr_ptr != nullptr)
        {
            void *internal_ptr = get_internal_ptr(usr_ptr, headers);
            remove_header_fun(internal_ptr);
            memlib::deallocate<AllocStrat>(internal_ptr);
        }
    }
}} // namespace nq::memlib

#endif // !NQ_MEMLIB_TOOLS_H_
