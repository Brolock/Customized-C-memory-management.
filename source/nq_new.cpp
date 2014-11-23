#include "../include/nq_memlib/nq_new.h"

namespace nq { namespace memlib {
# ifdef WITH_NQ_MEMLOG
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

# else // WITH_NQ_MEMLOG
    void remove_header_operator_delete(void*) {}
# endif // !WITH_NQ_MEMLOG

}} // namespace nq::memlib
