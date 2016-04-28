#include "solver.h"

T solve(const std::vector< Circle* > *circles, int density, RandomFunction random, int max_cores)
{
    Rect *rect = getFigureRect(circles);
    std::vector< Point* > *points = generatePoints(rect, density, random, max_cores);
    unsigned long intersects = getCountInsideCircles(circles, points, max_cores);
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
//        Point* value = new Point(0, 0);
        Point* value = data->random(data->rect, data->density);
        data->result->push_back(value);
    }
    return NULL;
}

std::vector< Point* > *generatePoints(const Rect *rect, int density, RandomFunction random, int max_cores)
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

    std::vector< Point* > *results[max_cores];
    for(int i = 0; i < max_cores; ++i)
    {
        results[i] = new std::vector< Point* > ();
    }

    pthread_t threads[max_cores - 1];
    for(int i = 0; i < max_cores - 1; ++i)
    {
        pthread_create(
                &threads[i],
                NULL,
                generatePointsThread,
                new PointGeneratorStruct(count/max_cores, density, rect, random, results[i])
        );
    }

    unsigned long real_count = count - count*(max_cores - 1)/max_cores;
    generatePointsThread(new PointGeneratorStruct(
            real_count + real_count % max_cores, density, rect, random, results[max_cores - 1]));
    for(int i = 0; i < max_cores - 1; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    std::cout << "Using " << max_cores << " cores" << std::endl;
    std::cout << "Gluing array chanks" << std::endl;
    std::cout << "[ 0 ] " << results[0]->size() << std::endl;
    for(int i = 1; i < max_cores; ++i)
    {
        std::cout << "[ " << i << " ] " << results[i]->size() << std::endl;
        results[0]->insert(results[0]->end(), results[i]->begin(), results[i]->end());
    }
    std::cout << "[all] " << results[0]->size() << std::endl;

    std::chrono::milliseconds end_time =
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    std::cout << "end_generation (time = " << end_time.count() - start_time.count() << ")" << std::endl;

    return results[0];
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

void *getCountInsideCirclesThread(void *params)
{
    CheckCirclesStruct *data = (CheckCirclesStruct*)params;
    std::chrono::milliseconds start_time =
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    std::cout << "$calc " << data->points->size() << " ...." << std::endl;

    *data->result = 0;
    for(typename std::vector< Point* >::const_iterator it = data->points->begin(); it != data->points->end(); it++)
    {
        if(isPointInsideCircles(data->circles, *it))
        {
            ++(*data->result);
        }
    }

    std::chrono::milliseconds end_time =
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    std::cout << "end (time = " << end_time.count() - start_time.count() << ")" << std::endl;
    return NULL;
}

unsigned long getCountInsideCircles(
        const std::vector< Circle* > *circles, const std::vector< Point* > *points, int max_cores)
{
    if(max_cores < 1)
    {
        return 0ul - 1ul;
    }

    std::chrono::milliseconds start_time =
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    std::cout << "$start_calc...." << std::endl;

    pthread_t threads[max_cores - 1];
    unsigned long results[max_cores];
    unsigned long elements_per_core = circles->size()/max_cores;
    for(int i = 0; i < max_cores - 1; ++i)
    {
        std::vector< Point* >* p = new std::vector< Point* >(
                        points->begin() + elements_per_core*i,
                        points->begin() + elements_per_core*(i+1));
        pthread_create(
                &threads[i],
                NULL,
                getCountInsideCirclesThread,
                new CheckCirclesStruct(
                        new std::vector< Point* >(
                                points->begin() + elements_per_core*i,
                                points->begin() + elements_per_core*(i+1)),
                        circles,
                        &results[i])
        );
    }

    getCountInsideCirclesThread(
            new CheckCirclesStruct(
                    new std::vector< Point* >(
                            points->begin() + elements_per_core*(max_cores - 1),
                            points->end()),
                    circles,
                    &results[max_cores - 1]));

    for(int i = 1; i < max_cores; i++){
        results[0] += results[i];
    }

    std::chrono::milliseconds end_time =
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    std::cout << "end_calc (time = " << end_time.count() - start_time.count() << ")" << std::endl;

    return results[0];
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
