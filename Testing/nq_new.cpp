#include "nq_new.h"

void operator delete(void *usr_ptr) noexcept
{
    // TODO To replace
    /*
    if (usr_ptr != nullptr)
    {
        void *internal_ptr =
            nq::memlib::get_internal_ptr(usr_ptr, BaseDomain::sub_header_size);

        nq::memlib::remove_header_operator_delete(internal_ptr);

        nq::memlib::deallocate(internal_ptr);
    }
    */
    nq::memlib::test_deallocate(usr_ptr, BaseDomain::sub_header_size,
                    nq::memlib::remove_header_operator_delete);
}

void operator delete[](void *usr_ptr) noexcept
{
    operator delete(usr_ptr);
}

void* operator new(size_t count)
{
    // TODO To replace
    void *internal_ptr =
        nq::memlib::allocate<NewedType>(count, UnknownDomain::sub_header_size);
    if (internal_ptr == nullptr)
        throw std::bad_alloc();

    UnknownDomain::getInstance().add(internal_ptr, count,
            "Don't use standard new", 999, &(UnknownDomain::getInstance()));
 
    void *usr_ptr = 
        nq::memlib::get_usr_ptr(internal_ptr, UnknownDomain::sub_header_size);
    return usr_ptr;
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
