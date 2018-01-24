//
// Created by Dominik Krupke, http://krupke.cc on 10/9/17.
//


#include <gtest/gtest.h>
#include "strip_cover.h"

TEST(ArkinApx, StripCover)
{
  std::set<turncostcover::Coordinate> coords{{0, 0},
                                  {1, 0},
                                  {2, 0},
                                  {0, 1},
                                  {1, 1},
                                  {2, 1}};
  turncostcover::GridGraph graph{coords};

  using namespace turncostcover;
  using namespace arkinapx;
  auto strips = arkinapx::ComputeStripCover(graph);
  ASSERT_EQ(strips.size(), 2);
  Strip h1{{{0, 0}, turncostcover::Direction::WEST},
           {{2, 0}, turncostcover::Direction::EAST}};
  Strip h2{{{0, 1}, turncostcover::Direction::WEST},
           {{2, 1}, turncostcover::Direction::EAST}};
  Strip v3{{{2,0}, turncostcover::Direction::SOUTH}, {{2,1}, turncostcover::Direction::NORTH}};
  ASSERT_TRUE(std::find(strips.begin(), strips.end(), h1) != strips.end());
  ASSERT_TRUE(std::find(strips.begin(), strips.end(), h2) != strips.end());
  ASSERT_FALSE(std::find(strips.begin(), strips.end(), v3) != strips.end());
}