#include "solver.h"

T solve(const std::vector< Circle* > *circles, int density, RandomFunction random)
{
    Rect *rect = getFigureRect(circles);
    std::pair<unsigned long, unsigned long> *result = generateAndCheckPoints(circles, rect, density, random);
    if(NULL == result)
    {
        return (T)(-1);
    }

    unsigned long count = result->first;
    unsigned long intersects = result->second;

    return static_cast<T>(intersects)/static_cast<T>(count)* rect->area();
}

std::pair<unsigned long, unsigned long> *generateAndCheckPoints(
        const std::vector< Circle* > *circles, const Rect *rect, int density, RandomFunction random)
{
    T area = rect->area();
    int count = density*static_cast<int>(area);

    int max_cores = 7;

    unsigned long results[max_cores];

    for(int i = 0; i < max_cores; ++i){
        results[i] = 0;
    }

    std::chrono::milliseconds start_time =
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    std::cout << "$start_generation...." << std::endl;

    #pragma omp parallel for shared(rect, density, circles, count, results)
    for(int i = 0; i < count; ++i)
    {
//        Point *point = random(rect, density);
        Point *point = new Point(0, 0);
        if(isPointInsideCircles(circles, point))
        {
            int thread_num = omp_get_thread_num();
            ++results[thread_num];
        }
    }

    std::cout << 0 << "---" << results[0] << std::endl;

    for(int i = 1; i < max_cores; ++i){
        std::cout << i << "---" << results[i] << std::endl;
        results[0] += results[i];
    }

    std::chrono::milliseconds end_time =
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    std::cout << "end_generation (time = " << end_time.count() - start_time.count() << ")" << std::endl;

    return new std::pair<unsigned long, unsigned long>(count, results[0]);
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
