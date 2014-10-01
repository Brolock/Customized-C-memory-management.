#ifndef NQ_SHARED_H_
# define NQ_SHARED_H_

# include <memory>
# include <type_traits>

# include "nq_allocator.h"
# include "nq_deleter.h"
# include "domains.h"
# include "alloc_strat.h"
# include "nq_unique.h"

# include "nq_memlib_new.h"

namespace nq
{
    template<class T>
    class weak_ptr;

	template<class T>
	class shared_ptr : public std::shared_ptr<T>
	{
	public:
		/* Allocator used to allocate the ref_count of the shared_ptr */
		typedef nq::allocator<T, SharedPtrRefCountDomain, DefaultAlloc> count_alloc;
		typedef nq::deleter<T, UnknownDomain, DefaultAlloc> nq_deleter;
        typedef std::shared_ptr<T> parent;

		/*** Constructors for a nullptr ***/

		shared_ptr() noexcept
			: parent(nullptr, nq_deleter{}, count_alloc{})
		{ // construct with nullptr nq_deleter{} and alloc{} 
		}

		shared_ptr(std::nullptr_t) noexcept
			:parent(nullptr, nq_deleter{}, count_alloc{})
		{ // construct with nullptr, nq_deleter{}, count_alloc{}
		}

		template<class nq_deleter>
		shared_ptr(std::nullptr_t, nq_deleter del) noexcept
			: parent(nullptr, del, count_alloc{})
		{ // construct with nullptr, del and count_alloc{}
		}

		template<class nq_deleter,
			class Allocator>
		shared_ptr(std::nullptr_t, nq_deleter del, Allocator alloc) noexcept
			: parent(nullptr, del, alloc)
		{ // construct with nullptr, del and alloc
		}

		/*** Constructors for an already allocated ptr ***/

		template<class Y,
			class nq_deleter>
		shared_ptr(Y *ptr, nq_deleter del) noexcept
			: parent(ptr, del, count_alloc{})
		{ // construct with ptr, del and count_alloc{}
		}

		template<class Y,
			class nq_deleter,
			class Allocator>
		shared_ptr(Y *ptr, nq_deleter del, Allocator alloc) noexcept
			: parent(ptr, del, alloc)
		{ // construct with ptr, del and alloc
		}

		/*** Copy constructors ***/

		template<class Y>
		shared_ptr(const shared_ptr<Y>& other, T *ptr) noexcept
			: parent(other, ptr)
		{ // construct shared_ptr object that aliases ptr
		}

		shared_ptr(const shared_ptr& other) noexcept
			: parent(other)
		{ // construct a shared_ptr object that owns same resource as other
		}

		template<class Y,
			class = class std::enable_if<std::is_convertible<Y*, T*>::value,
				void>::type>
		shared_ptr(const shared_ptr<Y>& other) noexcept
			: parent(other)
		{ // construct a shared_ptr object that owns same resource as other
		}

		/*** Move contructors ***/

		shared_ptr(shared_ptr&& other) noexcept
			: parent(std::move(other))
		{ // construct shared_ptr that takes resource from other
		}

		template<class Y,
			class = class std::enable_if<std::is_convertible<Y*, T*>::value,
				void>::type>
		shared_ptr(shared_ptr<Y>&& other) noexcept
			: parent(std::move(other))
		{ // construct shared_ptr that takes resource from other
		}

		template <class Y>
		explicit shared_ptr(const nq::weak_ptr<Y>& other) noexcept
			: parent(other)
		{ // construct shared_ptr object that owns resource *other
		}

		template <class Y,
			class Y_Domain,
            class Y_AllocStrat>
		shared_ptr(nq::unique_ptr<Y, Y_Domain, Y_AllocStrat>&& other) noexcept
			: parent(std::move(other))
		{ // construct from unique_ptr
		}

        /*** Assignement operator ***/

        shared_ptr& operator=(const shared_ptr& r) noexcept = default;

        template<class Y>
        shared_ptr& operator=(const shared_ptr<Y>& r) noexcept
        { // copy assignement from shared_ptr<Y>
            this->parent::operator=(r);
            return *this;
        }

        shared_ptr& operator=(shared_ptr&& r) noexcept
        { // move assignement from shared_ptr
            this->parent::operator=(std::move(r));
            return *this;
        }

        template<class Y>
        shared_ptr& operator=(shared_ptr<Y>&& r) noexcept
        { // move assignement from shared_ptr<Y>
            this->parent::operator=(std::move(r));
            return *this;
        }

        template<class Y,
			class Y_Domain,
            class Y_AllocStrat>
        shared_ptr& operator=(unique_ptr<Y, Y_Domain, Y_AllocStrat>&& other)
        { // move assignement from unique_ptr
            this->parent::operator=(std::move(other));
            return *this;
        }

        //template<class Y,
    public:
        /* TODO Check if this is really usefull */
        // convert to std from nq
        shared_ptr(std::shared_ptr<T>&& other) noexcept
            : parent(std::move(other))
        {}

        //convert to nq from str
        operator std::shared_ptr<T>()
        {
            return *this;
        }
	private:
		/*
		** We don't want to take the risk that a miss informed user write
        ** something like:
		** nq::shared<T>(new (Domain, AllocStrat) T());
		** The pointer allocated in Domain with AllocStrat will be, by default,
		** deleted in Unknown Domain with DefaultAlloc
		*/
		template<class Y>
		explicit shared_ptr(Y *ptr) = delete;
	};

	/*** Non member functions ***/

    template<class T,
		class Domain = UnknownDomain,
		class AllocStrat = DefaultAlloc,
		class... Args>
	shared_ptr<T> new_shared(Args&&... args)
	{ // make a shared_ptr with two allocation (as the shared_ptr constructor do)
		typedef nq::allocator<
            T, SharedPtrRefCountDomain, DefaultAlloc> count_alloc;

		typedef nq::deleter<T, Domain, AllocStrat> nq_deleter;

		return shared_ptr<T>(nq::memlib::New<T, Domain, AllocStrat>
                (std::forward<Args>(args)...), nq_deleter(), count_alloc());
	}

	template<class T,
		class Domain = UnknownDomain,
		class AllocStrat = DefaultAlloc,
		class... Args>
	shared_ptr<T> make_shared(Args&&... args)
	{ // make a shared_ptr with a single allocation
		typedef nq::allocator<T, Domain, AllocStrat> alloc;

		return std::allocate_shared<T>(alloc(), std::forward<Args>(args)...);
	}

	template<class T,
		class Allocator,
		class... Args>
	shared_ptr<T> allocate_shared(const Allocator& alloc, Args&&... args)
	{ // make a shared_ptr with a single allocation
		return std::allocate_shared<T>(alloc, std::forward<Args>(args)...);
	}

    template<class T>
    class weak_ptr : public std::weak_ptr<T>
    {
        typedef std::weak_ptr<T> parent;

    public:

        /*** Constructors ***/

        weak_ptr() noexcept
            : parent()
        { // Construct empty weak_ptr
        }

        /*  Copy constructors */

        weak_ptr(const weak_ptr& other) noexcept
            : parent(other)
        { // Construct weak_ptr for the ressource pointed to by other
        }

        template<class Y>
        weak_ptr(const weak_ptr& other) noexcept
            : parent(other)
        { // Construct weak_ptr for the ressource pointed to by other
        }

        template<class Y>
        weak_ptr(const shared_ptr<Y>& shared_other) noexcept
            : parent(shared_other)
        { // Construct weak_ptr for the ressource owned by shared_other
        }

        /*** Assignement operators ***/

        weak_ptr& operator=(const weak_ptr& rhs)
        { // Assign from rhs
            this->parent::operator=(rhs);
            return *this;
        }

        template<class Y>
        weak_ptr& operator=(const weak_ptr<Y>& rhs)
        { // Assign from rhs
            this->parent::operator=(rhs);
            return *this;
        }

        template<class Y>
        weak_ptr& operator=(const shared_ptr<T>& rhs)
        { // Assign from rhs
            this->parent::operator=(rhs);
            return *this;
        }

        /* Should be available with the C++14 just uncomment

           Move Contructors
        
        weak_ptr(weak_ptr&& r) noexcept
         : parent(std::move(r))
        { // Construct weak_ptr by taking the ressource pointer to by other
        }

        template<class Y>
        weak_ptr(weak_ptr<Y>&& r) noexcept
            : parent(std::move(r))
        { // Construct weak_ptr by taking the ressource pointer to by other
        }

            Move Assignements

        weak_ptr& operator=(weak_ptr&& rhs)
        { // Assign from rhs
            this->parent::operator=(std::move(rhs));
            return *this;
        }

        template<class Y>
        weak_ptr& operator=(weak_ptr<Y>&& rhs)
        { // Assign from rhs
            this->parent::operator=(std::move(rhs));
            return *this;
        }
        */
    };
}

// Provide partial specialization of std fonctions for nq::shared_ptr
namespace std
{
	#ifdef NQ_GNU_
    /* have to copy paste the stl to make this works... */
    template<typename T>
    struct owner_less<nq::shared_ptr<T>>
    : public _Sp_owner_less<shared_ptr<T>, weak_ptr<T>>
    { };

    template<typename T>
    struct owner_less<nq::weak_ptr<T>>
    : public _Sp_owner_less<weak_ptr<T>, shared_ptr<T>>
    { };
	#endif //NQ_GNU_
}

#endif // !NQ_SHARED_H_
