#ifndef NQ_MULTISET_H_
# define NQ_MULTISET_H_

# include <set>

# include "nq_allocator.h"
# include "lib_domains.h"
# include "alloc_strat.h"

namespace nq
{
    template<class Key,
        class Domain = UnknownDomain,
        class Compare = std::less<Key>,
        class AllocStrat = DefaultAlloc>
    class multiset
        : public std::multiset<Key, Compare, nq::allocator<Key, Domain, AllocStrat>>
    {
        typedef nq::allocator<Key, Domain, AllocStrat> nq_alloc;
        typedef std::multiset<Key, Compare, nq_alloc> parent;

        typedef typename parent::value_type value_type;
        typedef typename parent::size_type size_type;
        typedef typename parent::value_compare value_compare;

    public:
        /*** Constructors ***/
        multiset()
            : parent()
        {  // construct empty multiset
        }

        explicit multiset(const value_compare& comp, const nq_alloc& all = nq_alloc())
            : parent(comp, all)
        {  // construct empty multiset
        }

        template<class Iterator>
        multiset(Iterator first, Iterator last)
            : parent(first, last)
        { // construct multiset with the contents [first, last)
        }

        template<class Iterator>
        multiset(Iterator first, Iterator last,
                const value_compare& comp,
                const nq_alloc& all = nq_alloc())
            : parent(first, last, comp, all)
        { // construct multiset with the contents [first, last)
        }

        /*** Copy constructors ***/

        multiset(const multiset& other)
            : parent(other)
        { // construct multiset from other
        }

        multiset(multiset&& other) noexcept
            : parent(std::move(other))
        { // construct multiset by taking other datas
        }

        multiset(std::initializer_list<value_type> list,
                const value_compare comp = value_compare(),
                const nq_alloc& all = nq_alloc())
            : parent(list, comp, all)
        { // construct multiset with the contents of the initializer list
        }

        /*** Assignement operator ***/
        
        multiset& operator=(const multiset& rhs)
        { // assign multiset with rhs
            this->parent::operator=(rhs);
            return *this;
        }

        multiset& operator=(multiset&& rhs)
        { // assign multiset with by taking the datas from rhs
            this->parent::operator=(std::move(rhs));
            return *this;
        }

        multiset& operator=(std::initializer_list<value_type> list)
        { // assign multiset with the contents of the initializer list
            this->parent::operator=(list);
            return *this;
        }
    };
}

#endif // NQ_MULTISET_H_
