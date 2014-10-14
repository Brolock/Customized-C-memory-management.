#ifndef DOMAINS_H_
# define DOMAINS_H_

# include "base_domain.h"

/* Library defined domains */
NQ_DOMAIN(UnknownDomain);
NQ_DOMAIN(SharedPtrRefCountDomain);

/* !!! Add your domain to the print method if you want it printed !!!*/
/* User defined domains */
NQ_DOMAIN(DomainEarth);
NQ_DOMAIN(DomainSpace);

# undef NQ_DOMAIN

namespace nq { namespace log {
    /* forward declaration of the function defined in nq_log_printer.cpp*/
    void print_helper(std::ostream& os, const char *message);

    /*
    ** After declaring a Domain a user should add his
    ** MyDomain::getInstance().print() line if he wants it to be printed.
    */
    inline void print(::std::ostream& os = std::cout,
            const char *message = "No specific message")
    {
# ifdef WITH_NQ_MEMLOG
        os << "==================\n";

        print_helper(os, message);

        /* Library defined domains */
        UnknownDomain::getInstance().print(os);
        SharedPtrRefCountDomain::getInstance().print(os);

        /* User defined domains */
        DomainEarth::getInstance().print(os);
        DomainSpace::getInstance().print(os);

        os << "==================\n";
# endif // !WITH_NQ_MEMLOG
    }
}} // namespace nq::log

#endif // !DOMAINS_H_
