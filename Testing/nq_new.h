#ifndef NQ_NEW_H_
# define NQ_NEW_H_

# include "domains.h"
template <typename Domain, typename AllocStrat>
void *operator new(size_t size, Domain, AllocStrat);


template <typename Domain, class AllocStrat>
void operator delete(void* p, Domain, AllocStrat);

# include "nq_new.hxx"
#endif /* !NQ_NEW_H_ */