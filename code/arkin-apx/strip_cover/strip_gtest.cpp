//
// Created by Dominik Krupke, http://krupke.cc on 10/2/17.
//

#include <gtest/gtest.h>
#include "strip.h"

TEST(ArkinApx, StripIntersect){
  using namespace turncostcover;
  using namespace arkinapx;
  Strip a{{{0,1}, turncostcover::Direction::WEST}, {{2,1}, turncostcover::Direction::EAST}};
  Strip b{{{1,0}, turncostcover::Direction::SOUTH}, {{1,2}, turncostcover::Direction::NORTH}};
  Strip b2{{{1,2}, turncostcover::Direction::NORTH},{{1,0}, turncostcover::Direction::SOUTH}};
  Strip c{{{0,2}, turncostcover::Direction::WEST}, {{2,2}, turncostcover::Direction::EAST}};
  Strip d{{{3,0}, turncostcover::Direction::SOUTH}, {{3,2}, turncostcover::Direction::NORTH}};

  Strip shortStripH{{{0,1}, turncostcover::Direction::WEST}, {{0,1}, turncostcover::Direction::EAST}};
  Strip shortStripV{{{0,1}, turncostcover::Direction::SOUTH}, {{0,1}, turncostcover::Direction::NORTH}};
  ASSERT_TRUE(shortStripH.DoIntersect(shortStripV));
  ASSERT_TRUE(shortStripV.DoIntersect(shortStripH));
  ASSERT_TRUE(a.DoIntersect(b));
  ASSERT_TRUE(a.DoIntersect(b2));

  ASSERT_FALSE(a.DoIntersect(c));
  ASSERT_FALSE(a.DoIntersect(d));
}
