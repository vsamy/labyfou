# Version minimum
cmake_minimum_required(VERSION 2.8)
include(cmake/base.cmake)
include(cmake/eigen.cmake)
include(cmake/boost.cmake)

set(PROJECT_NAME labyfou)
set(PROJECT_DESCRIPTION "A game based on a labyrinth")
set(PROJECT_URL "https://github.com/vsamy/labyfou")

# C++ flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

project(${PROJECT_NAME} CXX)
SETUP_PROJECT()

add_subdirectory(src)

SETUP_PROJECT_FINALIZE()