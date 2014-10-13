#ifndef TYPE_TO_STRING_H_
# define TYPE_TO_STRING_H_

#include "domains.h"
#include "alloc_strat.h"

namespace nq
{
    template<typename T>
    const char* TypeToString()
    {
        return "UnknownType";
    }

#define COMMA ,
#define REGISTER_TYPE_NAME(NAME) template <>\
    const char* TypeToString<NAME>() { return #NAME; };

    /***********************/
    /* User specific types */
    /***********************/

    REGISTER_TYPE_NAME(nq::allocator<int>);
    //REGISTER_TYPE_NAME(nq::allocator<int COMMA DomainEarth COMMA DefaultAlloc>);
    //REGISTER_TYPE_NAME(DomainEarth);

    /********************/
    /* STL common types */
    /********************/
    REGISTER_TYPE_NAME(std::nullptr_t);


    /******************/
    /* Builtins types */
    /******************/
    //char
    REGISTER_TYPE_NAME(char);
    REGISTER_TYPE_NAME(unsigned char);
    REGISTER_TYPE_NAME(signed char);
    //int
    REGISTER_TYPE_NAME(int);
    REGISTER_TYPE_NAME(unsigned int);
    REGISTER_TYPE_NAME(short int);
    REGISTER_TYPE_NAME(unsigned short int);
    REGISTER_TYPE_NAME(long int);
    REGISTER_TYPE_NAME(unsigned long int);
    //misc
    REGISTER_TYPE_NAME(float);
    REGISTER_TYPE_NAME(double);
    REGISTER_TYPE_NAME(long double);
    REGISTER_TYPE_NAME(wchar_t);
    REGISTER_TYPE_NAME(bool);
    REGISTER_TYPE_NAME(void);
#undef COMMA
}

/*
** could also works with VA_ARGS (variadic args in maccros) but need to write
** the name twice: 
#define SET_TYPE_NAME(NAME, ...) \
template<typename T>          \
std::string name();           \
                              \
template<>                    \
std::string name<__VA_ARGS__>() {    \
    return NAME;              \
}
*/

#endif // !TYPE_TO_STRING_H_
