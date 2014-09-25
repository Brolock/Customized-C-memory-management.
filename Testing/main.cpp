
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


void test_nq_new(nq::vector<int*>& vec)
{
    for (int i = 0; i < 10000; ++i)
    {
    int *j = nq::New<int, DomainSpace>(2);
    vec.push_back(j);
    }
}

void test_new(std::vector<int*>& vec)
{
    for (int i = 0; i < 10000; ++i)
    {
        int *j = new int(2);
        vec.push_back(j);
    }
}

#include <chrono>
double loop()
{
    std::vector<int *> new_vec;
    nq::vector<int *> nq_vec;

    typedef std::chrono::microseconds ms;
    typedef std::chrono::duration<double> fs;


    auto start = std::chrono::system_clock::now();
    test_nq_new(nq_vec);
    auto end = std::chrono::high_resolution_clock::now();
    fs nq_sec = end - start;



    auto new_start = std::chrono::system_clock::now();
    test_new(new_vec);
    auto new_end = std::chrono::high_resolution_clock::now();
    fs std_sec = new_end - new_start;

    fs new_sec = std_sec - nq_sec;

    double lol = ((nq_sec.count() - std_sec.count()) / std_sec.count()) * 100;

    for (auto& it : new_vec)
    { delete it; }
    for (auto& it : nq_vec)
    { nq::Delete<int, DomainSpace>(it); }

    /*
    std::cout << "NQ: " << nq_sec.count() << std::endl;
    std::cout << "NEW: " << std_sec.count() << std::endl;
    std::cout << "NEW - NQ :" << new_sec.count() << std::endl;
    std::cout << "percent loss: " << lol << std::endl;
    */

    return lol;
}


int main()
{
    double result = 0;
    int nb_loop = 10000;
    for (int i = 0; i < nb_loop; ++i)
    {
        result += loop();
    }
    result /= nb_loop;

    std::cout << result << std::endl;



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
