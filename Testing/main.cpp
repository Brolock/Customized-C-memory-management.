
#include "tracer.h"
#include "nq_allocator.h"
#include "nq_new.h"
#include "nq_vector.h"
#include "nq_set.h"
#include "nq_unordered_set.h"
#include "nq_unordered_map.h"
#include "nq_unordered_multiset.h"
#include "nq_multiset.h"
#include "nq_map.h"
#include "nq_multimap.h"
#include "nq_deque.h"
#include "nq_forward_list.h"
#include "nq_list.h"
#include <memory>
#include <unordered_set>

#include <vector>
#include "alloc_strat.h"
#include "type_to_string.h"
#include "nq_shared.h"
#include "nq_unique.h"

#include <type_traits>

#include <algorithm>
#include <iostream>

#include "Header.h"
#include "nq_deleter.h"

nq::Tracer NewTrace;

struct Test
{
private:
	int num_;
	int whatever_;
public:
	Test()
		: num_(55),
        whatever_(33)
	{
		std::cout << "Test()" << std::endl;
	}
	Test(int i, int j)
		: num_(i),
		whatever_(j)
	{
		std::cout << "Test(" << num_ << ", " << whatever_ << ")" << std::endl;
	}
	~Test()
	{
		std::cout << "~Test(" << num_ << ", " << whatever_ << ")" << std::endl;
	}
	Test(const Test& o)
		: num_(o.num_),
          whatever_(o.whatever_)
	{
		std::cout << "Test(Test&" << o.num_ << ", " << o.whatever_<< ")" << std::endl;
	}
};


int main()
{
    int *k = new(DomainEarth::getInstance(), __FILE__, __LINE__) int(3);
    nq::vector<int, DomainEarth> v{1, 2, 3};

    DomainEarth::getInstance().print();
    delete k;
   /* 
        UnknownDomain::getInstance().print();
    Test* ptr = new Test[10];
        UnknownDomain::getInstance().print();

    delete[] ptr;

    UnknownDomain::getInstance().print();

    std::unique_ptr<int[]> salut;

    */
    /*
    {
        DomainEarth::getInstance().print();
        DomainSpace::getInstance().print();
        UnknownDomain::getInstance().print();
        SharedPtrRefCountDomain::getInstance().print();
        std::cout << "=================" << std::endl;
    }
    DomainEarth::getInstance().print();
    DomainSpace::getInstance().print();
    UnknownDomain::getInstance().print();
    SharedPtrRefCountDomain::getInstance().print();
    */
}
