
#include "tracer.h"
#include "nq_allocator.h"
#include "nq_new.h"
#include "nq_vector.h"
#include <memory>

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
		: num_(o.num_)
	{
		std::cout << "Test(Test&" << o.num_ << ", " << o.whatever_<< ")" << std::endl;
	}
};


int main()
{
    nq::shared_ptr<int> swapped1 = nq::new_shared<int, DomainSpace>(33);
    {
        auto test_un = nq::make_unique<Test>(2, 3);

        nq::vector<int, DomainEarth> vec_test = {1, 3, 5, 7};

        std::cout << vec_test[3] << std::endl;

        nq::vector<int> vec_swap({3, 3});

        for (auto it : vec_swap)
        {
            std::cout << it << std::endl;
        }

        nq::shared_ptr<int> swapped2 = nq::new_shared<int, DomainEarth>(100);
        nq::shared_ptr<Test> whatever(std::move(test_un));

        swapped1.swap(swapped2);

        DomainEarth::getInstance().print();
        DomainSpace::getInstance().print();
        UnknownDomain::getInstance().print();
        SharedPtrRefCountDomain::getInstance().print();
    }
    DomainEarth::getInstance().print();
    DomainSpace::getInstance().print();
    UnknownDomain::getInstance().print();
    SharedPtrRefCountDomain::getInstance().print();
    /*
	nq::vector<Test, DomainSpace> x;
	x.push_back(Test(4));
	x.push_back(Test(5));
	x.push_back(Test(7));
	x.push_back(Test(1));
	x.push_back(Test(345));
	x.push_back(Test(345));
	x.push_back(Test(345));
	nq::vector<Test, DomainEarth, DefaultAlloc> v;
	v.push_back(Test(1));
	v.push_back(Test(2));
	v.push_back(Test(3));
	v.push_back(Test(4));
	v.push_back(Test(1214));

	DomainSpace::getInstance().print();
	DomainEarth::getInstance().print();
    */
}
