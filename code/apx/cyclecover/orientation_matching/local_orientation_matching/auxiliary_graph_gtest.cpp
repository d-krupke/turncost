//
// Created by Dominik Krupke, http://krupke.cc on 8/9/17.
//

#include <gtest/gtest.h>
#include "../../../../problem_instance/coordinate.h"
#include "../../../../problem_instance/grid_graph.h"
#include "../../../../problem_instance/direction.h"
#include "auxiliary_graph.h"

TEST(AuxiliaryGraph, CalculateCost)
{
  std::set<turncostcover::Coordinate> coords = {{0, 0},
                                     {1, 0},
                                     {2, 0},
                                     {0, 1},
                                     {1, 1},
                                     {2, 1},
                                     {0, 2},
                                     {1, 2},
                                     {2, 2}};

  turncostcover::GridGraph graph{coords};
  auto f1 = graph.GetVertex({0, 0});
  auto f2 = graph.GetVertex({0, 1});

  using namespace turncostcover;
  using namespace apx;

  ASSERT_EQ(ComputeCost(graph, MatchingVertexInformation{f1, Direction::SOUTH},
                           MatchingVertexInformation{f2, Direction::NORTH}).turns, 4);
  ASSERT_EQ(ComputeCost(graph, MatchingVertexInformation{f1, Direction::NORTH},
                           MatchingVertexInformation{f2, Direction::WEST}).turns, 1);
  ASSERT_EQ(ComputeCost(graph, MatchingVertexInformation{f1, Direction::NORTH},
                           MatchingVertexInformation{f2, Direction::NORTH}).turns, 2);
  ASSERT_EQ(ComputeCost(graph, MatchingVertexInformation{f1, Direction::NORTH},
                           MatchingVertexInformation{f2, Direction::SOUTH}).turns, 0);
  ASSERT_EQ(ComputeCost(graph, MatchingVertexInformation{f1, Direction::WEST},
                           MatchingVertexInformation{f2, Direction::EAST}).turns, 2);
  ASSERT_EQ(ComputeCost(graph, MatchingVertexInformation{f1, Direction::EAST},
                           MatchingVertexInformation{f2, Direction::WEST}).turns, 2);
  ASSERT_EQ(ComputeCost(graph, MatchingVertexInformation{f1, Direction::WEST},
                           MatchingVertexInformation{f2, Direction::WEST}).turns, 2);
}

