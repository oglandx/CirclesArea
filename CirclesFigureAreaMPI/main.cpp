#include <iostream>
#include <mpi.h>

#include "solver.h"
#include "parser.h"
#include "randomizer.h"

#define TEST

namespace def {
    int MIN_ARGS_COUNT = 2;
    int MAX_CIRCLES_COUNT = 100;
    int DENSITY = 500;
    int MAX_CORES = 6;
};

void show_circles(std::vector< Circle* >  *circles)
{
    int i = 0;
    std::cout << "Loaded circles:" << std::endl;
    for(std::vector< Circle* >::const_iterator it = circles->begin(); it != circles->end(); it++)
    {
        std::cout << "Circle(" << i++ << ") :: r = " << (*it)->radius <<
        ", center = (" << (*it)->center->x << ", " << (*it)->center->y << ")" << std::endl;
    }
    std::cout << std::endl << std::endl;
}

int main(int argc, char *argv[]) {
    int status = -1;
    std::cout.precision(10);

    std::vector< Circle* >  *circles = NULL;
    const std::string filename("/home/oglandx/circles.txt");

    try{
        circles = parseCirclesFromFile(filename, def::MAX_CIRCLES_COUNT);
    }
    catch (FileException e)
    {
        std::cout << e.what();
    }

    // ---- MPI section ----

    int rc = MPI_Init(&argc, &argv);

    if(rc)
    {
        std::cout << "MPI error occured" << std::endl;
        MPI_Abort(MPI_COMM_WORLD, rc);
        return status;
    }

    int mpi_rank;

    MPI_Comm_size(MPI_COMM_WORLD, &(def::MAX_CORES));
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

    if(mpi_rank == 0)
    {
        if(NULL != circles)
        {
            T result = solve(circles, def::DENSITY, &random_simple, def::MAX_CORES);
            std::cout << "Result = " << result << std::endl;

            status = 0;
        }
    }
    else
    {

    }

    MPI_Finalize();

    // ---- /MPI ----

    for(unsigned long i = 0; circles != NULL && i < circles->size(); ++i)
    {
        delete circles->at(i);
    }
    delete circles;

    return status;
}