project(tsp_multi-objectif)

cmake_minimum_required(VERSION 2.8)
add_definitions(-Wall -std=c++11)

add_executable(tsp_multi-objectif tsp.cpp coordinates.cpp instance.cpp main.cpp)

install(TARGETS tsp_multi-objectif RUNTIME DESTINATION bin)