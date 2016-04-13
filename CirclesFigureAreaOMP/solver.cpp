#include <iostream>
#include "solver.h"

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

    time_t t = time(NULL);
    std::cout << "$start_generation" << std::endl;

    #pragma omp parallel for num_threads(2) shared(result)
    for(unsigned long i = 0; i < count; ++i)
    {
        Point* value = random(rect, density);
        #pragma omp critical
        result->push_back(value);
    }
    std::cout << "$end_generation " << time(NULL) - t << std::endl;

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
    time_t t = time(NULL);
    std::cout << "$start_count_inside" << std::endl;
    for(typename std::vector< Point* >::const_iterator it = points->begin(); it != points->end(); it++)
    {
        if(isPointInsideCircles(circles, *it))
        {
            result++;
        }
    }
    std::cout << "$end_count_inside " << time(NULL) - t << std::endl;
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
