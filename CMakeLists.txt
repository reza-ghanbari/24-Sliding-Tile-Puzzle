cmake_minimum_required(VERSION 3.25)
project(Code)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-O3")

add_executable(Code main.cpp models/State.cpp inc/State.h inc/Types.h models/NeighborCache.cpp inc/NeighborCache.h inc/Constants.h models/Heuristic.cpp inc/Heuristic.h models/Solver.cpp inc/Solver.h)
