#ifndef NQ_UNIQUE_H_
# define NQ_UNIQUE_H_

#include <memory>

# include "nq_deleter.h"

namespace nq
{
	template<class T,
		class Deleter = nq::deleter<T>>
	class unique_ptr : public std::unique_ptr<T, Deleter>
	{
		typedef std::unique_ptr<T, Deleter> parent;
		unique_ptr()
			: parent()
		{}
	};

	template<class T,
		class Deleter = nq::deleter<T>>
	class unique_ptr<T[], Deleter> : public std::unique_ptr<T[], Deleter>
	{
	};
}

#endif // !NQ_UNIQUE_H_
