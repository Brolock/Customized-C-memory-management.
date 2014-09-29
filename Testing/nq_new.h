#ifndef NQ_NEW_H_
# define NQ_NEW_H_

# include "nq_allocator.h"
# include "domains.h"

class Newed_type
{
    char size;
};

template<class Domain>
void *operator new(size_t count,
        const Domain&, const char *file, size_t line) noexcept
{
    char *internal_ptr = static_cast<char*>
        (std::malloc(count + Domain::sub_header_size));

    if (internal_ptr == nullptr)
        throw std::bad_alloc();

    Domain::getInstance().add(internal_ptr, count,
            file, line, &(Domain::getInstance()));
 
    void *usr_ptr = reinterpret_cast<void*>
        (internal_ptr + Domain::sub_header_size);
    return usr_ptr;
}

#endif // !NQ_NEW_H_
