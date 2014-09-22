#ifndef NQ_SET_H_
# define NQ_SET_H_

# include <set>

# include "nq_allocator.h"
# include "domains.h"
# include "alloc_strat.h"

namespace nq
{
    template<class Key,
        class Domain = UnknownDomain,
        class Compare = std::less<Key>,
        class AllocStrat = DefaultAlloc>
    class set
        : public std::set<Key, Compare, nq::allocator<Key, Domain, AllocStrat>>
    {
        typedef nq::allocator<Key, Domain, AllocStrat> nq_alloc;
        typedef std::set<Key, Compare, nq_alloc> parent;

    public:
        /*** Constructors ***/
        set()
            : parent()
        {  // construct empty set
        }

        explicit set(const Compare& comp, const nq_alloc& all = nq_alloc())
            : parent(comp, all)
        {  // construct empty set
        }

        template<class Iterator>
        set(Iterator first, Iterator last)
            : parent(first, last)
        { // construct set with the contents [first, last)
        }

        template<class Iterator>
        set(Iterator first, Iterator last, const Compare& comp,
                const nq_alloc& all = nq_alloc())
            : parent(first, last, comp, all)
        { // construct set with the contents [first, last)
        }

        /*** Copy constructors ***/

        set(const set& other)
            : parent(other)
        { // construct set from other
        }

        set(set&& other) noexcept
            : parent(std::move(other))
        { // construct set by taking other datas
        }

        set(std::initializer_list<Key> list, const Compare& comp = Compare(),
                const nq_alloc& all = nq_alloc())
            : parent(list, comp, all)
        { // construct set with the contents of the initializer list
        }

        /*** Assignement operator ***/
        
        set& operator=(const set& rhs)
        { // assign set with rhs
            this->parent::operator=(rhs);
            return *this;
        }

        set& operator=(set&& rhs)
        { // assign set with by taking the datas from rhs
            this->parent::operator=(std::move(rhs));
            return *this;
        }

        set& operator=(std::initializer_list<Key> list)
        { // assign set with the contents of the initializer list
            this->parent::operator=(list);
            return *this;
        }
    };
}

#endif // NQ_SET_H_
