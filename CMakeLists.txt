cmake_minimum_required(VERSION 3.26)
project(DA_2324_PRJ1_G15_4)

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
        src/Pipe.h
        src/App.h
        src/App.cpp
)

# Doxygen Build
find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(BUILD_DOC_DIR "${CMAKE_SOURCE_DIR}/docs/output")
    if(NOT EXISTS "${BUILD_DOC_DIR}")
        file(MAKE_DIRECTORY "${BUILD_DOC_DIR}")
    endif()

    set(DOXYGEN_IN "${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile")
    set(DOXYGEN_OUT "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
    configure_file("${DOXYGEN_IN}" "${DOXYGEN_OUT}" @ONLY)

    message("Doxygen build started")
    add_custom_target(Doxygen ALL
            COMMAND "${DOXYGEN_EXECUTABLE}" "${DOXYGEN_OUT}"
            WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM)
else(DOXYGEN_FOUND)
    message("Doxygen needs to be installed to generate the documentation.")
endif(DOXYGEN_FOUND)