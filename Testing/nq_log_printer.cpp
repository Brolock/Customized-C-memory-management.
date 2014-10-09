# include "domains.h"

namespace nq { namespace log {
    void print_time(std::ostream& os);

    /*
    ** The print_helper fonction is here to avoid having a lot of non needed
    ** code in the print() function in domains.h.
    ** It calls all the sub print functions.
    */
    void print_helper(std::ostream& os, const char *message)
    {
        os << "logged at:";
        print_time(os);

        os << message << std::endl;
    }

    /* Print the time of the print in h/min/sec format */
    /* This function is doing 10 allocs */
# include <ctime>
    void print_time(::std::ostream& os)
    {
        time_t t = time(0); // get current time
        tm *now = localtime(&t);
        os  << now->tm_hour << "h"
            << now->tm_min << "min"
            << now->tm_sec << "sec"
            << ::std::endl;
    }
}} // namespace nq::log
