#ifndef NQ_LIST_H_
# define NQ_LIST_H_

# include <list>

# include "nq_allocator.h"
# include "alloc_strat.h"
# include "domains.h"

namespace nq
{
	template<typename T,
		class Domain = UnknownDomain,
		class AllocStrat = DefaultAlloc>
	class list : public std::list<T, nq::allocator<T, Domain, AllocStrat>>
	{
        typedef nq::allocator<T, Domain, AllocStrat> nq_alloc;
        typedef std::list<T, nq::allocator<T, Domain, AllocStrat>> parent;

        typedef T value_type;
        typedef size_t size_type;
	public:

        /*** Constructors ***/
		list()
            : parent()
		{ // construct empty list
        }

        explicit list(const nq_alloc& alloc)
            : parent(alloc)
        { // construct empty list, allocator
        }

		explicit list(size_type count)
			: parent(count)
		{ // construct list of size count
        }

        /* Should be available with the C++14 and replace list(size_t)

        list(size_t count, const nq_alloc& alloc = nq_alloc{})
            : parent(count, alloc)
        { // construct list of size count
        }
        */

        list(size_type count, const value_type& value)
            : parent(count, value)
        { // construct list of size count copies of value
        }

        list(size_type count, const value_type& value,
                const nq_alloc& alloc)
            : parent(count, value, alloc)
        { // construct list of size count copies of value with alloc
        }

        template<class Iter>
        list(Iter first, Iter end)
            : parent(first, end)
        { // construct list from [first, last)
        }

        template<class Iter>
        list(Iter first, Iter end, const nq_alloc& alloc)
            : parent(first, end, alloc)
        { // construct list from [first, last) with allocator
        }

        /* Copy constructors */
        
        list(const list& other)
            : parent(other)
        { // construct by copying other
        }

        list(const list& other, const nq_alloc& alloc)
            : parent(other, alloc)
        { // construct by copying other with alloc
        }

        /* Move constructors */

        list(list&& other)
            : parent(std::move(other))
        { // construct by moving other
        }

        list(list&& other, const nq_alloc& alloc)
            : parent(other, alloc)
        { // construct by moving other with allocator
        }

        /* Initializer lise constructor */

        list(std::initializer_list<value_type> Ilist,
                const nq_alloc& alloc = nq_alloc{})
            : parent(Ilist, alloc)
        { // construct list from the initializer_list
        }

        /* Assignement operators */

        list& operator=(const list& other)
        { //assign list from right
            this->parent::operator=(other);
            return *this;
        }

        list& operator=(list&& other)
        { // assign list by moving other
            this->parent::operator=(std::move(other));
            return *this;
        }

        list& operator=(std::initializer_list<value_type> Ilist)
        { // assign list from ilist
            this->parent::operator=(Ilist);
            return *this;
        }
	};
}

#endif // !NQ_LIST_H_
