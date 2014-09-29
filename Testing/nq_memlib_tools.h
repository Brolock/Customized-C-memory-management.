#ifndef NQ_MEMLIB_TOOLS_H_
# define NQ_MEMLIB_TOOLS_H_

namespace nq
{
    /*
    ** Construct at pointer ptr, count number of type T objects by copying them
    ** from the elements in the range begin - end.
    */
    template<class T,
        class Iter>
    void construct_from_range(T * ptr, std::size_t count,
            const Iter& begin, const Iter& end)
    {
        nq::allocator<T, UnknownDomain, DefaultAlloc> allocator;
        size_t i = 0;
        for(Iter it = begin; it != end; ++it)
        {
            allocator.construct(ptr + i, *it);
            ++i;
        }
        for(; i < count; ++i)
        {
            allocator.construct(ptr + i);
        }
    }
}

#endif // !NQ_MEMLIB_TOOLS_H_
