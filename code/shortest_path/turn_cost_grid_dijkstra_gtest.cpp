//
// Created by Dominik Krupke, http://krupke.cc on 8/16/17.
//

#include <gtest/gtest.h>
#include "grid_graph.h"
#include "turn_cost_grid_dijkstra.h"
#include "DFieldId.h"

TEST(TurnCostGridDijkstra, simple_test)
{
  using namespace turncostgrid;
  std::vector<GridCoordinate> coords = {{0, 0},
                                        {1, 0},
                                        {2, 0},
                                        {3, 0},
                                        {4, 0},
                                        {5, 0},
                                        {6, 0}};
  GridGraph gridGraph{coords, true};
  TurnCostGridDijkstra turnCostGridDijkstra{gridGraph, {2, 1}};
  turnCostGridDijkstra.add_starting_point(0, Direction::EAST);
  turnCostGridDijkstra.dijkstra();
  ASSERT_EQ(turnCostGridDijkstra.distance(2, Direction::EAST), 2);
  ASSERT_EQ(turnCostGridDijkstra.distance(1, Direction::EAST), 1);
  ASSERT_EQ(turnCostGridDijkstra.distance(2, Direction::WEST), 6);
  ASSERT_EQ(turnCostGridDijkstra.distance(1, Direction::NORTH), 3);
}