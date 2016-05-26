#include <iostream>
#include <mpi.h>

#include "solver.h"
#include "parser.h"
#include "randomizer.h"

#define TEST

namespace def {
    int MAX_CIRCLES_COUNT = 100;
    int DENSITY = 500;
    int TESTS_COUNT = 50;
};

void show_circles(std::vector< Circle* >  *circles)
{
    int i = 0;
    std::cout << "[Loaded circles]" << std::endl;
    for(std::vector< Circle* >::const_iterator it = circles->begin(); it != circles->end(); it++)
    {
        std::cout << "Circle(" << i++ << ") :: r = " << (*it)->radius <<
        ", center = (" << (*it)->center->x << ", " << (*it)->center->y << ")" << std::endl;
    }
    std::cout << std::endl << std::endl;
}

template<typename __type>
void print_statistics(const std::string &name, __type results[], int count)
{
    std::cout.precision(5);

    std::cout << "[" << name << "]" << std::endl;

    __type mean = results[0];
    for(int i = 1; i < count; ++i)
    {
        mean += results[i];
    }
    mean /= count;

    std::cout << "Mean value = " << mean << std::endl;

    auto sqr = [](__type x){return x*x;};

    __type disp = sqr(results[0] - mean);

    for(int i = 1; i < count; ++i)
    {
        disp += sqr(results[i] - mean);
    }
    disp /= count;

    std::cout << "Dispersion = " << disp << std::endl << std::endl;
}

void test_statistics(std::vector< Circle* >  *circles)
{
    int tests_count = def::TESTS_COUNT;

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int mpi_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

    const int main_rank = 0;

    if(mpi_rank == main_rank)
    {
        std::cout << ">>> Running tests" << std::endl;

        show_circles(circles);

        std::cout << "[Running statistics test] This test consists of " <<
        tests_count << " iterations for " << world_size << " cores" << std::endl << std::endl;
    }

    T results[tests_count];
    double times[tests_count];

    for(int i = 0; i < tests_count; ++i)
    {
        std::chrono::milliseconds start_time =
                std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

        results[i] = solve(circles, def::DENSITY, &random_uniform_real_distribution, world_size);

        std::chrono::milliseconds end_time =
                std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
        times[i] = static_cast<double>(end_time.count() - start_time.count())/1000;

        std::cout << "end_generation (process = " << mpi_rank << ", time = " << times[i] << "s)" << std::endl;

        MPI_Barrier(MPI_COMM_WORLD);

        if((T)(-1) != results[i])
        {
            std::cout.precision(10);
            std::cout << "Result[" << i << "] = " << results[i] << std::endl << std::endl;
        }
    }
    if((T)(-1) != results[0]){
        std::cout << std::endl;
        print_statistics<T>(std::string("Result"), results, tests_count);
    }
    MPI_Barrier(MPI_COMM_WORLD);

    if(mpi_rank == 0){
        double full_times[tests_count*world_size];
        memcpy(full_times, times, sizeof(double)*tests_count);

        MPI_Status status;
        for(int rank = 1; rank < world_size; ++rank)
        {
            MPI_Recv(times, tests_count, MPI_DOUBLE, rank, 4, MPI_COMM_WORLD, &status);
            memcpy(full_times + tests_count*rank, times, sizeof(double)*tests_count);
        }
        print_statistics<double>(std::string("Time"), times, tests_count);
    }
    else
    {
        MPI_Send(times, tests_count, MPI_DOUBLE, 0, 4, MPI_COMM_WORLD);
    }
    std::cout << std::endl;
}

void run_tests(std::vector< Circle* >  *circles){
    test_statistics(circles);
}

int main(int argc, char *argv[]) {
    int status = -1;

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
        std::cout << "MPI error occurred" << std::endl;
        MPI_Abort(MPI_COMM_WORLD, rc);
        return status;
    }

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if(NULL != circles)
    {
        #ifdef TEST

        run_tests(circles);

        #else

        T result = solve(circles, def::DENSITY, &random_uniform_real_distribution, world_size);
        if((T)(-1) != result){
            std::cout << "Result = " << result << std::endl;
        }

        #endif
        status = 0;
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