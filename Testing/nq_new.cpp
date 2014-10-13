#include "nq_new.h"

void operator delete(void *usr_ptr) noexcept
{
    nq::memlib::deallocate_log(usr_ptr, BaseDomain::sub_header_size,
                    nq::memlib::remove_header_operator_delete);
}

void operator delete[](void *usr_ptr) noexcept
{
    operator delete(usr_ptr);
}

void* operator new(size_t count)
{
    return nq::memlib::allocate_log<NewedType>(count,
            UnknownDomain::getInstance().sub_header_size, "Do not use new", 0);
}

void* operator new[](size_t count)
{
    return operator new(count);
}

namespace nq { namespace memlib {
#ifdef WITH_NQ_MEMLOG
    /*
    ** Function used by operator delete to recover the SubHeader of a pointer
    ** and call the logged Domain to remove the log
    */
    void remove_header_operator_delete(void *ptr)
    {
        BaseDomain::SubHeader* subHeader =
            reinterpret_cast<BaseDomain::SubHeader*>(ptr);

        BaseDomain* domain_ptr = subHeader->get_domain();
        domain_ptr->virtual_remove(ptr);
    }
#else // WITH_NQ_MEMLOG
    void remove_header_operator_delete(void *ptr) {}
#endif // !WITH_NQ_MEMLOG
}} // namespace nq::memlib
