#include <iostream>
#include <stdlib.h>

#include "solver.h"
#include "parser.h"
#include "randomizer.h"

#define TEST

namespace def {
    int MIN_ARGS_COUNT = 2;
    int MAX_CIRCLES_COUNT = 100;
    int DENSITY = 500;
};

void test_statistics(std::vector< Circle* >  *circles){
    int tests_count = 10;
    std::cout << ">>>Running statistics test. This test consists of " << tests_count << " iterations" << std::endl;
    T results[tests_count];
    for(int i = 0; i < tests_count; ++i){
        results[i] = solve(circles, def::DENSITY, &random_uniform_real_distribution);
        std::cout << "Result[" << i << "] = " << results[i] << std::endl;
    }
    std::cout << std::endl;

    T mean = results[0];
    for(int i = 1; i < tests_count; ++i){
        mean += results[i];
    }
    mean /= tests_count;

    std::cout << "Mean value = " << mean << std::endl;

    auto sqr = [](T x){return x*x;};
    T disp = sqr(results[0] - mean);
    for(int i = 0; i < tests_count; ++i){
        disp += sqr(results[0] - mean);
    }
    disp /= tests_count;

    std::cout << "Dispersion = " << disp << std::endl;
}

void run_tests(std::vector< Circle* >  *circles){
    test_statistics(circles);
}

int main() {
    int status = -1;
    std::cout.precision(10);

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