#include <chrono>
#include <vector>

#include "nq_vector.h"
#include "nq_memlib_new.h"
#include "domains.h"

#include "nq_new.h"

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
	}
	Test(int i, int j, int h)
		: num_(i),
		whatever_(j),
		break_(h)
	{
	}
	~Test()
	{
	}
	Test(const Test& o)
		: num_(o.num_),
		whatever_(o.whatever_),
		break_(o.break_)
	{
	}
};

void test_nq_new(nq::vector<Test*, DomainEarth>& vec)
{
    for (int i = 0; i < 10000; ++i)
    {
    //Test *j = nq::memlib::New<Test, DomainSpace>(i % 10000, (i+1) % 10000, (i + 2) % 10000);
    Test *j = new(DomainSpace::getInstance(), __FILE__, __LINE__) Test(i % 10000, (i+1) % 10000, (i + 2) % 10000);
    vec.push_back(j);
    }
}

void test_new(std::vector<Test*>& vec)
{
    for (int i = 0; i < 10000; ++i)
    {
        Test *j = new Test(i % 10000, (i+1) % 10000, (i + 2) % 10000);
        vec.push_back(j);
    }
}

double loop()
{
    std::vector<Test*> new_vec;
    nq::vector<Test*, DomainEarth> nq_vec;

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

    double perf_loss= ((nq_sec.count() - std_sec.count()) / std_sec.count()) * 100;

    for (auto& it : new_vec)
    { delete it; }
    for (auto& it : nq_vec)
    //{ nq::memlib::Delete<Test, DomainSpace>(it); }
    { delete it; }
    return perf_loss;
}

void perf_test()
{
    double result = 0;
    int nb_loop = 1000;
    for (int i = 0; i < nb_loop; ++i)
    {
        result += loop();
    }
    result /= nb_loop;

    std::cout << result << std::endl;
}
