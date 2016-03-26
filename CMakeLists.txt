project(tsp_multi-objectif)

<<<<<<< HEAD
cmake_minimum_required(VERSION 2.8)

add_definitions(-Wall)
=======
add_definitions(-Wall -std=c++11)
>>>>>>> 8b388ee0f8547bb32c3458d95eb8f9c00f6e5326

add_executable(tsp_multi-objectif tsp.cpp coordinates.cpp instance.cpp main.cpp)

install(TARGETS tsp_multi-objectif RUNTIME DESTINATION bin)
