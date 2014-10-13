#ifndef ALLOC_STRAT_H_
# define ALLOC_STRAT_H_

# include <cstdlib>

/*
 * TODO
template < int T, bool ... > class PoolAlloc
{};
template <int size = 126, bool ThreadSafe = true>
*/

struct DefaultAlloc
{
    void* allocate(std::size_t size)
    {
        return std::malloc(size);
    }

    void deallocate(void *ptr)
    {
        std::free(ptr);
    }
};

#endif // !ALLOC_STRAT_H_
