//
// Created by Dominik Krupke, http://krupke.cc on 8/16/17.
//

#include <gtest/gtest.h>
#include "../../../problem_instance/coordinate.h"
#include "../../../problem_instance/grid_graph.h"
#include "../../../problem_instance/coverage.h"
#include "../../../problem_instance/solution/solution.h"
#include "component_graph.h"

TEST(ComponentGraph, SimpleTest)
{
  using namespace turncostcover;
  std::set<Coordinate> coords = {{0, 0},
                                 {1, 0},
                                 {2, 0},
                                 {3, 0},
                                 {4, 0},
                                 {0, 1},
                                 {1, 1},
                                 {2, 1},
                                 {3, 1},
                                 {4, 1},
                                 {0, 2},
                                 {1, 2},
                                 {2, 2},
                                 {3, 2},
                                 {4, 2}};

  turncostcover::GridGraph graph{coords};

  turncostcover::IntegralSolution int_sol{graph};

  auto coord_coverage = [&](const Coordinate a, const Coordinate b, const Coordinate c) -> turncostcover::Coverage {
      return {graph.GetVertex(a), graph.GetVertex(b), graph.GetVertex(c)};
  };

  //left cycle
  int_sol.Add(coord_coverage({0, 1}, {0, 0}, {1, 0}), 1);
  int_sol.Add(coord_coverage({0, 0}, {1, 0}, {1, 1}), 1);
  int_sol.Add(coord_coverage({1, 0}, {1, 1}, {1, 2}), 1);
  int_sol.Add(coord_coverage({1, 1}, {1, 2}, {0, 2}), 1);
  int_sol.Add(coord_coverage({1, 2}, {0, 2}, {0, 1}), 1);
  int_sol.Add(coord_coverage({0, 2}, {0, 1}, {0, 0}), 1);

  //right cycle
  int_sol.Add(coord_coverage({3, 1}, {3, 0}, {4, 0}), 1);
  int_sol.Add(coord_coverage({3, 0}, {4, 0}, {4, 1}), 1);
  int_sol.Add(coord_coverage({4, 0}, {4, 1}, {4, 2}), 1);
  int_sol.Add(coord_coverage({4, 1}, {4, 2}, {3, 2}), 1);
  int_sol.Add(coord_coverage({4, 2}, {3, 2}, {3, 1}), 1);
  int_sol.Add(coord_coverage({3, 2}, {3, 1}, {3, 0}), 1);

  ASSERT_TRUE(int_sol.IsFeasible());

  turncostcover::Costs costs{3, 2};//tc, dc

  apx::ComponentGraph comp_graph{int_sol, costs};
  comp_graph.ComputeConnectionCosts();
  ASSERT_EQ(comp_graph.GetConnectionCost(1, 2), 2 * costs.dist_costs);
  ASSERT_EQ(comp_graph.GetShortestPath(1, 2).size(), 3);

}