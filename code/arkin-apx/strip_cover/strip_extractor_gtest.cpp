//
// Created by Dominik Krupke, http://krupke.cc on 10/2/17.
//

#include <gtest/gtest.h>
#include "../../problem_instance/coordinate.h"
#include "../../problem_instance/grid_graph.h"
#include "strip_extractor.h"

TEST(ArkinApx, StripExtractor)
{
  std::set<turncostcover::Coordinate>
      coords{{0, 0}, {1, 0}, {2, 0}, {0, 1}, {1, 1}, {2, 1}};
  turncostcover::GridGraph graph{coords};
  using namespace turncostcover;
  using namespace arkinapx;
  StripExtractor stripExtractor;
  auto strips = stripExtractor.ExtractStrips(graph);
  ASSERT_EQ(strips.size(), 5);
  Strip h1{{{0, 0}, turncostcover::Direction::WEST},
           {{2, 0}, turncostcover::Direction::EAST}};
  Strip h2{{{0, 1}, turncostcover::Direction::WEST},
           {{2, 1}, turncostcover::Direction::EAST}};
  Strip v1{{{0, 0}, turncostcover::Direction::SOUTH},
           {{0, 1}, turncostcover::Direction::NORTH}};
  Strip v2{{{1, 0}, turncostcover::Direction::SOUTH},
           {{1, 1}, turncostcover::Direction::NORTH}};
  Strip v3{{{2, 0}, turncostcover::Direction::SOUTH},
           {{2, 1}, turncostcover::Direction::NORTH}};
  ASSERT_TRUE(std::find(strips.begin(), strips.end(), h1) != strips.end());
  ASSERT_TRUE(std::find(strips.begin(), strips.end(), h2) != strips.end());
  ASSERT_TRUE(std::find(strips.begin(), strips.end(), v1) != strips.end());
  ASSERT_TRUE(std::find(strips.begin(), strips.end(), v2) != strips.end());
  ASSERT_TRUE(std::find(strips.begin(), strips.end(), v3) != strips.end());
}