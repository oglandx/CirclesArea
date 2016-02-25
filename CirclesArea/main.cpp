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
#include <cmath>

#include "parser.h"
#include "solver.h"


namespace def {
    int MIN_ARGS_COUNT = 2;
    int MAX_CIRCLES_COUNT = 1000;
    int DENSITY = 1000;
};

typedef double T;

Coords2<T> rand_fun(const Rect<T> &rect, int density)
{
    // this is bad solution, can make better :)
    Coords2<int> distances = {
        .x = static_cast<int>(rect.br.x - rect.lt.x),
        .y = static_cast<int>(rect.br.y - rect.lt.y)
    };

    Coords2<T> t =  {
        .x = static_cast<T>(rand() % distances.x - distances.x/2),
        .y = static_cast<T>(rand() % distances.y - distances.y/2)
    };
    return t;
}

/*
 * 
 */
int main(int argc, char** argv) {
    int status = -1;
    if(argc < def::MIN_ARGS_COUNT)
    {
        std::cout << "Wrong count of command line args" << std::endl;
        return status;
    }

    std::vector< Circle<T> >  *circles = NULL;

    try{
        circles = parseCirclesFromFile<T>(std::string(argv[1]), def::MAX_CIRCLES_COUNT);
    }
    catch (FileException e)
    {
        std::cout << e.what();
    }

    if(NULL != circles)
    {
        srand((unsigned)time(NULL));
        std::function<Coords2<T>(const Rect<T>&,int)> *randomizer = new std::function<Coords2<T>(const Rect<T>&,int)>(&rand_fun);
        T result = solve<T>(circles, def::DENSITY, randomizer);

        delete randomizer;
        delete circles;

        std::cout << "Result = " << result << std::endl;
        status = 0;
    }

    return status;
}

