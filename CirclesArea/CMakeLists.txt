cmake_minimum_required(VERSION 3.3)
project(CirclesArea)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        parser.cpp
        parser.h
    main.cpp
    solver.cpp
    solver.h
    types.h)

add_executable(CirclesArea ${SOURCE_FILES})