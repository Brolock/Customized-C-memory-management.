#ifndef NQ_MULTIMAP_H_
# define NQ_MULTIMAP_H_

# include <map>

# include "nq_allocator.h"
# include "domains.h"
# include "alloc_strat.h"

namespace nq
{
    template<class Key,
        class T,
        class Domain = UnknownDomain,
        class Compare = std::less<Key>,
        class AllocStrat = DefaultAlloc>
    class multimap
        : public std::multimap<const Key, T, Compare,
            nq::allocator<std::pair<const Key, T>, Domain, AllocStrat>>
    {
        typedef nq::allocator<std::pair<const Key, T>, 
                Domain, AllocStrat> nq_alloc;

        typedef std::multimap<const Key, T, Compare, nq_alloc> parent;

        typedef typename parent::value_type value_type;
        typedef typename parent::size_type size_type;
        typedef typename parent::key_compare value_compare;

    public:
        /*** Constructors ***/
        multimap()
            : parent()
        {  // construct empty multimap
        }

        explicit multimap(const value_compare& comp, const nq_alloc& all = nq_alloc())
            : parent(comp, all)
        {  // construct empty multimap
        }

        template<class Iterator>
        multimap(Iterator first, Iterator last)
            : parent(first, last)
        { // construct multimap with the contents [first, last)
        }

        template<class Iterator>
        multimap(Iterator first, Iterator last,
                const value_compare& comp,
                const nq_alloc& all = nq_alloc())
            : parent(first, last, comp, all)
        { // construct multimap with the contents [first, last)
        }

        /*** Copy constructors ***/

        multimap(const multimap& other)
            : parent(other)
        { // construct multimap from other
        }

        multimap(multimap&& other) noexcept
            : parent(std::move(other))
        { // construct multimap by taking other datas
        }

        multimap(std::initializer_list<value_type> list,
                const value_compare comp = value_compare(),
                const nq_alloc& all = nq_alloc())
            : parent(list, comp, all)
        { // construct multimap with the contents of the initializer list
        }

        /*** Assignement operator ***/
        
        multimap& operator=(const multimap& rhs)
        { // assign multimap with rhs
            this->parent::operator=(rhs);
            return *this;
        }

        multimap& operator=(multimap&& rhs)
        { // assign multimap with by taking the datas from rhs
            this->parent::operator=(std::move(rhs));
            return *this;
        }

        multimap& operator=(std::initializer_list<value_type> list)
        { // assign multimap with the contents of the initializer list
            this->parent::operator=(list);
            return *this;
        }
    };
}

#endif // NQ_MULTIMAP_H_
