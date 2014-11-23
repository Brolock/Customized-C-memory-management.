#ifndef NQ_NEW_H_
# define NQ_NEW_H_

# include "nq_memlib_allocate.h"
# include "nq_memlib_tools.h"

# include "lib_domains.h"

# ifndef WITH_NQ_MEMOFF
/*
** NQ_NEW is logged with its file line for easier leak detections
** The Domain is also logged to be recovered by operator delete
*/
#  define NQ_NEW(Domain) new (Domain::getInstance(), __LINE__, __FILE__)

class NewedType
{
    char size;
};

template<class Domain>
void* operator new(size_t count,
        const Domain&, size_t line, const char* file) noexcept
{
    return nq::memlib::allocate_log<NewedType, Domain>(count,
            Domain::sub_header_size, file, line);
}

namespace nq { namespace memlib {
    void remove_header_operator_delete(void *ptr);
}} // namespace nq::memlib

/** This specific NEW is only reserved for internal implementations **/
# define INTERNAL_NQ_NEW(Domain) new (Domain, 0, nullptr)


/*********************/
/* Memlib turned off */
/*********************/
# else // WITH_NQ_MEMOFF defined

#  define NQ_NEW(Domain) new

/** This specific NEW is only reserved for internal implementations **/
#  define INTERNAL_NQ_NEW(Domain) new

# endif // !WITH_NQ_MEMOFF

/*****************************************/
/* New Implementation of News and Delete */
/*****************************************/

# define NQ_DELETE(ptr) nqDelete(ptr)

# define NQ_NEW_ARRAY(Domain, T, size)\
    nqNewArray<T, Domain>(__FILE__, __LINE__, size)

# define NQ_DELETE_ARAY(ptr) nqDeleteArray(ptr)

# ifndef WITH_NQ_MEMOFF

namespace nq { namespace memlib {
    void remove_header_operator_delete(void *ptr); }}

template<class T>
void nqDelete(T *ptr)
{
    if (ptr != nullptr)
    {
        nq::memlib::destroy(ptr);
        nq::memlib::deallocate_log(ptr, BaseDomain::sub_header_size,
                nq::memlib::remove_header_operator_delete);
    }
}

struct ArrayHeader
{
    ArrayHeader(uint32_t size)
        : count_(size)
    {}

    uint32_t count_;
};

template<class T,
    class Domain>
T* nqNewArray(const char* file, int line, size_t count)
{
    /* Allocate the user + header + arraHeader size and log with file line */
    T *usr_ptr = nq::memlib::allocate_log<T, Domain>(count * sizeof (T),
            Domain::sub_header_size + sizeof (ArrayHeader), file, line);

    /*
    ** Stock the number of elements to be destroyed when
    ** memlib::Delete_array is called
    */
    {
        ArrayHeader *array_ptr = reinterpret_cast<ArrayHeader*>(
                nq::memlib::get_internal_ptr(usr_ptr, sizeof (ArrayHeader)));
        nq::memlib::construct<ArrayHeader>(array_ptr, count);
    }
    return usr_ptr;
}

template<class T>
void nqDeleteArray(T *usr_ptr)
{
    if (usr_ptr != nullptr)
    {
        /*
        ** Recover the number of elements to be destroyed in the
        ** ArrayHeader and use it to call destroy_from_range
        */
        {
            ArrayHeader *array_ptr = reinterpret_cast<ArrayHeader*>(
                nq::memlib::get_internal_ptr(usr_ptr, sizeof (ArrayHeader)));
            nq::memlib::destroy_from_range(usr_ptr, array_ptr->count_);
        }

        nq::memlib::deallocate_log(usr_ptr,
                BaseDomain::sub_header_size + sizeof(ArrayHeader),
                nq::memlib::remove_header_operator_delete);
    }
}

#else // WITH_NQ_MEMOFF defined

template<class T>
void nqDelete(T *ptr)
{
    delete ptr;
}

template<class T,
    class Domain>
T* nqNewArray(const char* file, int line, size_t count)
{ // allocate a raw memory of size count
    return new T[count];
}

template<class T>
void nqDeleteArray(T *ptr)
{
    delete[] ptr;
}
# endif // !WITH_NQ_MEMOFF

#endif // !NQ_NEW_H_
