cmake_minimum_required(VERSION 3.3)
project(CirclesFigureArea)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

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