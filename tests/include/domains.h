#ifndef DOMAINS_H_
# define DOMAINS_H_

# include <mutex>

# include <nq_memlib/base_domain.h>
# include <nq_memlib/lib_domains.h>


/* !!! Add your domain to the print method if you want it printed !!!*/
/* User defined domains */
# define NQ_USR_DOMAIN(domain_name) NQ_DOMAIN(domain_name)
# include "domains_decl.h"
# undef NQ_USR_DOMAIN

namespace nq { namespace log {
    /* forward declaration of the function defined in nq_log_printer.cpp*/
    void print_helper(std::ostream& os, const char *message);

    /*
    ** /!\                                 /!\
    ** After declaring a Domain a user should add his
    ** MyDomain::getInstance().print() line if he wants it to be printed.
    ** /!\                                 /!\
    */
    inline void print(::std::ostream& os = std::cout,
            const char *message = "No specific message")
    {
        static std::mutex mutex;
        std::lock_guard<std::mutex> locker(mutex);
# ifdef WITH_NQ_MEMLOG
        os << "==================\n";

        print_helper(os, message);

        /* Library defined domains */
        UnknownDomain::getInstance().print(os);
        SharedPtrRefCountDomain::getInstance().print(os);

        /* User defined domains */
#  define NQ_USR_DOMAIN(domain_name) domain_name::getInstance().print(os);
#  include "domains_decl.h"
#  undef NQ_USR_DOMAIN

        os << "==================\n";
# endif // !WITH_NQ_MEMLOG
    }
}} // namespace nq::log

#endif // !DOMAINS_H_