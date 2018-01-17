//
// Created by Dominik Krupke, http://krupke.cc on 8/4/17.
//

#include <gtest/gtest.h>
#include "../../../problem_instance/grid_graph.h"
#include "../../../problem_instance/solution/solution.h"
#include "greedy_connect.h"

TEST(Connect, FindCheapestSingleCycle)
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
  int_sol.Add(coord_coverage({1, 1}, {1, 0}, {2, 0}), 1);
  int_sol.Add(coord_coverage({1, 0}, {2, 0}, {2, 1}), 1);
  int_sol.Add(coord_coverage({2, 0}, {2, 1}, {2, 2}), 1);
  int_sol.Add(coord_coverage({2, 1}, {2, 2}, {1, 2}), 1);
  int_sol.Add(coord_coverage({2, 2}, {1, 2}, {1, 1}), 1);
  int_sol.Add(coord_coverage({1, 2}, {1, 1}, {1, 0}), 1);

  ASSERT_EQ(int_sol.GetNumComponents(), 1);
  ASSERT_FALSE(apx::details::FindCheapestMerge(int_sol,
                                               {1, 1},
                                               0).IsFeasible());
}

TEST(Connect, FindCheapestTwoCycle)
{
  using namespace turncostcover;
  std::set<Coordinate> coords = {{0, 0},
                                 {1, 0},
                                 {2, 0},
                                 {3, 0},
                                 {0, 1},
                                 {1, 1},
                                 {2, 1},
                                 {3, 1},
                                 {0, 2},
                                 {1, 2},
                                 {2, 2},
                                 {3, 2}};

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
  int_sol.Add(coord_coverage({2, 1}, {2, 0}, {3, 0}), 1);
  int_sol.Add(coord_coverage({2, 0}, {3, 0}, {3, 1}), 1);
  int_sol.Add(coord_coverage({3, 0}, {3, 1}, {3, 2}), 1);
  int_sol.Add(coord_coverage({3, 1}, {3, 2}, {2, 2}), 1);
  int_sol.Add(coord_coverage({3, 2}, {2, 2}, {2, 1}), 1);
  int_sol.Add(coord_coverage({2, 2}, {2, 1}, {2, 0}), 1);

  turncostcover::Costs costs{2, 1};//tc, dc

  auto merge = apx::details::FindCheapestMerge(int_sol, costs, 0);

  ASSERT_TRUE(merge.IsFeasible());
  ASSERT_TRUE(merge.cov1.IsSimpleTurn(graph));
  ASSERT_TRUE(merge.cov2.IsSimpleTurn(graph));
  ASSERT_EQ(merge.cost, 2);

  apx::GreedyConnectAdjacentCycles(&int_sol, costs);
  ASSERT_TRUE(int_sol.GetNumComponents() == 1);
}