#ifndef NQ_UNIQUE_H_
# define NQ_UNIQUE_H_

# include "nq_deleter.h"

namespace nq
{
	template<class T,
		class Deleter = nq::deleter<T>>
	class unique_ptr : public std::unique_ptr<T, Deleter>
	{

	};
}

#endif // !NQ_UNIQUE_H_
