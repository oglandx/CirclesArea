#include "solver.h"

T solve(const std::vector< Circle* > *circles, int density, RandomFunction random, int max_cores, double *result_time)
{
    Rect *rect = getFigureRect(circles);
    std::pair<unsigned long, unsigned long> *result =
            generateAndCheckPoints(circles, rect, density, random, max_cores, result_time);

    if(NULL == result)
    {
        return (T)(-1);
    }

    unsigned long count = result->first;
    unsigned long intersects = result->second;

    return static_cast<T>(intersects)/static_cast<T>(count)* rect->area();
}

void *generatePointsThread(void *params)
{
    PointGeneratorStruct *data = (PointGeneratorStruct*)params;
    data->result = 0;
    for(unsigned long i = 0; i < data->count; ++i)
    {
        Point* point = data->random(data->rect, data->density);
        if(isPointInsideCircles(data->circles, point))
        {
            ++(data->result);
        }
        delete point;
    }
    return NULL;
}

std::pair<unsigned long, unsigned long> *generateAndCheckPoints(
        const std::vector< Circle* > *circles, const Rect *rect, int density, RandomFunction random, int max_cores,
        double *result_time)
{
    if(max_cores < 1)
    {
        return NULL;
    }

    T area = rect->area();
    unsigned long count = density*static_cast<unsigned long>(area);

    std::chrono::milliseconds start_time =
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    std::cout << "$start_generation...." << std::endl;

    PointGeneratorStruct *params[max_cores];

    pthread_t threads[max_cores - 1];
    for(int i = 0; i < max_cores - 1; ++i)
    {
        params[i] = new PointGeneratorStruct(count/max_cores, density, rect, random, circles);
        pthread_create(
                &threads[i],
                NULL,
                generatePointsThread,
                params[i]
        );
    }

    unsigned long real_count = count - count*(max_cores - 1)/max_cores;
    params[max_cores - 1] =
            new PointGeneratorStruct(real_count + real_count % max_cores, density, rect, random, circles);
    generatePointsThread(params[max_cores - 1]);

    for(int i = 0; i < max_cores - 1; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    std::cout << "Using " << max_cores << " cores" << std::endl;

    unsigned long result = params[0]->result;

    for(int i = 1; i < max_cores; ++i)
    {
        result += params[i]->result;
        delete params[i];
    }

    std::chrono::milliseconds end_time =
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    double time = static_cast<double>(end_time.count() - start_time.count())/1000;

    std::cout << "end_generation (time = " << time << "s)" << std::endl;

    if(NULL != result_time)
    {
        *result_time = time;
    }

    return new std::pair<unsigned long, unsigned long>(count, result);
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
