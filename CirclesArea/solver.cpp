#include "solver.h"

#include <ctime>

template <typename T> std::vector< Coords2<T> > &generatePoints(const Rect<T> &rect, int density, std::function<T ()> &randomizer);
template<typename T> Rect<T> getFigureRect(const std::vector< Circle<T> > &circles);
template <typename T> unsigned long getCountInsideCircles(const std::vector< Circle<T> > &circles, const std::vector< Coords2<T> > &points);
template <typename T> bool isPointInsideCircles(const std::vector< Circle<T> > &circles, const Coords2<T> &point);
template<typename T> inline bool isPointInsideCircle(const Circle<T> &circle, const Coords2<T> &point);
template<typename T> inline T sqr(T value);

template<typename T>
T solve(const std::vector< Circle<T> > circles, int density, std::function<T ()> &randomizer)
{
    Rect<T> rect = getFigureRect(circles);
    std::vector< Coords2<T> > points = generatePoints(rect, density, randomizer);
    unsigned long intersects = getCountInsideCircles(circles, points);
    return static_cast<T>(intersects/points.size());
}

template <typename T>
std::vector< Coords2<T> >
&generatePoints(const Rect<T> &rect, int density, std::function<T ()> &randomizer)
{
    std::vector< Coords2<T> > result = std::vector< Coords2<T> > ();

    T square = (rect.br.x - rect.lt.x)*(rect.br.y - rect.lt.y);
    int count = density*static_cast<int>(square);

    for(int i = 0; i < count; ++i)
    {
        T value = randomizer();
        result.push_back(value);
    }

    return result;
}

template<typename T>
Rect<T>
getFigureRect(const std::vector< Circle<T> > &circles)
{
    typename std::vector< Circle<T> >::const_iterator it = circles.cbegin();

    Coords2<T> min = {
        .x = it->center.x - it->radius,
        .y = it->center.y - it->radius
    };
    Coords2<T> max = {
        .x = it->center.x + it->radius,
        .y = it->center.y + it->radius
    };

    while(it != circles.cend())
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
unsigned long getCountInsideCircles(const std::vector< Circle<T> > &circles, const std::vector< Coords2<T> > &points)
{
    unsigned long result = 0ul;
    for(std::vector< Coords2<T> >::const_iterator it = points.cbegin(); it != points.cend(); it++)
    {
        if(isPointInsideCircles(circles, *it))
        {
            result++;
        }
    }
    return result;
}

template <typename T>
bool isPointInsideCircles(const std::vector< Circle<T> > &circles, const Coords2<T> &point)
{
    for(std::vector< Circle<T> >::const_iterator it = circles.cbegin(); it != circles.cend(); it++)
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