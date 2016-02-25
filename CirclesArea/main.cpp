/* 
 * File:   main.cpp
 * Author: oglandx
 *
 * Created on February 17, 2016, 1:30 AM
 * 
 * Finds area value of circles set with Monte-Carlo method
 * 
 * Input is array of tuples with coordinates of circle center and its radius.
 * 
 */

#include <cstdlib>
#include <iostream>

#include "parser.h"
#include "solver.h"


namespace def {
    int MIN_ARGS_COUNT = 2;
    int MAX_CIRCLES_COUNT = 1000;
    int DENSITY = 100;
};

typedef double T;

/*
 * 
 */
int main(int argc, char** argv) {
    if(argc < def::MIN_ARGS_COUNT)
    {
        std::cout << "Wrong count of command line args" << std::endl;
        return -1;
    }
    std::vector< Circle<T> >  *circles = parseCirclesFromFile<T>(std::string(argv[1]), def::MAX_CIRCLES_COUNT);
    srand((unsigned)time(NULL));
    std::function<T(void)> *randomizer = new std::function<T()>(&std::rand);
    T result = solve<T>(circles, def::DENSITY, randomizer);

    std::cout << "Result = " << result << std::endl;

    return 0;
}

