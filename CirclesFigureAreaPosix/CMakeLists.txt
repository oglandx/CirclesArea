cmake_minimum_required(VERSION 3.3)
project(CirclesFigureArea)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build/dist")

set(SOURCE_FILES
    main.cpp
    parser.cpp
    parser.h
    randomizer.h
    randomizer.cpp
    solver.cpp
    solver.h
    types.h)

add_executable(CirclesFigureArea ${SOURCE_FILES})
