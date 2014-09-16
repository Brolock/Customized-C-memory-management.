#ifndef NQ_UNIQUE_H_
# define NQ_UNIQUE_H_

#include <memory>

# include "nq_deleter.h"

namespace nq
{
	template<class T,
		class Deleter = nq::deleter<T>>
	class unique_ptr : public std::unique_ptr<T, Deleter>
	{
        typedef T element_type;
        typedef Deleter deleter_type;

		typedef std::unique_ptr<T, Deleter> parent;

        /*** Constructors ***/

		unique_ptr() noexcept
			: parent()
		{ // default construct
        }

        unique_ptr(std::nullptr_t ptr) noexcept
            : parent(ptr)
        { // construct unique_ptr with nullptr (equivalent to default)
        }

        explicit unique_ptr(T *ptr) noexcept
            : parent(ptr)
        { // construct unique_ptr with T*
        }
        
        unique_ptr(T *ptr,
                typename std::conditional<is_reference<deleter_type>::value,
                  deleter_type, const deleter_type&>::type del) noexcept
            : parent(ptr, del)
        {}

        unique_ptr(T *ptr,
                typename std::remove_reference<deleter_type>::type&& del) noexcept
            : parent(std::move(ptr), std::move(del))
        {}

        /*** Move constructors ***/
        
        unique_ptr(unique_ptr&& other) noexcept
            : parent(std::move(other))
        { // construct unique_ptr object that takes resource from other
        }

        template<class Y, class Del>
        unique_ptr(unique_ptr<T, Del>&& other) noexcept
            : parent(std::move(other))
        { // construct unique_ptr object that takes resource from other
        }

        /*** Assigment operator ***/
        
        unique_ptr& operator=(unique_ptr&& rhs) noexcept
        {
            this->parent::operator=(std::move(rhs();
            return *this;
        }

        template<class Y, class Del>
        unique_ptr& operator=(unique_ptr<Y, Del>&& rhs) noexcept
        {
            this->parent::operator=(std::move(rhs));
            return *this;
        }
        
        unique_ptr& operator=(std::nullptr_t) noexcept
        {
            this->parent::operator=(nullptr);
            return *this;
        }

	};

	template<class T,
		class Deleter = nq::deleter<T>>
	class unique_ptr<T[], Deleter> : public std::unique_ptr<T[], Deleter>
	{
	};
}

#endif // !NQ_UNIQUE_H_
