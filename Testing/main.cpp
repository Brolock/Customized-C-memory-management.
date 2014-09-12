
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
    nq::shared_ptr<Test> anoth_test = nq::make_shared<Test>(5, 6);
    {
        nq::shared_ptr<Test> j{nq::New<Test, DomainEarth>(1, 2), nq::deleter<Test, DomainEarth>{}};

        nq::shared_ptr<Test> testse = nq::new_shared<Test, DomainSpace>(3, 4);
        nq::shared_ptr<Test> wait_wat(anoth_test);

        if (wait_wat == anoth_test)
            std::cout << "YOYO NUMERO UNO" << std::endl;

        nq::swap(wait_wat, testse);
        if (testse == anoth_test)
            std::cout << "ME AND MY DAWG YEH" << std::endl;

        if (wait_wat != anoth_test)
            std::cout << "THREESOME BECHZ" << std::endl;

        if(wait_wat)
            std::cout << "No joke, not joking, jokinorino" << std::endl;

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
