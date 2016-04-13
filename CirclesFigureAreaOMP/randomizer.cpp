/* 
 * File:   randomizer.cpp
 * Author: oglandx
 * 
 * Created on March 30, 2016, 1:10 AM
 */

#include "randomizer.h"

Point *random_simple(const Rect *rect, int density)
{
    // ax = (x1-x0)/xm; ay = (y1-y0)/ym;
    // bx = x0; by = y0;
    // x = ax*xrand + bx; y = ax*yrand + by

    Point norms = Point(
            static_cast<T>(density),
            static_cast<T>(density));
    Point transform_a = Point(
            (rect->br->x - rect->tl->x)/norms.x,
            (rect->br->y - rect->tl->y)/norms.y);
    Point *transform_b = rect->tl;

    Point *result = new Point(
            static_cast<T>(rand() % static_cast<int>(norms.x))*transform_a.x + transform_b->x,
            static_cast<T>(rand() % static_cast<int>(norms.y))*transform_a.y + transform_b->y);
    return result;
}

Point *random_uniform_real_distribution(const Rect *rect, int density){
    static std::random_device seed;
    static std::mt19937 generator(seed());
    std::uniform_real_distribution<T> distribution_x(rect->tl->x, rect->br->x);
    std::uniform_real_distribution<T> distribution_y(rect->tl->y, rect->br->y);

    return new Point(distribution_x(generator), distribution_y(generator));
}