#include <iostream>
#include <stdlib.h>

#include "solver.h"
#include "parser.h"
#include "randomizer.h"

#define TEST

namespace def {
    int MAX_CIRCLES_COUNT = 100;
    int DENSITY = 500;
    int TESTS_COUNT = 50;
    int MAX_CORES = 6;
};

void show_circles(std::vector< Circle* >  *circles)
{
    int i = 0;
    std::cout << "[Loaded circles]" << std::endl;
    for(std::vector< Circle* >::const_iterator it = circles->begin(); it != circles->end(); it++)
    {
        std::cout << "Circle(" << i++ << ") :: r = " << (*it)->radius <<
        ", center = (" << (*it)->center->x << ", " << (*it)->center->y << ")" << std::endl;
    }
    std::cout << std::endl << std::endl;
}

template<typename __type>
void print_statistics(const std::string &name, __type results[], int count)
{
    std::cout << "[" << name << "]" << std::endl;

    __type mean = results[0];
    for(int i = 1; i < count; ++i)
    {
        mean += results[i];
    }
    mean /= count;

    std::cout << "Mean value = " << mean << std::endl;

    auto sqr = [](__type x){return x*x;};

    __type disp = sqr(results[0] - mean);

    for(int i = 1; i < count; ++i)
    {
        disp += sqr(results[i] - mean);
    }
    disp /= count;

    std::cout << "Dispersion = " << disp << std::endl << std::endl;
}

void test_statistics(std::vector< Circle* >  *circles)
{
    int tests_count = def::TESTS_COUNT;

    show_circles(circles);

    T results[tests_count];
    double times[tests_count];

    for(int i = 1; i < def::MAX_CORES + 1; ++i)
    {
        std::cout << ">>>Running statistics test. This test consists of " << tests_count <<
                " iterations for " << i << " cores" << std::endl << std::endl;
        for(int j = 0; j < tests_count; ++j)
        {
            double *result_time = new double;
            results[j] = solve(circles, def::DENSITY, random_uniform_real_distribution, i, result_time);
            times[j] = *result_time;
            delete result_time;

            std::cout.precision(10);
            std::cout << "Result[" << j << "] = " << results[j] << std::endl << std::endl;
        }
        std::cout << std::endl;

        print_statistics<T>(std::string("Result"), results, tests_count);

        std::cout.precision(5);
        print_statistics<double>(std::string("Time"), times, tests_count);
    }
}

void run_tests(std::vector< Circle* >  *circles){
    test_statistics(circles);
}

int main() {
    int status = -1;

    std::vector< Circle* >  *circles = NULL;
    const std::string filename("/home/oglandx/circles.txt");

    try{
        circles = parseCirclesFromFile(filename, def::MAX_CIRCLES_COUNT);
    }
    catch (FileException e)
    {
        std::cout << e.what();
    }

    if(NULL != circles)
    {
        srand((unsigned)time(NULL));

#ifdef TEST
        run_tests(circles);
#else
        T result = solve(circles, def::DENSITY, &random_simple);
        std::cout << "Result = " << result << std::endl;
#endif
        status = 0;
    }


    for(unsigned long i = 0; circles != NULL && i < circles->size(); ++i)
    {
        delete circles->at(i);
    }

    delete circles;

    return status;
}