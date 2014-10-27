#ifndef NQ_UNORDERED_MULTIMAP_H_
# define NQ_UNORDERED_MULTIMAP_H_

# include <unordered_map>

# include "nq_allocator.h"
# include "lib_domains.h"
# include "alloc_strat.h"

namespace nq
{
    template<class Key,
        class T,
        class Domain = UnknownDomain,
        class AllocStrat = DefaultAlloc,
        class Hash = std::hash<Key>,
        class KeyEqual = std::equal_to<Key>>
    class unordered_multimap
    : public std::unordered_multimap<Key, T, Hash, KeyEqual,
            nq::allocator<std::pair<Key, T>, Domain, AllocStrat>>
    {
        typedef nq::allocator<std::pair<Key, T>, Domain, AllocStrat> nq_alloc;
        typedef std::unordered_multimap<Key, T, Hash, KeyEqual, nq_alloc> parent;

        typedef typename parent::value_type value_type;
        typedef typename parent::size_type size_type;
        typedef typename parent::hasher hasher;
        typedef typename parent::key_equal key_equal;
        
    public:
        /*** Constructors ***/

        explicit unordered_multimap(size_type bucket_count = 10,
                const hasher hf = hasher(),
                const key_equal& keq = key_equal(),
                const nq_alloc& all = nq_alloc())
            : parent(bucket_count, hf, keq, all)
        { // construct empty u_multimap
        }

        explicit unordered_multimap(nq_alloc& all)
            : parent(all)
        { // construct empty u_multimap, nq_alloc
        }

        template<class Iterator>
        unordered_multimap(Iterator first, Iterator last,
                size_type buckets = 0,
                const hasher hf = hasher(),
                const key_equal& keq = key_equal(),
                const nq_alloc& all = nq_alloc())
            : parent(first, last, buckets, hf, keq, all)
        { // construct u_multimap from sequence [first, last)
        }


        /* Copy contructor */

        unordered_multimap(const unordered_multimap& other)
            : parent(other)
        { // copy contruct u_multimap from other
        }

        /* Move constructor */

        unordered_multimap(unordered_multimap&& other)
            : parent(std::move(other))
        { // move contruct u_multimap from other
        }

        /* Initializer constructor */

        unordered_multimap(std::initializer_list<value_type> ilist,
                size_type bucket_count = 0,
                const hasher& hf = hasher(),
                const key_equal& keq = key_equal(),
                const nq_alloc& all = nq_alloc())
            : parent(ilist, bucket_count, hf, keq, all)
        { // construct u_multimap by copying elements in the list
        }

        /*** Assignement operator ***/

        unordered_multimap& operator=(const unordered_multimap& rhs)
        { // copy assignment operator
            this->parent::operator=(rhs);
            return *this;
        }

        unordered_multimap& operator=(unordered_multimap&& rhs)
        { // move assignment operator
            this->parent::operator=(std::move(rhs));
            return *this;
        }

        unordered_multimap& operator=(std::initializer_list<value_type> ilist)
        { // assign initializer_list
            this->parent::operator=(ilist);
            return *this;
        }
    };
}

#endif // NQ_UNORDERED_MULTIMAP_H_
