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

    return static_cast<T>(intersects)/static_cast<T>(count) * rect->area();
}

unsigned long generatePointsProcess(PointGeneratorStruct *data, const std::vector< Circle* > *circles, RandomFunction random)
{
    unsigned long result = 0;
    for(unsigned long i = 0; i < data->count; ++i)
    {
        Point* point = random(data->rect, data->density);
        if(isPointInsideCircles(circles, point))
        {
            ++result;
        }
        delete point;
    }
    delete data;
    return result;
}

void sendResult(unsigned long result, int to)
{
    MPI_Send(&result, 1, MPI_UNSIGNED_LONG, to, 3, MPI_COMM_WORLD);
}

void recvResult(unsigned long *result, int from)
{
    MPI_Status status;
    MPI_Recv(result, 1, MPI_UNSIGNED_LONG, from, 3, MPI_COMM_WORLD, &status);
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

    unsigned long result;

    // ---- MPI section ----

    int mpi_rank;
    const int rank_main = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

    if(mpi_rank == rank_main)
    {
        unsigned long real_count = count - count*(max_cores - 1)/max_cores;
        result = generatePointsProcess(new PointGeneratorStruct(real_count, density, rect), circles, random);
    }
    else
    {
        result = generatePointsProcess(new PointGeneratorStruct(count/max_cores, density, rect), circles, random);
        sendResult(result, rank_main);
        return NULL;
    }

    for(int rank = 1; rank < max_cores; ++rank)
    {
        unsigned long current_result;
        recvResult(&current_result, rank);
        result += current_result;
    }

    // ---- /MPI ----

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
