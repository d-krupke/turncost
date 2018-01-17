//
// Created by Dominik Krupke, http://krupke.cc on 8/16/17.
//

#include <gtest/gtest.h>
#include "../../../problem_instance/grid_graph.h"
#include "tcg_grid_graph_wrapper.h"
#include "../../../shortest_path/turn_cost_grid_dijkstra.h"

TEST(TCGGridGraphWrapper, Size)
{
  using namespace turncostcover;
  std::set<Coordinate> coords = {{0, 0},
                                 {1, 0},
                                 {2, 0},
                                 {0, 1},
                                 {1, 1},
                                 {2, 1},
                                 {0, 2},
                                 {1, 2},
                                 {2, 2}};

  turncostcover::GridGraph graph{coords};

  apx::TCGGridGraphWrapper sptGridGraphWrapper{graph};
  ASSERT_EQ(sptGridGraphWrapper.size(), coords.size());
}

TEST(TCGGridGraphWrapper, test)
{
  using namespace turncostcover;
  std::set<Coordinate> coords = {{0, 0},
                                 {1, 0},
                                 {2, 0},
                                 {0, 1},
                                 {1, 1},
                                 {2, 1},
                                 {0, 2},
                                 {1, 2},
                                 {2, 2}};

  turncostcover::GridGraph graph{coords};

  apx::TCGGridGraphWrapper sptGridGraphWrapper{graph};
  turncostgrid::TurnCostGridDijkstra
      turnCostGridDijkstra{sptGridGraphWrapper, {1, 1}};
  turnCostGridDijkstra.add_starting_point(sptGridGraphWrapper.GetId(graph.GetVertex(
      {0, 0})),
                                          turncostgrid::Direction::EAST);
  turnCostGridDijkstra.dijkstra();
  ASSERT_EQ(turnCostGridDijkstra.distance(sptGridGraphWrapper.GetId(graph.GetVertex(
      {2, 0})),
                                          turncostgrid::Direction::EAST), 2);
}