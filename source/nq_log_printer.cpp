#include <iostream>
#include <ctime>

#include "../include/nq_memlib/env_maccro.h"

#ifdef NQ_GNU_
# include <pthread.h>
#else
# include <Windows.h>
#endif

namespace nq { namespace log {
    void print_time(std::ostream& os);

    /*
    ** The print_helper fonction is here to avoid having a lot of non needed
    ** code in the print() function in domains.h.
    ** It calls all the print options (like displaying time, message...)
    */
    void print_helper(std::ostream& os,  const char* message)
    {
        os << message << std::endl;

#ifdef NQ_GNU_
        os << "Thread: " << pthread_self() << std::endl;
#else // NQ_GNU_ (NQ_WIN_ defined)
        os << "Thread: " << GetCurrentThreadId() << std::endl;
#endif // !NQ_GNU_
#ifdef WITH_NQ_LOGTIME
        print_time(os);
#endif // !WITH_NQ_LOGTIME
    }

    /* Print the time of the print in h/min/sec format */
    /* This function is doing 10 allocs */
    void print_time(::std::ostream& os)
    {
        os << "logged at: ";

        time_t t = time(0); // get current time
        tm *now = localtime(&t);
        os  << now->tm_hour << "h"
            << now->tm_min << "min"
            << now->tm_sec << "sec"
            << ::std::endl;
    }
}} // namespace nq::log
