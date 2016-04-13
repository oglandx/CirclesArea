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
    int DENSITY = 20000;

};

typedef double T;

Coords2<T> *rand_fun(const Rect<T> &rect, int density)
{
    // ax = (x1-x0)/xm; ay = (y1-y0)/ym;
    // bx = x0; by = y0;
    // x = ax*xrand + bx; y = ax*yrand + by

    Coords2<T> norms = {
            .x = static_cast<T>(density),
            .y = static_cast<T>(density)
    };
    Coords2<T> transform_a = {
        .x = (rect.br.x - rect.lt.x)/norms.x,
        .y = (rect.br.y - rect.lt.y)/norms.y
    };
    Coords2<T> transform_b = rect.lt;

    Coords2<T> *result = new Coords2<T>;
    result->x = static_cast<T>(rand() % static_cast<int>(norms.x))*transform_a.x + transform_b.x;
    result->y = static_cast<T>(rand() % static_cast<int>(norms.y))*transform_a.y + transform_b.y;
    return result;
}

/*
 * Main function. Performs all basic logic.
 */
int main(int argc, char** argv) {
    int status = -1;
    if(argc < def::MIN_ARGS_COUNT)
    {
        std::cout << "Wrong count of command line args" << std::endl;
        return status;
    }

    std::vector< Circle<T>* >  *circles = NULL;

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
        RandomFunction *randomizer = new RandomFunction(&rand_fun);
        T result = solve<T>(circles, def::DENSITY, randomizer);

        delete randomizer;
        delete circles;

        std::cout << "Result = " << result << std::endl;
        status = 0;
    }

    for(unsigned long i = 0; circles != NULL && i < circles->size(); ++i)
    {
        delete circles->at(i);
    }
    if(circles != NULL)
    {
        delete circles;
    }

    return status;
}

