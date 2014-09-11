#ifndef NQ_SHARED_H_
# define NQ_SHARED_H_

# include <memory>
# include <atomic>
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

    public:
        shared_ptr(std::shared_ptr<T>&& other)
            : std::shared_ptr<T>(other)
        {}
	private:
		/*
		** We don't want to take the risk that a misinformed user write somthing like:
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
		typedef nq::allocator<T, SharedPtrRefCountDomain, DefaultAlloc> count_alloc;
		typedef nq::deleter<T, Domain, AllocStrat> deleter;

		return shared_ptr<T>(nq::New<T, Domain, AllocStrat>(std::forward<Args>(args)...), deleter{}, count_alloc{});
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
		return std::operator==(left, right);
	}

	template<class T,
		class U>
	bool operator!=(const shared_ptr<T>& left, const shared_ptr<U>& right)
	{ // test if shared_ptr left != right
		return std::operator!=(left, right);
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

	/* Comparison operators with nullptr */

	template<class T,
		class U>
	bool operator==(const shared_ptr<T>& left, std::nullptr_t right)
	{ // test if shared_ptr left == nullptr
		return std::operator==(left, right);
	}

	template<class T,
		class U>
	bool operator==(std::nullptr_t left, const shared_ptr<U>& right)
	{ // test if nullptr == right
		return std::operator==(left, right);
	}

	template<class T,
		class U>
	bool operator!=(const shared_ptr<T>& left, std::nullptr_t right)
	{ // test if shared_ptr left != nullptr
		return std::operator!=(left, right);
	}

	template<class T,
		class U>
	bool operator!=(std::nullptr_t left, const shared_ptr<U>& right)
	{ // test if nullptr != right
		return std::operator!=(left, right);
	}

	template<class T,
		class U>
	bool operator<(const shared_ptr<T>& left, std::nullptr_t right)
	{ // test if shared_ptr left < nullptr
		return std::operator<(left, right);
	}

	template<class T,
		class U>
	bool operator<(std::nullptr_t left, const shared_ptr<U>& right)
	{ // test if nullptr < right
		return std::operator<(left, right);
	}

	template<class T,
		class U>
	bool operator<=(const shared_ptr<T>& left, std::nullptr_t right)
	{ // test if shared_ptr left <= nullptr
		return std::operator<=(left, right);
	}

	template<class T,
		class U>
	bool operator<=(std::nullptr_t left, const shared_ptr<U>& right)
	{ // test if nullptr <= right
		return std::operator<=(left, right);
	}

	template<class T,
		class U>
	bool operator>(const shared_ptr<T>& left, std::nullptr_t right)
	{ // test if shared_ptr left > nullptr
		return std::operator>(left, right);
	}

	template<class T,
		class U>
	bool operator>(std::nullptr_t left, const shared_ptr<U>& right)
	{ // test if nullptr > right
		return std::operator>(left, right);
	}

	template<class T,
		class U>
	bool operator>=(const shared_ptr<T>& left, std::nullptr_t right)
	{ // test if shared_ptr left >= nullptr
		return std::operator>=(left, right);
	}

	template<class T,
		class U>
	bool operator>=(std::nullptr_t left, const shared_ptr<U>& right)
	{ // test if nullptr >= right
		return std::operator>=(left, right);
	}

	template<class Elem,
		class Traits,
		class T>
	std::basic_ostream<Elem, Traits>&
	operator<<(std::basic_ostream<Elem, Traits>& os, const shared_ptr<T>& ptr)
	{ // write the contained pointer to stream
		return std::operator<<(os, ptr);
	}

	template<class T>
	void swap(shared_ptr<T>& left, shared_ptr<T>& right)
	{ // swap left and right shared_ptrs
		return std::swap(left, right);
	}

	template<class T> inline
	bool atomic_is_lock_free(std::shared_ptr<T> *p)
	{ // return true if atomic operation on shared_ptr<T> are lock-free
		return std::atomic_is_lock_free(p);
	}

	template<class T> inline
	shared_ptr<T> atomic_load(const shared_ptr<T> *ptr)
	{ // load *ptr atomically
		return std::atomic_load(ptr);
	}

	template<class T> inline
	shared_ptr<T> atomic_load_explicit(const shared_ptr<T> *ptr, std::memory_order mem_fwd)
	{ // load *ptr atomically
		return std::atomic_load_explicit(ptr, mem_fwd);
	}

	template<class T> inline
	void atomic_store(shared_ptr<T> *ptr, shared_ptr<T> other)
	{	// store other to *ptr atomically
		std::atomic_store(ptr, other);
	}

	template<class T> inline
	void atomic_store_explicit(shared_ptr<T> *ptr, shared_ptr<T> other,
		std::memory_order mem_fwd)
	{	// store other to *ptr atomically
		std::atomic_store_explicit(ptr, other, mem_fwd);
	}

	template<class T> inline
	shared_ptr<T> atomic_exchange(shared_ptr<T> *ptr, shared_ptr<T> other)
	{ // copy other to *ptr and return previous value of *ptr atomically
		return std::atomic_exchange(ptr, other);
	}

	template<class T> inline
	shared_ptr<T> atomic_exchange_explicit(shared_ptr<T> *ptr, shared_ptr<T> other,
		std::memory_order mem_fwd)
	{ // copy other to *ptr and return previous value of *ptr atomically
		return std::atomic_exchange_explicit(ptr, other, mem_fwd);
	}

	template<class T> inline
	bool atomic_compare_exchange_weak(shared_ptr<T> *ptr, shared_ptr<T> *exp,
		shared_ptr<T> value)
	{ // atomically compare and exchange
		return std::atomic_compare_exchange_weak(ptr, exp, value);
	}

	template<class T> inline
	bool atomic_compare_exchange_weak_explicit(shared_ptr<T> *ptr, shared_ptr<T> *exp,
		shared_ptr<T> value, std::memory_order mem_fwd1, std::memory_order mem_fwd2)
	{ // atomically compare and exchange
		return std::atomic_compare_exchange_weak_explicit(ptr, exp, value, mem_fwd1, mem_fwd2);
	}

	template<class T> inline
	bool atomic_compare_exchange_strong(shared_ptr<T> *ptr, shared_ptr<T> *exp,
		shared_ptr<T> value)
	{ // atomically compare and exchange
		return std::atomic_compare_exchange_strong(ptr, exp, value);
	}

	template<class T> inline
	bool atomic_compare_exchange_strong_explicit(shared_ptr<T> *ptr, shared_ptr<T> *exp,
		shared_ptr<T> value, std::memory_order mem_fwd1, std::memory_order mem_fwd2)
	{ // atomically compare and exchange
		return std::atomic_compare_exchange_strong(ptr, exp, value, mem_fwd1, mem_fwd2);
	}
}

#endif // !NQ_SHARED_H_
