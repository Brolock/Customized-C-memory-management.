#ifndef NQ_UNORDERED_SET_H_
# define NQ_UNORDERED_SET_H_

# include <unordered_set>

# include "nq_allocator.h"
# include "lib_domains.h"
# include "alloc_strat.h"

namespace nq
{
    template<class Key,
        class Domain = UnknownDomain,
        class AllocStrat = DefaultAlloc,
        class Hash = std::hash<Key>,
        class KeyEqual= std::equal_to<Key>>
    class unordered_set
    : public std::unordered_set<Key, Hash, KeyEqual,
            nq::allocator<Key, Domain, AllocStrat>>
    {
        typedef nq::allocator<Key, Domain, AllocStrat> nq_alloc;
        typedef std::unordered_set<Key, Hash, KeyEqual, nq_alloc> parent;
        
        typedef typename parent::value_type value_type;
        typedef typename parent::size_type size_type;
        typedef typename parent::hasher hasher;
        typedef typename parent::key_equal key_equal;
    public:
        /*** Constructors ***/

        explicit unordered_set(size_type bucket_count = 10,
                const hasher hf = hasher(),
                const key_equal& keq = key_equal(),
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
                size_type buckets = 0,
                const hasher hf = hasher(),
                const key_equal& keq = key_equal(),
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

        unordered_set(std::initializer_list<value_type> ilist,
                size_type bucket_count = 0,
                const hasher& hf = hasher(),
                const key_equal& keq = key_equal(),
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

        unordered_set& operator=(std::initializer_list<value_type> ilist)
        { // assign initializer_list
            this->parent::operator=(ilist);
            return *this;
        }
    };
}

#endif // NQ_UNORDERED_SET_H_
