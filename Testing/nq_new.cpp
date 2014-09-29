#include "nq_new.h"

void operator delete(void *usr_ptr) noexcept
{
    if (usr_ptr != nullptr)
    {
        void *internal_ptr =
            reinterpret_cast<char *>(usr_ptr) - BaseDomain::sub_header_size;
		BaseDomain::SubHeader* subHeader = reinterpret_cast<BaseDomain::SubHeader*>(internal_ptr);

		BaseDomain* domain_ptr = subHeader->get_domain();
		domain_ptr->virtual_remove(internal_ptr);
	
        std::free(internal_ptr);
    }
}
