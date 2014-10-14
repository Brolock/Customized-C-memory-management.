#ifndef NQ_DEQUE_H_
# define NQ_DEQUE_H_

# include <deque>

# include "nq_allocator.h"
# include "alloc_strat.h"
# include "domains.h"

namespace nq
{
    template<typename T,
        class Domain = UnknownDomain,
        class AllocStrat = DefaultAlloc>
    class deque : public std::deque<T, nq::allocator<T, Domain, AllocStrat>>
    {
        typedef nq::allocator<T, Domain, AllocStrat> nq_alloc;
        typedef std::deque<T, nq::allocator<T, Domain, AllocStrat>> parent;

        typedef T value_type;
        typedef size_t size_type;
    public:

        /*** Constructors ***/
        deque()
            : parent()
        { // construct empty deque
        }

        explicit deque(const nq_alloc& alloc)
            : parent(alloc)
        { // construct empty deque, allocator
        }

        explicit deque(size_type count)
            : parent(count)
        { // construct deque of size count
        }

        /* Should be available with the C++14 and replace deque(size_t)

        deque(size_t count, const nq_alloc& alloc = nq_alloc{})
            : parent(count, alloc)
        { // construct deque of size count
        }
        */

        deque(size_type count, const value_type& value)
            : parent(count, value)
        { // construct deque of size count copies of value
        }

        deque(size_type count, const value_type& value,
                const nq_alloc& alloc)
            : parent(count, value, alloc)
        { // construct deque of size count copies of value with alloc
        }

        template<class Iter>
        deque(Iter first, Iter end)
            : parent(first, end)
        { // construct deque from [first, last)
        }

        template<class Iter>
        deque(Iter first, Iter end, const nq_alloc& alloc)
            : parent(first, end, alloc)
        { // construct deque from [first, last) with allocator
        }

        /* Copy constructors */
        
        deque(const deque& other)
            : parent(other)
        { // construct by copying other
        }

        deque(const deque& other, const nq_alloc& alloc)
            : parent(other, alloc)
        { // construct by copying other with alloc
        }

        /* Move constructors */

        deque(deque&& other)
            : parent(std::move(other))
        { // construct by moving other
        }

        deque(deque&& other, const nq_alloc& alloc)
            : parent(other, alloc)
        { // construct by moving other with allocator
        }

        /* Initializer lise constructor */

        deque(std::initializer_list<value_type> Ilist,
                const nq_alloc& alloc = nq_alloc{})
            : parent(Ilist, alloc)
        { // construct deque from the initializer_list
        }

        /* Assignement operators */

        deque& operator=(const deque& other)
        { //assign deque from right
            this->parent::operator=(other);
            return *this;
        }

        deque& operator=(deque&& other)
        { // assign deque by moving other
            this->parent::operator=(std::move(other));
            return *this;
        }

        deque& operator=(std::initializer_list<value_type> Ilist)
        { // assign deque from ilist
            this->parent::operator=(Ilist);
            return *this;
        }
    };
}

#endif // !NQ_DEQUE_H_
