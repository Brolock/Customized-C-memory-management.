#include "nq_new.h"

void operator delete(void *usr_ptr) noexcept
{
    // TODO To replace
    if (usr_ptr != nullptr)
    {
        void *internal_ptr =
            nq::memlib::get_internal_ptr(usr_ptr, BaseDomain::sub_header_size);

        nq::memlib::remove_header_operator_delete(internal_ptr);

        nq::memlib::deallocate(internal_ptr);
    }
}

void operator delete[](void *usr_ptr) noexcept
{
    operator delete(usr_ptr);
}

namespace nq { namespace memlib {
    void remove_header_operator_delete(void *ptr)
    {
		BaseDomain::SubHeader* subHeader =
            reinterpret_cast<BaseDomain::SubHeader*>(ptr);

		BaseDomain* domain_ptr = subHeader->get_domain();
		domain_ptr->virtual_remove(ptr);
    }
}} // namespace nq::memlib
