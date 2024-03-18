cmake_minimum_required(VERSION 3.26)
project(untitled17)

set(CMAKE_CXX_STANDARD 17)

add_executable(untitled17 main.cpp
        src/System.h
        src/System.cpp
        data_structures/Graph.h
        data_structures/MutablePriorityQueue.h
        data_structures/UFDS.cpp
        data_structures/UFDS.h
        src/Reservoir.h
        src/Station.h
        src/City.h
)
