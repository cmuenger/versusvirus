
set(libary_sources
    entities.cpp
    parameters.cpp 
    postprocess.cpp
    simulation.cpp
    util.cpp
)

add_library(ABM ${libary_sources})

add_executable_png(main main.cpp)
target_link_libraries(main ABM)