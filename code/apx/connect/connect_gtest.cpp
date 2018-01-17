//
// Created by Dominik Krupke, http://krupke.cc on 8/29/17.
//

#include <gtest/gtest.h>
#include "../../problem_instance/grid_graph.h"
#include "../../problem_instance/solution/solution.h"
#include "connect.h"

TEST(ComponentMst, ConnectSubset)
{
  using namespace turncostcover;
  std::set<Coordinate> coords = {{0, 0},
                                 {1, 0},
                                 {2, 0},
                                 {3, 0},
                                 {4, 0},
                                 {4, 1},
                                 {4, 2},
                                 {4, 3}
  };

  turncostcover::GridGraph graph{coords};

  turncostcover::IntegralSolution int_sol{graph};

  auto coord_coverage = [&](const Coordinate a, const Coordinate b, const Coordinate c) -> turncostcover::Coverage {
      return {graph.GetVertex(a), graph.GetVertex(b), graph.GetVertex(c)};
  };

  //comp_a
  int_sol.Add(coord_coverage({1, 0}, {0, 0}, {1, 0}), 1);
  int_sol.Add(coord_coverage({0, 0}, {1, 0}, {0, 0}), 1);

  //comp_b
  int_sol.Add(coord_coverage({4, 0}, {3, 0}, {4, 0}), 1);
  int_sol.Add(coord_coverage({3, 0}, {4, 0}, {3, 0}), 1);

  //comp_c
  int_sol.Add(coord_coverage({4, 2}, {4, 3}, {4, 2}), 1);
  int_sol.Add(coord_coverage({4, 3}, {4, 2}, {4, 3}), 1);

  ASSERT_EQ(int_sol.GetNumComponents(), 3);
  turncostcover::Costs costs{3, 1};
  apx::ConnectDistantCycles(&int_sol, costs);

  ASSERT_EQ(int_sol.GetNumComponents(), 1);
  ASSERT_EQ(int_sol.GetCoverageObjectiveValue(costs), 14 * costs.dist_costs + 6 * costs.turn_costs);
}