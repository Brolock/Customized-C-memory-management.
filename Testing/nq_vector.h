#ifndef NQ_VECTOR_H_
# define NQ_VECTOR_H_

# include <vector>

# include "nq_allocator.h"
# include "alloc_strat.h"
# include "domains.h"

namespace nq
{
	template<typename T,
		class Domain = UnknownDomain,
		class AllocStrat = DefaultAlloc>
	class vector : public std::vector<T, nq::allocator<T, Domain, AllocStrat>>
	{
        typedef nq::allocator<T, Domain, AllocStrat> nq_alloc;
        typedef std::vector<T, nq::allocator<T, Domain, AllocStrat>> parent;

        typedef T value_type;
        typedef size_t size_type;
	public:

        /*** Constructors ***/
		vector()
            : parent()
		{ // construct empty vector
        }

        explicit vector(const nq_alloc& alloc)
            : parent(alloc)
        { // construct empty vector, allocator
        }

		explicit vector(size_type count)
			: parent(count)
		{ // construct vector of size count
        }

        /* Should be available with the C++14 and replace vector(size_t)

        vector(size_t count, const nq_alloc& alloc = nq_alloc{})
            : parent(count, alloc)
        { // construct vector of size count
        }
        */

        vector(size_type count, const value_type& value)
            : parent(count, value)
        { // construct vector of size count copies of value
        }

        vector(size_type count, const value_type& value,
                const nq_alloc& alloc)
            : parent(count, value, alloc)
        { // construct vector of size count copies of value with alloc
        }

        template<class Iter>
        vector(Iter first, Iter end)
            : parent(first, end)
        { // construct vector from [first, last)
        }

        template<class Iter>
        vector(Iter first, Iter end, const nq_alloc& alloc)
            : parent(first, end, alloc)
        { // construct vector from [first, last) with allocator
        }

        /* Copy constructors */
        
        vector(const vector& other)
            : parent(other)
        { // construct by copying other
        }

        vector(const vector& other, const nq_alloc& alloc)
            : parent(other, alloc)
        { // construct by copying other with alloc
        }

        /* Move constructors */

        vector(vector&& other)
            : parent(std::move(other))
        { // construct by moving other
        }

        vector(vector&& other, const nq_alloc& alloc)
            : parent(other, alloc)
        { // construct by moving other with allocator
        }

        /* Initializer lise constructor */

        vector(std::initializer_list<value_type> Ilist,
                const nq_alloc& alloc = nq_alloc{})
            : parent(Ilist, alloc)
        { // construct vector from the initializer_list
        }

        /* Assignement operators */

        vector& operator=(const vector& other)
        { //assign vector from right
            this->parent::operator=(other);
            return *this;
        }

        vector& operator=(vector&& other)
        { // assign vector by moving other
            this->parent::operator=(std::move(other));
            return *this;
        }

        vector& operator=(std::initializer_list<value_type> Ilist)
        { // assign vector from ilist
            this->parent::operator=(Ilist);
            return *this;
        }
	};
}

#endif // !NQ_VECTOR_H_
