#ifndef NQ_SHARED_H_
# define NQ_SHARED_H_

# include <memory>
# include <type_traits>

# include "nq_allocator.h"
# include "nq_deleter.h"
# include "domains.h"
# include "alloc_strat.h"

    /* weak allocation ? where how ? weak ref count */

    namespace nq
    {
        template<typename T,
            typename Domain = UnknownDomain,
            class AllocStrat = DefaultAlloc>
        class shared_ptr : public std::shared_ptr<T>
        {
        public:
            /* Allocator used to allocate the ref_count of the shared_ptr */
            typedef nq::allocator<T, SharedPtrRefCountDomain, AllocStrat> count_alloc;
            typedef nq::deleter<T, Domain, AllocStrat> deleter;

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

            template<typename Y>
            explicit shared_ptr(Y *ptr)
                : std::shared_ptr<T>(ptr, deleter{}, count_alloc{})
            { // construct with ptr deleter{} and count_alloc{}
            }

            template<typename Y,
                class Deleter>
            shared_ptr(Y *ptr, Deleter del)
                : std::shared_ptr<T>(ptr, del, count_alloc{})
            { // construct with ptr, del and count_alloc{}
            }

            template<typename Y,
                 class Deleter,
                 class Allocator>
            shared_ptr(Y *ptr, Deleter del, Allocator alloc)
                : std::shared_ptr<T>(ptr, del, alloc)
            { // construct with ptr, del and alloc
            }

            /*** Copy constructors ***/
                 /*not quite sure I can construct a std::shared from a nq::shared, mb explicit conversion?*/

            template<typename Y>
            shared_ptr(const shared_ptr<Y, Domain, AllocStrat>& other, T *ptr)
                : std::shared_ptr<T>(other, ptr)
		{ // construct shared_ptr object that aliases ptr
		}

		shared_ptr(const shared_ptr& other)
			: std::shared_ptr<T>(other)
		{ // construct a shared_ptr object that owns same resource as other
		}

		template<typename Y,
			class = typename std::enable_if<std::is_convertible<Y *, T *>::value, void>::type>
		shared_ptr(const shared_ptr<Y, Domain, AllocStrat>& other)
			: std::shared_ptr<T>(other)
		{ // construct a shared_ptr object that owns same resource as other
		}

		/*** Move contructors ***/
		
		shared_ptr(shared_ptr&& other)
			: std::shared_ptr<T>(std::move(other))
		{ // construct shared_ptr that takes resource from other
		}

		template<typename Y,
			class = typename std::enable_if<std::is_convertible<Y *, T *>::value, void>::type>
		shared_ptr(shared_ptr<Y, Domain, AllocStrat>&& other)
			: std::shared_ptr<T>(std::move(other))
		{ // construct shared_ptr that takes resource from other
		}
	};

	/* Non member functions */
	template<class T,
		typename Domain = UnknownDomain,
		class AllocStrat = DefaultAlloc,
		class... Args>
	shared_ptr<T, Domain, AllocStrat> make_shared(Args&&... args)
	{
		typedef nq::allocator<T, Domain, AllocStrat> alloc;
		return std::allocate_shared<T>(alloc{}, std::forward<Args>(args)...);
	}
}

#endif // !NQ_SHARED_H_
