#ifndef NQ_VECTOR_H_
# define NQ_VECTOR_H_

/* TODO ALALALALALFH EJHWBJK jkbek */

# include <vector>
# include "nq_allocator.h"
# include "alloc_strat.h"
# include "domains.h"

namespace nq
{
	template <typename T, class Domain = UnknownDomain, class AllocStrat = DefaultAlloc>
	class vector : public std::vector < T, nq::allocator<T, Domain, AllocStrat> >
	{
		typedef std::vector<T, nq::allocator<T, Domain, AllocStrat>> parent;
	public:
		vector()
		{}
		vector(size_t count)
			: parent(count)
		{}
	};
}

#endif // !NQ_VECTOR_H_