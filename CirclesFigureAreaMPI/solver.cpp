#include "solver.h"

#include <mpi.h>

T solve(const std::vector< Circle* > *circles, int density, RandomFunction random, int max_cores)
{
    Rect *rect = getFigureRect(circles);
    std::pair<unsigned long, unsigned long> *result = generateAndCheckPoints(circles, rect, density, random, max_cores);
    if(NULL == result)
    {
        return (T)(-1);
    }

    unsigned long count = result->first;
    unsigned long intersects = result->second;

    return static_cast<T>(intersects)/static_cast<T>(count)* rect->area();
}

void *generatePointsProcess(void *params, const std::vector< Circle* > *circles)
{
    PointGeneratorStruct *data = (PointGeneratorStruct*)params;
    *data->result = 0;
    for(unsigned long i = 0; i < data->count; ++i)
    {
        Point* point = data->random(data->rect, data->density);
        if(isPointInsideCircles(data->circles, point))
        {
            ++(*data->result);
        }
    }
    return NULL;
}

void sendStruct(const PointGeneratorStruct *_struct, int to)
{
    MPI_Send(_struct, sizeof(*_struct), MPI_BYTE, to, 0, MPI_COMM_WORLD);
    MPI_Send(_struct->rect->br, sizeof(Point), MPI_BYTE, to, 1, MPI_COMM_WORLD);
    MPI_Send(_struct->rect->tl, sizeof(Point), MPI_BYTE, to, 2, MPI_COMM_WORLD);
}

void recvStruct(PointGeneratorStruct *_struct, int from)
{
    MPI_Status status;
    _struct = new PointGeneratorStruct(0, 0, 0, 0);
    MPI_Recv((void*)_struct, sizeof(*_struct), MPI_BYTE, from, 0, MPI_COMM_WORLD, &status);
    _struct->rect = new Rect(new Point(0, 0), new Point(0, 0));
    MPI_Recv(_struct->rect->br, sizeof(Point), MPI_BYTE, from, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(_struct->rect->tl, sizeof(Point), MPI_BYTE, from, 2, MPI_COMM_WORLD, &status);
}

void sendResult(unsigned long result, int to)
{
    MPI_Send(&result, 1, MPI_UNSIGNED_LONG, to, 3, MPI_COMM_WORLD);
}

void recvResult(unsigned long &result, int from)
{
    MPI_Status status;
    MPI_Recv(&result, 1, MPI_UNSIGNED_LONG, from, 3, MPI_COMM_WORLD, &status);
}

std::pair<unsigned long, unsigned long> *generateAndCheckPoints(
        const std::vector< Circle* > *circles, const Rect *rect, int density, RandomFunction random, int max_cores)
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

    unsigned long results[max_cores];

    // ---- MPI section ----

    for(int rank = 1; rank < max_cores; ++rank)
    {
        sendStruct(new PointGeneratorStruct(count/max_cores, density, rect, random), rank);
    }

    unsigned long real_count = count - count*(max_cores - 1)/max_cores;
    generatePointsProcess(new PointGeneratorStruct(real_count + real_count % max_cores, density, rect, random));


    std::cout << "Using " << max_cores << " cores" << std::endl;
    std::cout << "Gluing array chanks" << std::endl;

    for(int i = 1; i < max_cores; ++i)
    {
        std::cout << "[ " << i << " ] " << results[i] << std::endl;
        results[0] += results[i];
    }

    // ---- /MPI ----

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
