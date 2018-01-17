//
// Created by Dominik Krupke, http://krupke.cc on 8/18/17.
//

#include <gtest/gtest.h>
#include "../../../problem_instance/grid_graph.h"
#include "path_to_cycle.h"

TEST(PathToCycle, BaseTest)
{

  std::set<turncostcover::Coordinate> coords = {{0, 0},
                                     {1, 0},
                                     {2, 0},
                                     {3, 0},
                                     {4, 0},
  };

  turncostcover::GridGraph graph{coords};

  auto coord_coverage = [&](const turncostcover::Coordinate a, const turncostcover::Coordinate b, const turncostcover::Coordinate c) -> turncostcover::Coverage {
      return {graph.GetVertex(a), graph.GetVertex(b), graph.GetVertex(c)};
  };

  std::vector<turncostcover::Field> path = {graph.GetVertex({0, 0}),
                                            graph.GetVertex({1, 0}),
                                            graph.GetVertex({2, 0})};
  auto cycle = turncostcover::apx::PathToCycle(path);

  ASSERT_EQ(cycle.size(), 4);
  ASSERT_EQ(std::count(cycle.begin(), cycle.end(), coord_coverage({1, 0}, {0, 0}, {1, 0})), 1);
  ASSERT_EQ(std::count(cycle.begin(), cycle.end(), coord_coverage({1, 0}, {2, 0}, {1, 0})), 1);
  ASSERT_EQ(std::count(cycle.begin(), cycle.end(), coord_coverage({0, 0}, {1, 0}, {2, 0})), 2);
}