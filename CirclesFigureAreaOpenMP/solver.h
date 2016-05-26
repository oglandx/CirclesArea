/*
 * File:   solver.h
 * Author: oglandx
 *
 * Created on February 17, 2016, 1:54 AM
 */

#ifndef CIRCLESFIGUREAREA_SOLVER_H
#define CIRCLESFIGUREAREA_SOLVER_H

#include "types.h"
#include <vector>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <chrono>
#include <omp.h>


T solve(const std::vector< Circle* > *circles, int density, RandomFunction random);
std::pair<unsigned long, unsigned long> *generateAndCheckPoints(
        const std::vector< Circle* > *circles, const Rect *rect, int density, RandomFunction random);
Rect *getFigureRect(const std::vector< Circle* > *circles);
bool isPointInsideCircles(const std::vector< Circle* > *circles,
                          const Point *point);


#endif //CIRCLESFIGUREAREA_SOLVER_H
