cmake_minimum_required(VERSION 3.7)
project(LazyGridGraphShortestPathWithTurnCosts)

set(CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set(SOURCE_FILES turn_cost_grid_dijkstra.cpp turn_cost_grid_dijkstra.h grid_graph.cpp grid_graph.h)
add_executable(LazyGridGraphShortestPathWithTurnCosts main.cpp ${SOURCE_FILES})