include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(linstance
        ./coordinate
        ./costs
        ./coverage
        ./direction
        ./grid_graph
        ./save_and_load
        ./solution/solution
        ./solution/simple_svg_1.0.0.hpp
        ./solution/visualization.h ./solution/visualization.cpp)
