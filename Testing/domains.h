#ifndef DOMAINS_H_
# define DOMAINS_H_

# include "base_domain.h"

NQ_DOMAIN(UnknownDomain);
NQ_DOMAIN(DomainEarth);
NQ_DOMAIN(DomainSpace);

NQ_DOMAIN(SharedPtrRefCountDomain);

# undef NQ_DOMAIN

namespace nq { namespace log {
    /* forward declaration of the function declared in nq_log_printer.cpp*/
    void print_helper(std::ostream& os, const char *message);

    inline void print(::std::ostream& os = ::std::cout,
            const char *message = "No specific message")
    {
        os << "==================\n";

        print_helper(os, message);

        UnknownDomain::getInstance().print(os);
        DomainEarth::getInstance().print(os);
        DomainSpace::getInstance().print(os);
        SharedPtrRefCountDomain::getInstance().print(os);

        os << "==================\n";
    }
}} // namespace nq::log

#endif // !DOMAINS_H_
