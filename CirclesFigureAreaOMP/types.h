//
// Created by oglandx on 3/16/16.
//

#ifndef TYPES_H
#define	TYPES_H

#include <cmath>

typedef double T;

class Point
{
public:
    T x;
    T y;

    Point(T x, T y)
    {
        this->x = x;
        this->y = y;
    }

    Point(const Point *point)
    {
        this->x = point->x;
        this->y = point->y;
    }
};

class Circle
{
private:
    inline static T sqr(T x){ return x*x; }
public:
    Point *center;
    T radius;

    Circle(Point *center, T radius)
    {
        this->center = center;
        this->radius = radius;
    }

    ~Circle()
    {
        delete center;
    }

    bool containsPoint(const Point *point)
    {
        return this->sqr(this->center->x - point->x) +
               this->sqr(this->center->y - point->y) <= this->sqr(this->radius);
    }
};


class Rect
{
public:
    Point *tl;
    Point *br;

    Rect(Point *tl, Point *br)
    {
        this->tl = tl;
        this->br = br;
    }

    Rect(T x0, T y0, T x1, T y1)
    {
        this->tl = new Point(x0, y0);
        this->br = new Point(x1, y1);
    }

    ~Rect()
    {
        delete tl;
        delete br;
    }

    T area() const
    {
        return std::fabs((tl->x - br->x)*(tl->y - br->y));
    }
};

typedef Point* (*RandomFunction)(const Rect*, int);

#endif /* TYPES_H */