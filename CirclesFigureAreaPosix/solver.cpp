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

void *generatePointsThread(void *params)
{
    PointGeneratorStruct *data = (PointGeneratorStruct*)params;
    for(unsigned long i = 0; i < data->count; ++i)
    {
        Point* value = data->random(data->rect, data->density);
        pthread_mutex_lock(&data->mutex);
        data->result->push_back(value);
        pthread_mutex_unlock(&data->mutex);
    }
    return NULL;
}

std::vector< Point* > *generatePoints(const Rect *rect, int density, RandomFunction random)
{
    std::vector< Point* > *result = new std::vector< Point* > ();

    T area = rect->area();
    unsigned long count = density*static_cast<unsigned long>(area);

    int max_cores = 4;
    pthread_t threads[max_cores - 1];
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    for(int i = 0; i < max_cores - 1; ++i)
    {
        pthread_create(
                &threads[i],
                NULL,
                generatePointsThread,
                new PointGeneratorStruct(count/(max_cores - 1), density, rect, random, result, mutex)
        );
    }
    generatePointsThread(new PointGeneratorStruct(count - count/(max_cores - 1), density, rect, random, result, mutex));
    for(int i = 0; i < max_cores - 1; i++)
    {
        pthread_join(threads[i], NULL);
    }
    pthread_mutex_destroy(&mutex);

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
    for(typename std::vector< Point* >::const_iterator it = points->begin(); it != points->end(); it++)
    {
        if(isPointInsideCircles(circles, *it))
        {
            result++;
        }
    }
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
