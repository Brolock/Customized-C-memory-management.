#ifndef NQ_MEMLIB_TOOLS_H_
# define NQ_MEMLIB_TOOLS_H_

# include <utility>

namespace nq { namespace memlib
{
    /*
     * */
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
}} // namespace nq::memlib

#endif // !NQ_MEMLIB_TOOLS_H_
