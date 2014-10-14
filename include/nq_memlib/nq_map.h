#ifndef NQ_MAP_H_
# define NQ_MAP_H_

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
    class map
        : public std::map<const Key, T, Compare,
            nq::allocator<std::pair<const Key, T>, Domain, AllocStrat>>
    {
        typedef nq::allocator<std::pair<const Key, T>, 
                Domain, AllocStrat> nq_alloc;

        typedef std::map<const Key, T, Compare, nq_alloc> parent;

        typedef typename parent::value_type value_type;
        typedef typename parent::size_type size_type;
        typedef typename parent::key_compare value_compare;

    public:
        /*** Constructors ***/
        map()
            : parent()
        {  // construct empty map
        }

        explicit map(const value_compare& comp, const nq_alloc& all = nq_alloc())
            : parent(comp, all)
        {  // construct empty map
        }

        template<class Iterator>
        map(Iterator first, Iterator last)
            : parent(first, last)
        { // construct map with the contents [first, last)
        }

        template<class Iterator>
        map(Iterator first, Iterator last,
                const value_compare& comp,
                const nq_alloc& all = nq_alloc())
            : parent(first, last, comp, all)
        { // construct map with the contents [first, last)
        }

        /*** Copy constructors ***/

        map(const map& other)
            : parent(other)
        { // construct map from other
        }

        map(map&& other) noexcept
            : parent(std::move(other))
        { // construct map by taking other datas
        }

        map(std::initializer_list<value_type> list,
                const value_compare comp = value_compare(),
                const nq_alloc& all = nq_alloc())
            : parent(list, comp, all)
        { // construct map with the contents of the initializer list
        }

        /*** Assignement operator ***/
        
        map& operator=(const map& rhs)
        { // assign map with rhs
            this->parent::operator=(rhs);
            return *this;
        }

        map& operator=(map&& rhs)
        { // assign map with by taking the datas from rhs
            this->parent::operator=(std::move(rhs));
            return *this;
        }

        map& operator=(std::initializer_list<value_type> list)
        { // assign map with the contents of the initializer list
            this->parent::operator=(list);
            return *this;
        }
    };
}

#endif // NQ_MAP_H_
