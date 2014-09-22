#ifndef NQ_UNORDERED_MULTISET_H_
# define NQ_UNORDERED_MULTISET_H_

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
    class unordered_multiset
    : public std::unordered_multiset<Key, Hash, KeyEqual,
            nq::allocator<Key, Domain, AllocStrat>>
    {
        typedef nq::allocator<Key, Domain, AllocStrat> nq_alloc;
        typedef std::unordered_multiset<Key, Hash, KeyEqual, nq_alloc> parent;

        typedef typename parent::value_type value_type;
        typedef typename parent::size_type size_type;
        typedef typename parent::hasher hasher;
        typedef typename parent::key_equal key_equal;
        
    public:
        /*** Constructors ***/

        explicit unordered_multiset(size_type bucket_count = 10,
                const hasher hf = hasher(),
                const key_equal& keq = key_equal(),
                const nq_alloc& all = nq_alloc())
            : parent(bucket_count, hf, keq, all)
        { // construct empty u_set
        }

        explicit unordered_multiset(nq_alloc& all)
            : parent(all)
        { // construct empty u_set, nq_alloc
        }

        template<class Iterator>
        unordered_multiset(Iterator first, Iterator last,
                size_type buckets = 0,
                const hasher hf = hasher(),
                const key_equal& keq = key_equal(),
                const nq_alloc& all = nq_alloc())
            : parent(first, last, buckets, hf, keq, all)
        { // construct u_set from sequence [first, last)
        }


        /* Copy contructor */

        unordered_multiset(const unordered_multiset& other)
            : parent(other)
        { // copy contruct u_set from other
        }

        /* Move constructor */

        unordered_multiset(unordered_multiset&& other)
            : parent(std::move(other))
        { // move contruct u_set from other
        }

        /* Initializer constructor */

        unordered_multiset(std::initializer_list<value_type> ilist,
                size_type bucket_count = 0,
                const hasher& hf = hasher(),
                const key_equal& keq = key_equal(),
                const nq_alloc& all = nq_alloc())
            : parent(ilist, bucket_count, hf, keq, all)
        { // construct u_set by copying elements in the list
        }

        /*** Assignement operator ***/

        unordered_multiset& operator=(const unordered_multiset& rhs)
        { // copy assignment operator
            this->parent::operator=(rhs);
            return *this;
        }

        unordered_multiset& operator=(unordered_multiset&& rhs)
        { // move assignment operator
            this->parent::operator=(std::move(rhs));
            return *this;
        }

        unordered_multiset& operator=(std::initializer_list<value_type> ilist)
        { // assign initializer_list
            this->parent::operator=(ilist);
            return *this;
        }
    };
}

#endif // NQ_UNORDERED_MULTISET_H_
