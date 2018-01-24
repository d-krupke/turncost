include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(cyclecover/orientation_matching/blossom5-v205)
add_subdirectory(connect/details/pcst_fast)

set(APX_SOURCES
        ../utils/cplex.hpp
        ../shortest_path/turn_cost_grid_dijkstra
        ../shortest_path/grid_graph
        ./cyclecover/orientation_selection/linear_program
        ./cyclecover/orientation_selection/select_orientations
        ./cyclecover/orientation_matching
        ./connect/details/greedy_connect
        ./cyclecover/orientation_matching/local_orientation_matching/auxiliary_graph
        ./connect/details/component_edge
        ./cyclecover/orientation_matching
        ./cyclecover/orientation_selection/solution
        ./cyclecover/orientation_matching/local_orientation_matching/local_matching
        ./connect/details/component_graph
        ./connect/details/tcg_grid_graph_wrapper
        ./connect/details/component_mst
        ./connect/details/path_to_cycle
        ./connect/details/connect_penalty_cycle_cover
        ./connect/connect.h
        ./approximation
        ./cyclecover/orientation_matching/orientation_matching connect/connect.cpp visualize.cpp visualize.h)

add_library(lapxalg ${APX_SOURCES})

include_directories(cyclecover/orientation_matching/blossom5-v205)
#add_subdirectory(cyclecover/matching/blossom5-v205)
include_directories(connect/details/pcst_fast)



message(${INCLUDE_DIRS})
target_include_directories(lapxalg PRIVATE ${INCLUDE_DIRS})
target_link_libraries(lapxalg pcstfast blossom5-bgl)
