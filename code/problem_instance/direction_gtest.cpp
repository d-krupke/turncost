//
// Created by Dominik Krupke, http://krupke.cc on 8/9/17.
//

#include <gtest/gtest.h>
#include "direction.h"

TEST(Direction, Difference)
{
  using namespace turncostcover;
  ASSERT_EQ(GetDifference(Direction::NORTH, Direction::SOUTH), 2);
  ASSERT_EQ(GetDifference(Direction::SOUTH, Direction::NORTH), 2);
  ASSERT_EQ(GetDifference(Direction::NORTH, Direction::EAST), 1);
  ASSERT_EQ(GetDifference(Direction::NORTH, Direction::WEST), 1);
  ASSERT_EQ(GetDifference(Direction::NORTH, Direction::NORTH), 0);
  ASSERT_EQ(GetDifference(Direction::WEST, Direction::SOUTH), 1);
}

TEST(Direction, NeighborDirection)
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
  auto f_north = graph.GetVertex({0, 1});
  auto f_east = graph.GetVertex({1, 0});

  using namespace turncostcover;

  ASSERT_EQ(DeduceNeighborDirection(f1, f_north, graph), Direction::NORTH);
  ASSERT_EQ(DeduceNeighborDirection(f1, f_east, graph), Direction::EAST);
}