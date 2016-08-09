# Version minimum
cmake_minimum_required(VERSION 2.8.11)

# Declaration du projet
project(labyfou)
set(labyfou_VERSION_MAJOR 1)
set(labyfou_VERSION_MINOR 0)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

message("The used compiler is ${CMAKE_CXX_COMPILER_ID}")
SET(STRICT_COMPILATION "False" CACHE STRING "")
if ("${STRICT_COMPILATION}" STREQUAL "True")
  SET(STRICT_COMPILATION_FLAGS "-Weffc++ -Wswitch-enum")
  message("More strict compilation enable (Weffc++ not set for clang though)")
else()
  SET(STRICT_COMPILATION_FLAGS "")
  message("Minimal warning compilation enable")
endif()

# C++ flags
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic -Wall -Wextra -Wold-style-cast -Woverloaded-virtual -Wfloat-equal -Wwrite-strings -Wpointer-arith -Wcast-qual -Wcast-align -Wconversion -Wshadow -Weffc++ -Wredundant-decls -Wdouble-promotion -Winit-self -Wswitch-default -Wswitch-enum -Wundef -Wlogical-op -Winline")
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic -Wall -Wextra -Wold-style-cast -Woverloaded-virtual -Wfloat-equal -Wwrite-strings -Wpointer-arith -Wcast-qual -Wcast-align -Wconversion -Wshadow -Wredundant-decls -Winit-self -Wswitch-default -Wswitch -Wundef -Winline ${STRICT_COMPILATION_FLAGS}")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic -Wall -Wextra -Wold-style-cast -Woverloaded-virtual -Wfloat-equal -Wwrite-strings -Wpointer-arith -Wcast-qual -Wcast-align -Wconversion -Wshadow -Wredundant-decls -Wdouble-promotion -Winit-self -Wswitch-default -Wswitch -Wundef -Wlogical-op -Winline ${STRICT_COMPILATION_FLAGS}")
endif()

set(CMAKE_INSTALL_PREFIX "${CMAKE_MODULE_PATH}")

add_subdirectory(src)