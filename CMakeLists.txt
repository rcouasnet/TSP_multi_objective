project(tsp_multi-objectif)

add_definitions(-Wall)

add_executable(tsp_multi-objectif coordinates.cpp instance.cpp main.cpp)

install(TARGETS tsp_multi-objectif RUNTIME DESTINATION bin)
