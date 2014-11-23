#ifndef DOMAINS_H_
# define DOMAINS_H_

# include <mutex>
# include <fstream>

# include <nq_memlib/base_domain.h>
# include <nq_memlib/lib_domains.h>

# include "log_path.h"

namespace nq { namespace log {
    /* forward declaration of the function defined in nq_log_printer.cpp*/
    void print_helper(std::ostream& os, const char* message);

    inline void print(::std::ostream& os,
            const char* message = "No specific message")
    {
# ifdef WITH_NQ_MEMLOG
        static std::mutex mutex;
        std::lock_guard<std::mutex> locker(mutex);
        os << "==================\n";

        print_helper(os, message);

        AllDomains::getInstance().print(os);
        os << "==================\n";
# endif // !WITH_NQ_MEMLOG
    }

    inline void print_file(std::string filename,
           const char* message = "No specific message")
    {
        std::ofstream file(nq::log::path + filename, std::ios_base::app);
        print(file, message);
    }

    inline void dump(std::string filename, const char* message = "dump_leak!")
    {
# ifdef WITH_NQ_MEMLOG
        size_t res = 0;

        /* Library defined domains */
        res += UnknownDomain::getInstance().get_count();
        res += SharedPtrRefCountDomain::getInstance().get_count();

        /* User defined domains */
        /* TODO count user define domains size */

        if (res)
            print_file(filename, message);
# endif // !WITH_NQ_MEMLOG
    }
}} // namespace nq::log

#endif // !DOMAINS_H_
