#include "nq_allocator.h"
#include "nq_memlib_new.h"
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

struct Test
{
private:
	int num_;
	int whatever_;
    int break_;
public:
	Test()
		: num_(55),
        whatever_(33),
        break_(123)
	{
		std::cout << "Test()" << std::endl;
	}
	Test(int i, int j, int h)
		: num_(i),
		whatever_(j),
        break_(h)
	{
		std::cout << "Test(" << num_ << ", " << whatever_ << ")" << std::endl;
	}
	~Test()
	{
		std::cout << "~Test(" << num_ << ", " << whatever_ << ")" << std::endl;
	}
	Test(const Test& o)
		: num_(o.num_),
          whatever_(o.whatever_),
          break_(o.break_)
	{
		std::cout << "Test(Test&" << o.num_ << ", " << o.whatever_
            << ", " << o.break_ << ")" << std::endl;
	}

    void print()
    {
        std::cout << num_ << ", " << whatever_ << ", " << break_ << std::endl;
    }
};

void print(Test* a, int n)
{
    std::cout << "----------\n";
    for(int t = 0; t < n; t++)
        a[t].print();
}

# define NQ_NEW(Domain) new (Domain::getInstance(), __FILE__, __LINE__)

int main()
{
    Test *ptr = nq::memlib::New_array<Test, DomainSpace>(3, {Test(1, 2, 3), Test(4, 44, 22), Test(324, 0, 78)});

    print(ptr, 3);

    {
        nq::vector<int> j{1, 3, 4};

        /*
        nq::vector<int, DomainEarth> lol{1};
        lol.push_back(3);

        nq::set<int, DomainEarth> l{2, 5, 32};
        */
    
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
}
