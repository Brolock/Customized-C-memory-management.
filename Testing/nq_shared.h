#ifndef NQ_SHARED_H_
# define NQ_SHARED_H_

# include <memory>
# include <type_traits>

# include "nq_allocator.h"
# include "nq_new.h"
# include "nq_deleter.h"
# include "domains.h"
# include "alloc_strat.h"

/* weak allocation ? where how ? weak ref count */

namespace nq
{
	template<class T>
	class shared_ptr : public std::shared_ptr<T>
	{
	public:
		/* Allocator used to allocate the ref_count of the shared_ptr */
		typedef nq::allocator<T, SharedPtrRefCountDomain, DefaultAlloc> count_alloc;
		typedef nq::deleter<T, UnknownDomain, DefaultAlloc> deleter;

		/*** Constructors for a nullptr ***/

		shared_ptr()
			: std::shared_ptr<T>(nullptr, deleter{}, count_alloc{})
		{ // construct with nullptr deleter{} and alloc{} 
		}

		shared_ptr(std::nullptr_t)
			:std::shared_ptr<T>(nullptr, deleter{}, count_alloc{})
		{ // construct with nullptr, deleter{}, count_alloc{}
		}

		template<class Deleter>
		shared_ptr(std::nullptr_t, Deleter del)
			: std::shared_ptr<T>(nullptr, del, count_alloc{})
		{ // construct with nullptr, del and count_alloc{}
		}

		template<class Deleter,
			class Allocator>
		shared_ptr(std::nullptr_t, Deleter del, Allocator alloc)
			: std::shared_ptr<T>(nullptr, del, alloc)
		{ // construct with nullptr, del and alloc
		}

		/*** Constructors for an already allocated ptr ***/

		template<class Y,
			class Deleter>
		shared_ptr(Y *ptr, Deleter del)
			: std::shared_ptr<T>(ptr, del, count_alloc{})
		{ // construct with ptr, del and count_alloc{}
		}

		template<class Y,
			class Deleter,
			class Allocator>
		shared_ptr(Y *ptr, Deleter del, Allocator alloc)
			: std::shared_ptr<T>(ptr, del, alloc)
		{ // construct with ptr, del and alloc
		}

		/*** Copy constructors ***/
		/*not quite sure I can construct a std::shared from a nq::shared, mb explicit conversion?*/

		template<class Y>
		shared_ptr(const shared_ptr<Y>& other, T *ptr)
			: std::shared_ptr<T>(other, ptr)
		{ // construct shared_ptr object that aliases ptr
		}

		shared_ptr(const shared_ptr& other)
			: std::shared_ptr<T>(other)
		{ // construct a shared_ptr object that owns same resource as other
		}

		template<class Y,
			class = class std::enable_if<std::is_convertible<Y *, T *>::value,
				void>::type>
		shared_ptr(const shared_ptr<Y>& other)
			: std::shared_ptr<T>(other)
		{ // construct a shared_ptr object that owns same resource as other
		}

		/*** Move contructors ***/

		shared_ptr(shared_ptr&& other)
			: std::shared_ptr<T>(std::move(other))
		{ // construct shared_ptr that takes resource from other
		}

		template<class Y,
			class = class std::enable_if<std::is_convertible<Y *, T *>::value,
				void>::type>
		shared_ptr(shared_ptr<Y>&& other)
			: std::shared_ptr<T>(std::move(other))
		{ // construct shared_ptr that takes resource from other
		}

			/* TODO FIXME change std::weak_ptr with nq::weak_ptr when it'll be handled */
		template <class Y>
		explicit shared_ptr(const std::weak_ptr<Y>& other)
			: std::shared_ptr<T>(other)
		{ // construct shared_ptr object that owns resource *other
		}

			/* TODO FIXME change std::auto_ptr with nq::auto_ptr when it'll be handled */
		template <class Y>
		shared_ptr(std::auto_ptr<Y>&& other)
			: std::shared_ptr<T>(std::move(other))
		{ // construct shared_ptr object that owns *other.get()
		}

			/* TODO FIXME change std::unique_ptr with nq::unique_ptr when it'll be handled */
		template <class Y,
			class Del>
		shared_ptr(std::unique_ptr<Y, Del>&& other)
			: std::shared_ptr<T>(std::move(other))
		{ // construct from unique_ptr
		}

	private:
		/*
		** We don't want to take the risk that an unsuspecting user write somthing like:
		** nq::shared<T>(new (Domain, AllocStrat) T());
		** The pointer allocated in Domain with AllocStrat will be, by default,
		** deleted in Unknown Domain with DefaultAlloc
		*/
		template<class Y>
		explicit shared_ptr(Y *ptr)
			: std::shared_ptr<T>(ptr, deleter{}, count_alloc{})
		{ // construct with ptr deleter{} and count_alloc{}
		}
	};

	/*** Non member functions ***/

	template<class T,
		class Domain = UnknownDomain,
		class AllocStrat = DefaultAlloc,
		class... Args>
	shared_ptr<T> new_shared(Args&&... args)
	{ // make a shared_ptr with two allocation (as the shared_ptr constructor do)
		typedef nq::allocator<T, SharedPtrRefCountDomain, DefaultAlloc> count_alloc;
		typedef nq::deleter<T, Domain, AllocStrat> deleter;

		return shared_ptr<T>(nq::New<T, Domain, AllocStrat>, deleter{}, count_alloc{});
	}

	template<class T,
		class Domain = UnknownDomain,
		class AllocStrat = DefaultAlloc,
		class... Args>
	shared_ptr<T> make_shared(Args&&... args)
	{ // make a shared_ptr with a single allocation
		typedef nq::allocator<T, Domain, AllocStrat> alloc;

		return std::allocate_shared<T>(alloc{}, std::forward<Args>(args)...);
	}

	template<class T,
		class Allocator,
		class... Args>
	shared_ptr<T> allocate_shared(const Allocator& alloc, Args&&... args)
	{ // make a shared_ptr with a single allocation
		return std::allocate_shared<T>(alloc, std::forward<Args>(args)...);
	}

	template<class T,
		class U>
	shared_ptr<T> static_pointer_cast(const shared_ptr<U>& other)
	{	// construct shared_ptr object for static_pointer_cast
		return std::static_pointer_cast(other);
	}

	template<class T,
		class U>
	shared_ptr<T> dynamic_pointer_cast(const shared_ptr<U>& other)
	{	// construct shared_ptr object for dynamic_pointer_cast
		return std::dynamic_pointer_cast(other);
	}

	template<class T,
		class U>
	shared_ptr<T> const_pointer_cast(const shared_ptr<U>& other)
	{	// construct shared_ptr object for const_pointer_cast
		return std::const_pointer_cast(other);
	}

	template<class Deleter,
		class T>
	Deleter* get_deleter(const shared_ptr<T>& p)
	{
		return std::get_deleter(p);
	}

	/* Comparison operators */

	template<class T,
		class U>
	bool operator==(const shared_ptr<T>& left, const shared_ptr<U>& right)
	{ // test if shared_ptr left == right
		return left.get() == right.get();
	}

	template<class T,
		class U>
	bool operator!=(const shared_ptr<T>& left, const shared_ptr<U>& right)
	{ // test if shared_ptr left != right
		return !(left.get() == right.get());
	}

	template<class T,
		class U>
	bool operator<(const shared_ptr<T>& left, const shared_ptr<U>& right)
	{ // test if shared_ptr left < right
		return std::operator<(left, right);
	}

	template<class T,
		class U>
	bool operator<=(const shared_ptr<T>& left, const shared_ptr<U>& right)
	{ // test if shared_ptr left <= right
		return std::operator<=(left, right);
	}
	template<class T,
		class U>
	bool operator>(const shared_ptr<T>& left, const shared_ptr<U>& right)
	{ // test if shared_ptr left > right
		return std::operator>(left, right);
	}
	template<class T,
		class U>
	bool operator>=(const shared_ptr<T>& left, const shared_ptr<U>& right)
	{ // test if shared_ptr left >= right
		return std::operator>=(left, right);
	}
}

#endif // !NQ_SHARED_H_
