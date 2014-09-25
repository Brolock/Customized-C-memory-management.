#ifndef NQ_NEW_H_
# define NQ_NEW_H_

# include "nq_allocator.h"
# include "domains.h"
# include "alloc_strat.h"

namespace nq
{
	template <class T,
		 class Domain = UnknownDomain,
		 class AllocStrat = DefaultAlloc,
		 class... Args>
	T* New(Args... Parameters)
	{
		T *ptr = nq::allocator<T, Domain, AllocStrat>().allocate(1);
		nq::allocator<T, Domain, AllocStrat>().construct(ptr, std::forward<Args>(Parameters)...);
		return ptr;
	}

	template <class T,
		 class Domain = UnknownDomain,
		 class AllocStrat = DefaultAlloc>
    T* New_array(std::size_t count)
    {
		T *ptr = nq::allocator<T, Domain, AllocStrat>().allocate(count);
        for(std::size_t i = 0; i < count; ++i)
        {
		    nq::allocator<T, Domain, AllocStrat>().construct(ptr + i);
        }
    }

	template <class T,
		class Domain = UnknownDomain,
		class AllocStrat = DefaultAlloc>
	void Delete(T *ptr)
	{
		nq::allocator<T, Domain, AllocStrat>().destroy(ptr);
		nq::allocator<T, Domain, AllocStrat>().deallocate(ptr, 0);
	}
}

class Newed_type
{
    char size;
};

class Virtual_Domain
{
    virtual void v_deallocate() = 0;
};

class Header
{
    Virtual_Domain *ID;
    size_t padding;
    size_t line;
    char* file;
};

# define NQ_NEW(Domain) new(Domain, __FILE__, __LINE__)

void *operator new[](std::size_t count)
{
    return nq::allocator<Newed_type, UnknownDomain>().allocate(count + sizeof(Header));
}

void operator delete[](void *ptr) noexcept
{
    if (ptr != nullptr)
    {
        /*
        Header* head = nullptr; // arithmetic
        Virtual_Domain* a = head->ID;
        a->virtual_remove(head);
        */
    }

    /*
    int *test = (int*) ptr;
    std::cout << *test << std::endl;

    nq::allocator<Newed_type, UnknownDomain>().deallocate(static_cast<Newed_type *>(ptr), 0);
    */
}

template<class Domain>
void *operator new(size_t count, const Domain&)  noexcept
{
    std::cout << "salut" << std::endl;
    std::cout << count << std::endl;
    return nq::allocator<Newed_type, Domain>().allocate(count);
}

#endif /* !NQ_NEW_H_ */
