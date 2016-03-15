project(tsp_multi-objectif)

add_executable(tsp_multi-objectif instance.cpp main.cpp)

install(TARGETS tsp_multi-objectif RUNTIME DESTINATION bin)
