#ifndef TEST_DOMAINS_H_
# define TEST_DOMAINS_H_

#include "../../domains/nq_memlib/domains.h"

NQ_DOMAIN(DomainSpace, AllDomains);
NQ_DOMAIN(DomainEarth, AllDomains);
NQ_DOMAIN(SubDomainEarth, DomainEarth);

#endif // !TEST_DOMAINS_H_
