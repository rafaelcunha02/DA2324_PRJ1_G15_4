cmake_minimum_required(VERSION 3.26)
project(untitled17)

set(CMAKE_CXX_STANDARD 17)

add_executable(untitled17 main.cpp
        src/reader.h
        src/reader.cpp
        data_structures/Graph.h
        data_structures/MutablePriorityQueue.h
        data_structures/UFDS.cpp
        data_structures/UFDS.h
        src/Pipe.h
        src/WaterReservoir.h
        src/PumpingStation.h
        src/City.h
)
