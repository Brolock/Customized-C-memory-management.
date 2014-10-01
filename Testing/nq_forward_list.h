#ifndef NQ_FORWARD_LIST_H_
# define NQ_FORWARD_LIST_H_

# include <forward_list>

# include "nq_allocator.h"
# include "alloc_strat.h"
# include "domains.h"

namespace nq
{
	template<typename T,
		class Domain = UnknownDomain,
		class AllocStrat = DefaultAlloc>
	class forward_list
    : public std::forward_list<T, nq::allocator<T, Domain, AllocStrat>>
	{
        typedef nq::allocator<T, Domain, AllocStrat> nq_alloc;
        typedef std::forward_list<T, nq::allocator<T, Domain, AllocStrat>> parent;

        typedef T value_type;
        typedef size_t size_type;
	public:

        /*** Constructors ***/
		forward_list()
            : parent()
		{ // construct empty forward_list
        }

        explicit forward_list(const nq_alloc& alloc)
            : parent(alloc)
        { // construct empty forward_list, allocator
        }

		explicit forward_list(size_type count)
			: parent(count)
		{ // construct forward_list of size count
        }

        /* Should be available with the C++14 and replace forward_list(size_t)

        forward_list(size_t count, const nq_alloc& alloc = nq_alloc{})
            : parent(count, alloc)
        { // construct forward_list of size count
        }
        */

        forward_list(size_type count, const value_type& value)
            : parent(count, value)
        { // construct forward_list of size count copies of value
        }

        forward_list(size_type count, const value_type& value,
                const nq_alloc& alloc)
            : parent(count, value, alloc)
        { // construct forward_list of size count copies of value with alloc
        }

        template<class Iter>
        forward_list(Iter first, Iter end)
            : parent(first, end)
        { // construct forward_list from [first, last)
        }

        template<class Iter>
        forward_list(Iter first, Iter end, const nq_alloc& alloc)
            : parent(first, end, alloc)
        { // construct forward_list from [first, last) with allocator
        }

        /* Copy constructors */
        
        forward_list(const forward_list& other)
            : parent(other)
        { // construct by copying other
        }

        forward_list(const forward_list& other, const nq_alloc& alloc)
            : parent(other, alloc)
        { // construct by copying other with alloc
        }

        /* Move constructors */

        forward_list(forward_list&& other)
            : parent(std::move(other))
        { // construct by moving other
        }

        forward_list(forward_list&& other, const nq_alloc& alloc)
            : parent(other, alloc)
        { // construct by moving other with allocator
        }

        /* Initializer lise constructor */

        forward_list(std::initializer_list<value_type> Ilist,
                const nq_alloc& alloc = nq_alloc{})
            : parent(Ilist, alloc)
        { // construct forward_list from the initializer_list
        }

        /* Assignement operators */

        forward_list& operator=(const forward_list& other)
        { //assign forward_list from right
            this->parent::operator=(other);
            return *this;
        }

        forward_list& operator=(forward_list&& other)
        { // assign forward_list by moving other
            this->parent::operator=(std::move(other));
            return *this;
        }

        forward_list& operator=(std::initializer_list<value_type> Ilist)
        { // assign forward_list from ilist
            this->parent::operator=(Ilist);
            return *this;
        }
	};
}

#endif // !NQ_FORWARD_LIST_H_
