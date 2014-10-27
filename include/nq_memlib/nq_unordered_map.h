#ifndef NQ_UNORDERED_MAP_H_
# define NQ_UNORDERED_MAP_H_

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
    class unordered_map
    : public std::unordered_map<Key, T, Hash, KeyEqual,
            nq::allocator<std::pair<Key, T>, Domain, AllocStrat>>
    {
        typedef nq::allocator<std::pair<Key, T>, Domain, AllocStrat> nq_alloc;
        typedef std::unordered_map<Key, T, Hash, KeyEqual, nq_alloc> parent;

        typedef typename parent::value_type value_type;
        typedef typename parent::size_type size_type;
        typedef typename parent::hasher hasher;
        typedef typename parent::key_equal key_equal;
        
    public:
        /*** Constructors ***/

        explicit unordered_map(size_type bucket_count = 10,
                const hasher hf = hasher(),
                const key_equal& keq = key_equal(),
                const nq_alloc& all = nq_alloc())
            : parent(bucket_count, hf, keq, all)
        { // construct empty u_map
        }

        explicit unordered_map(nq_alloc& all)
            : parent(all)
        { // construct empty u_map, nq_alloc
        }

        template<class Iterator>
        unordered_map(Iterator first, Iterator last,
                size_type buckets = 0,
                const hasher hf = hasher(),
                const key_equal& keq = key_equal(),
                const nq_alloc& all = nq_alloc())
            : parent(first, last, buckets, hf, keq, all)
        { // construct u_map from sequence [first, last)
        }


        /* Copy contructor */

        unordered_map(const unordered_map& other)
            : parent(other)
        { // copy contruct u_map from other
        }

        /* Move constructor */

        unordered_map(unordered_map&& other)
            : parent(std::move(other))
        { // move contruct u_map from other
        }

        /* Initializer constructor */

        unordered_map(std::initializer_list<value_type> ilist,
                size_type bucket_count = 0,
                const hasher& hf = hasher(),
                const key_equal& keq = key_equal(),
                const nq_alloc& all = nq_alloc())
            : parent(ilist, bucket_count, hf, keq, all)
        { // construct u_map by copying elements in the list
        }

        /*** Assignement operator ***/

        unordered_map& operator=(const unordered_map& rhs)
        { // copy assignment operator
            this->parent::operator=(rhs);
            return *this;
        }

        unordered_map& operator=(unordered_map&& rhs)
        { // move assignment operator
            this->parent::operator=(std::move(rhs));
            return *this;
        }

        unordered_map& operator=(std::initializer_list<value_type> ilist)
        { // assign initializer_list
            this->parent::operator=(ilist);
            return *this;
        }
    };
}

#endif // NQ_UNORDERED_MAP_H_
