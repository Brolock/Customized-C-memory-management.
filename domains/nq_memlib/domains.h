#ifndef DOMAINS_H_
# define DOMAINS_H_

# include <mutex>
# include <fstream>

# include <nq_memlib/base_domain.h>
# include <nq_memlib/lib_domains.h>


/* User defined domains */
# define NQ_USR_DOMAIN(domain_name) NQ_DOMAIN(domain_name)
# include "domains_decl.h"
# undef NQ_USR_DOMAIN

namespace nq { namespace log {
    /* forward declaration of the function defined in nq_log_printer.cpp*/
    void print_helper(std::ostream& os, const char *message);

    inline void print(::std::ostream& os = std::cout,
            const char *message = "No specific message")
    {
# ifdef WITH_NQ_MEMLOG
        static std::mutex mutex;
        std::lock_guard<std::mutex> locker(mutex);
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

    inline void print_file(const char *path,
           const char *message = "No specific message")
    {
        std::ofstream file(path, std::ios_base::app);
        print(file, message);
    }

    inline void dump(const char *path)
    {
# ifdef WITH_NQ_MEMLOG
        size_t res = 0;

        /* Library defined domains */
        res += UnknownDomain::getInstance().get_count();
        res += SharedPtrRefCountDomain::getInstance().get_count();

        /* User defined domains */
#  define NQ_USR_DOMAIN(domain_name) res += domain_name::getInstance().get_count();
#  include "domains_decl.h"
#  undef NQ_USR_DOMAIN

        if (res)
            print_file(path, "dump_leaks!");
# endif // !WITH_NQ_MEMLOG
    }
}} // namespace nq::log

#endif // !DOMAINS_H_
