/* 
 * File:   solver.h
 * Author: oglandx
 *
 * Created on February 17, 2016, 1:54 AM
 */

#ifndef SOLVER_H
#define	SOLVER_H

#include "types.h"
#include <functional>
#include <vector>



template<typename T> T solve(const std::vector< Circle<T> > *circles, int density, std::function<Coords2<T>(const Rect<T>&,int)> *randomizer);
template <typename T> std::vector< Coords2<T> > *generatePoints(const Rect<T> &rect, int density, std::function<Coords2<T>(const Rect<T&>,int)> *randomizer);
template<typename T> Rect<T> getFigureRect(const std::vector< Circle<T> > *circles);
template <typename T> unsigned long getCountInsideCircles(const std::vector< Circle<T> > *circles, const std::vector< Coords2<T> > *points);
template <typename T> bool isPointInsideCircles(const std::vector< Circle<T> > *circles, const Coords2<T> &point);
template<typename T> inline bool isPointInsideCircle(const Circle<T> &circle, const Coords2<T> &point);
template<typename T> inline T sqr(T value);
template <typename T> inline T sqrRect(Rect<T> rect);



template<typename T>
T solve(const std::vector< Circle<T> > *circles, int density, std::function<Coords2<T>(const Rect<T>&,int)> *randomizer)
{
    Rect<T> rect = getFigureRect(circles);
    std::vector< Coords2<T> > *points = generatePoints(rect, density, randomizer);
    unsigned long intersects = getCountInsideCircles(circles, points);
    unsigned long points_count = points->size() > 0 ? points->size() : 1ul;
    delete points;
    return static_cast<T>(intersects)/static_cast<T>(points_count)*sqrRect(rect);
}

template <typename T>
std::vector< Coords2<T> >*
generatePoints(const Rect<T> &rect, int density, std::function<Coords2<T>(const Rect<T>&,int)> *randomizer)
{
    std::vector< Coords2<T> > *result = new std::vector< Coords2<T> > ();

    T square = sqrRect(rect);
    int count = density*static_cast<int>(square);

    for(int i = 0; i < count; ++i)
    {
        Coords2<T> value = (*randomizer)(rect, density);
        result->push_back(value);
    }

    return result;
}

template<typename T>
Rect<T>
getFigureRect(const std::vector< Circle<T> > *circles)
{
    typename std::vector< Circle<T> >::const_iterator it = circles->cbegin();

    Coords2<T> min = {
            .x = it->center.x - it->radius,
            .y = it->center.y - it->radius
    };
    Coords2<T> max = {
            .x = it->center.x + it->radius,
            .y = it->center.y + it->radius
    };

    while(it != circles->cend())
    {
        it++;
        if(it->center.x - it->radius < min.x)
        {
            min.x = it->center.x - it->radius;
        }
        else if(it->center.x + it->radius > max.x)
        {
            max.x = it->center.x + it->radius;
        }

        if(it->center.y - it->radius < min.y)
        {
            min.y = it->center.y - it->radius;
        }
        else if(it->center.y - it->radius > max.y)
        {
            max.y = it->center.y + it->radius;
        }
    }
    return Rect<T>{
            .lt = min,
            .br = max
    };
}

template <typename T>
unsigned long getCountInsideCircles(const std::vector< Circle<T> > *circles, const std::vector< Coords2<T> > *points)
{
    unsigned long result = 0ul;
    for(typename std::vector< Coords2<T> >::const_iterator it = points->cbegin(); it != points->cend(); it++)
    {
        if(isPointInsideCircles(circles, *it))
        {
            result++;
        }
    }
    return result;
}

template <typename T>
bool isPointInsideCircles(const std::vector< Circle<T> > *circles, const Coords2<T> &point)
{
    for(typename std::vector< Circle<T> >::const_iterator it = circles->cbegin(); it != circles->cend(); it++)
    {
        if(isPointInsideCircle(*it, point))
        {
            return true;
        }
    }
    return false;
}

template<typename T>
inline bool
isPointInsideCircle(const Circle<T> &circle, const Coords2<T> &point)
{
    return sqr(point.x - circle.center.x) +
           sqr(point.y - circle.center.y) <= sqr(circle.radius);
}

template<typename T>
inline T
sqr(T value)
{
    return value*value;
}

template <typename T>
inline T
sqrRect(Rect<T> rect)
{
    return (rect.br.x - rect.lt.x)*(rect.br.y - rect.lt.y);
}

#endif	/* SOLVER_H */

