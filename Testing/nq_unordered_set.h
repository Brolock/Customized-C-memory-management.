#ifndef NQ_UNORDERED_SET_H_
# define NQ_UNORDERED_SET_H_

# include <unordered_set>
# include <cstdlib>

# include "nq_allocator.h"
# include "domains.h"
# include "alloc_strat.h"

namespace nq
{
    template<class Key,
        class Domain = UnknownDomain,
        class AllocStrat = DefaultAlloc,
        class Hash = std::hash<Key>,
        class KeyEqual = std::equal_to<Key>>
    class unordered_set
    : public std::unordered_set<Key, Hash, KeyEqual,
            nq::allocator<Key, Domain, AllocStrat>>
    {
        typedef nq::allocator<Key, Domain, AllocStrat> nq_alloc;
        typedef std::unordered_set<Key, Hash, KeyEqual, nq_alloc> parent;
        
    public:
        /*** Constructors ***/

        explicit unordered_set(std::size_t bucket_count = 10,
                const Hash hf = Hash(),
                const KeyEqual& keq = KeyEqual(),
                const nq_alloc& all = nq_alloc())
            : parent(bucket_count, hf, keq, all)
        { // construct empty u_set
        }

        explicit unordered_set(nq_alloc& all)
            : parent(all)
        { // construct empty u_set, nq_alloc
        }

        template<class Iterator>
        unordered_set(Iterator first, Iterator last,
                std::size_t buckets = 0,
                const Hash hf = Hash(),
                const KeyEqual& keq = KeyEqual(),
                const nq_alloc& all = nq_alloc())
            : parent(first, last, buckets, hf, keq, all)
        { // construct u_set from sequence [first, last)
        }


        /* Copy contructor */

        unordered_set(const unordered_set& other)
            : parent(other)
        { // copy contruct u_set from other
        }

        /* Move constructor */

        unordered_set(unordered_set&& other)
            : parent(std::move(other))
        { // move contruct u_set from other
        }

        /* Initializer constructor */

        unordered_set(std::initializer_list<Key> ilist,
                std::size_t bucket_count = 0,
                const Hash& hf = Hash(),
                const KeyEqual& keq = KeyEqual(),
                const nq_alloc& all = nq_alloc())
            : parent(ilist, bucket_count, hf, keq, all)
        { // construct u_set by copying elements in the list
        }

        /*** Assignement operator ***/

        unordered_set& operator=(const unordered_set& rhs)
        { // copy assignment operator
            this->parent::operator=(rhs);
            return *this;
        }

        unordered_set& operator=(unordered_set&& rhs)
        { // move assignment operator
            this->parent::operator=(std::move(rhs));
            return *this;
        }

        unordered_set& operator=(std::initializer_list<Key> ilist)
        { // assign initializer_list
            this->parent::operator=(ilist);
            return *this;
        }
    };
}

#endif // NQ_UNORDERED_SET_H_
