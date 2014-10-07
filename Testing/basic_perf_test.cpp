#include <chrono>
#include <vector>
#include "nq_vector.h"
#include "nq_memlib_new.h"

#include "nq_new.h"

void test_nq_new(nq::vector<int*>& vec)
{
    for (int i = 0; i < 10000; ++i)
    {
    //int *j = nq::memlib::New<int, DomainSpace>(2);
    int *j = new(DomainSpace::getInstance(), __FILE__, __LINE__) int(2);
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

double loop()
{
    std::vector<int*> new_vec;
    nq::vector<int*> nq_vec;

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
    //{ nq::memlib::Delete<int, DomainSpace>(it); }
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
