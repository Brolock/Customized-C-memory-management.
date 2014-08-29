#include <Windows.h>
#undef max
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
public:
	Test()
		: num_(55)
	{
		std::cout << "Test()" << std::endl;
	}
	Test(int i)
		: num_(i)
	{
		std::cout << "Test(" << num_ << ")" << std::endl;
	}
	~Test()
	{
		std::cout << "~Test(" << num_ << ")" << std::endl;
	}
	Test(const Test& o)
		: num_(o.num_)
	{
		std::cout << "Test(Test&" << o.num_ << ")" << std::endl;
	}
};
int main()
{
	std::default_delete<int> qweqf;
	//nq::shared_ptr<int> j;
	std::shared_ptr<int> h(new int(3), nq::deleter < int >(), nq::allocator < int > {});
	
	std::shared_ptr<int> j(new int(3), nq::deleter < int >(), test::test_allocator < int> {});
	//std::make_shared<int>(2);
	//nq::make_shared<int>(5);
	
	//std::shared_ptr<int> p = std::make_shared<int>(1);
	
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

	std::cout << nq::TypeToString<nq::allocator<int>>() << std::endl;
	std::cout << nq::TypeToString<nq::allocator<int, DomainEarth, DefaultAlloc>>() << std::endl;
	system("pause");
}

/*
new (domain, alloc_strat)Maclass()
{
	nq::allocator < Maclass, Domain, defaultstrat >.allocate();
}
*/


/*
To work out part 3 of low level C++ XIII

int AddOneTo(int param)
{
int local = param + 1;
return local;
}

int main(int argc, char **argv)
{
int iResult = 0;
iResult = AddOneTo(iResult);
return 0;
}
*/