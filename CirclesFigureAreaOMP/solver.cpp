#include <iostream>
#include <chrono>
#include "solver.h"

void __diagnostic_show_points(std::vector< Point* > *points)
{
    for(std::vector< Point* >::const_iterator it = points->begin(); it != points->end(); it++)
    {
        std::cout << "Point (" << (*it)->x << ", " << (*it)->y << ")" << std::endl;
    }
}

T solve(const std::vector< Circle* > *circles, int density, RandomFunction random)
{
    Rect *rect = getFigureRect(circles);
    std::vector< Point* > *points = generatePoints(rect, density, random);
    unsigned long intersects = getCountInsideCircles(circles, points);
    unsigned long points_count = points->size() > 0 ? points->size() : 1ul;

    for(unsigned long i = 0; i < points->size(); ++i)
    {
        delete points->at(i);
    }
    delete points;

    return static_cast<T>(intersects)/static_cast<T>(points_count)* rect->area();
}

std::vector< Point* > *generatePoints(const Rect *rect, int density, RandomFunction random)
{
    std::vector< Point* > *result = new std::vector< Point* > ();

    T area = rect->area();
    unsigned long count = density*static_cast<unsigned long>(area);

    std::chrono::milliseconds start_time =
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    std::cout << "$start_generation...." << std::endl;

    #pragma omp parallel for num_threads(3) shared(result, rect, density)
    for(unsigned long i = 0; i < count; ++i)
    {
        Point* value = random(rect, density);
        #pragma omp critical
        result->push_back(value);
    }

    std::chrono::milliseconds end_time =
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    std::cout << "end_generation (time = " << end_time.count() - start_time.count() << ")" << std::endl;

    return result;
}

Rect *getFigureRect(const std::vector< Circle* > *circles)
{
    typename std::vector< Circle* >::const_iterator it = circles->begin();

    Point *min = new Point(
            (*it)->center->x - (*it)->radius,
            (*it)->center->y - (*it)->radius
    );
    Point *max = new Point(
            (*it)->center->x + (*it)->radius,
            (*it)->center->y + (*it)->radius
    );

    while(++it != circles->end())
    {
        if((*it)->center->x - (*it)->radius < min->x)
        {
            min->x = (*it)->center->x - (*it)->radius;
        }
        else if((*it)->center->x + (*it)->radius > max->x)
        {
            max->x = (*it)->center->x + (*it)->radius;
        }

        if((*it)->center->y - (*it)->radius < min->y)
        {
            min->y = (*it)->center->y - (*it)->radius;
        }
        else if((*it)->center->y - (*it)->radius > max->y)
        {
            max->y = (*it)->center->y + (*it)->radius;
        }
    }
    return new Rect(min, max);
}

unsigned long getCountInsideCircles(const std::vector< Circle* > *circles, const std::vector< Point* > *points)
{
    unsigned long result = 0ul;
    std::chrono::milliseconds start_time =
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    std::cout << "$start_count_inside..... " << std::endl;
    for(typename std::vector< Point* >::const_iterator it = points->begin(); it != points->end(); it++)
    {
        if(isPointInsideCircles(circles, *it))
        {
            result++;
        }
    }
    std::chrono::milliseconds end_time =
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    std::cout << "end_count_inside (time = " << end_time.count() - start_time.count() << ")" << std::endl;
    return result;
}

bool isPointInsideCircles(const std::vector< Circle* > *circles, const Point *point)
{
    for(typename std::vector< Circle* >::const_iterator it = circles->begin(); it != circles->end(); it++)
    {
        if((*it)->containsPoint(point))
        {
            return true;
        }
    }
    return false;
}
