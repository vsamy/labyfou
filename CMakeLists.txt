# Version minimum
cmake_minimum_required(VERSION 2.8)

set(PROJECT_NAME labyfou)
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}")
set(SFML_ROOT "${CMAKE_SOURCE_DIR}/SFML")

# C++ flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

project(${PROJECT_NAME} CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
find_package(SFML 2.4 REQUIRED system window graphics network audio)

add_subdirectory(src)

