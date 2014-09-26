#include "nq_new.h"

void operator delete(void *usr_ptr) noexcept
{
    if (usr_ptr != nullptr)
    {
        void *internal_ptr =
            reinterpret_cast<char *>(usr_ptr) - DomainEarth::header_size - 4 * sizeof(size_t);

        void *domain_ptr = reinterpret_cast<char *>(internal_ptr) + DomainEarth::header_size + 2 * sizeof(size_t);


        (reinterpret_cast<BaseDomain*>(domain_ptr))->virtual_remove(internal_ptr);

        std::free(internal_ptr);
    }
}
