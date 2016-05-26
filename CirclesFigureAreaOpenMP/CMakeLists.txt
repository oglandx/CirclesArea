cmake_minimum_required(VERSION 3.3)
project(CirclesFigureArea)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -pg -fopenmp -DPROFILE ")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -fopenmp -DNDEBUG")

set(SOURCE_FILES
    main.cpp
    parser.cpp
    parser.h
    solver.cpp
    solver.h
    types.h
    randomizer.h
    randomizer.cpp)

add_executable(CirclesFigureArea ${SOURCE_FILES})