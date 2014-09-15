
#include "tracer.h"
#include "nq_allocator.h"
#include "nq_new.h"
#include "nq_vector.h"
#include <memory>

#include <vector>
#include "alloc_strat.h"
#include "type_to_string.h"
#include "nq_shared.h"

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
        nq::shared_ptr<int> swapped2 = nq::new_shared<int, DomainEarth>(100);

        swapped1.swap(swapped2);

        if (!(nullptr >= swapped1))
        {
            std::cout << "IM A PRINT YEE" << std::endl;
        }

        if (swapped1 >= swapped2)
        {
            std::cout << "whatever" << std::endl;
        }
        std::swap(swapped1, swapped2);
        if (!(swapped1 >= swapped2))
        {
            std::cout << "whatever 2.0 y" << std::endl;
        }

        if (swapped1)
        {
            std::cout << "bool tested" << std::endl;
        }

        std::cout << swapped1 << std::endl;


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
