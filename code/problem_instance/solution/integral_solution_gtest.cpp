#include <gtest/gtest.h>
#include "../grid_graph.h"
#include "solution.h"


TEST(IntegralSolution, CmpId)
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

  ASSERT_TRUE(int_sol.IsFeasible());
  ASSERT_EQ(int_sol.GetNumComponents(), 2);

  ASSERT_EQ(int_sol.GetComponentId(coord_coverage({0, 1}, {0, 0}, {1, 0})), 1);
  ASSERT_EQ(int_sol.GetComponentsFieldIsIn(graph.GetVertex({0, 0})).front(), 1);
  ASSERT_EQ(int_sol.GetComponentsFieldIsIn(graph.GetVertex({0, 0})).size(), 1);
  ASSERT_EQ(int_sol.GetComponentId(coord_coverage({2, 1}, {2, 0}, {3, 0})), 2);
}


TEST(IntegralSolution, Connect)
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

  ASSERT_TRUE(int_sol.IsFeasible());
  ASSERT_EQ(int_sol.GetNumComponents(), 2);

  ASSERT_TRUE(int_sol.AreInSameComponent(coord_coverage({0, 1}, {0, 0}, {1, 0}),
                                         coord_coverage({1, 1}, {1, 2}, {0, 2})));
  ASSERT_FALSE(int_sol.AreInSameComponent(coord_coverage({0, 1},
                                                         {0, 0},
                                                         {1, 0}),
                                          coord_coverage({2, 0},
                                                         {3, 0},
                                                         {3, 1})));

  auto c = int_sol.Connect(coord_coverage({0, 0}, {1, 0}, {1, 1}),
                           coord_coverage({2, 1}, {2, 0}, {3, 0}));
  ASSERT_EQ(c.turns, 0);
  ASSERT_EQ(c.distance, 2);
  ASSERT_EQ(int_sol.GetNumComponents(), 1);
  ASSERT_TRUE(int_sol.AreInSameComponent(coord_coverage({0, 1}, {0, 0}, {1, 0}),
                                         coord_coverage({2, 0}, {3, 0}, {3, 1})));
  ASSERT_TRUE(int_sol.AreInSameComponent(coord_coverage({0, 2}, {0, 1}, {0, 0}),
                                         coord_coverage({2, 0}, {3, 0}, {3, 1})));

  ASSERT_TRUE(int_sol.IsFeasible());

  //Remove a random coverage which renders the solution infeasible.
  turncostcover::Coverage cov = coord_coverage({0, 1}, {0, 0}, {1, 0});
  ASSERT_EQ(int_sol.GetUsageOfCoverage(cov), 1);
  int_sol.Remove(cov);
  ASSERT_FALSE(int_sol.IsFeasible());

}

TEST(IntegralSolution, AssertFeasibility)
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

  //empty
  ASSERT_TRUE(int_sol.IsFeasible());
  //left cycle
  int_sol.Add(coord_coverage({0, 1}, {0, 0}, {1, 0}), 1);
  ASSERT_FALSE(int_sol.IsFeasible());
  int_sol.Add(coord_coverage({0, 0}, {1, 0}, {1, 1}), 1);
  ASSERT_FALSE(int_sol.IsFeasible());
  int_sol.Add(coord_coverage({1, 0}, {1, 1}, {1, 2}), 1);
  ASSERT_FALSE(int_sol.IsFeasible());
  int_sol.Add(coord_coverage({1, 1}, {1, 2}, {0, 2}), 1);
  ASSERT_FALSE(int_sol.IsFeasible());
  int_sol.Add(coord_coverage({1, 2}, {0, 2}, {0, 1}), 1);
  ASSERT_FALSE(int_sol.IsFeasible());
  int_sol.Add(coord_coverage({0, 2}, {0, 1}, {0, 0}), 1);
  ASSERT_TRUE(int_sol.IsFeasible());

  //right cycle
  int_sol.Add(coord_coverage({1, 1}, {1, 0}, {2, 0}), 1);
  ASSERT_FALSE(int_sol.IsFeasible());
  int_sol.Add(coord_coverage({1, 0}, {2, 0}, {2, 1}), 1);
  ASSERT_FALSE(int_sol.IsFeasible());
  int_sol.Add(coord_coverage({2, 0}, {2, 1}, {2, 2}), 1);
  ASSERT_FALSE(int_sol.IsFeasible());
  int_sol.Add(coord_coverage({2, 1}, {2, 2}, {1, 2}), 1);
  ASSERT_FALSE(int_sol.IsFeasible());
  int_sol.Add(coord_coverage({2, 2}, {1, 2}, {1, 1}), 1);
  ASSERT_FALSE(int_sol.IsFeasible());
  int_sol.Add(coord_coverage({1, 2}, {1, 1}, {1, 0}), 1);
  ASSERT_TRUE(int_sol.IsFeasible());

}

TEST(IntegralSolution, GetUsedCoverages)
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

  auto used_covs = int_sol.GetUsedCoverages(graph.GetVertex({1, 0}));
  ASSERT_TRUE(std::find(used_covs.begin(), used_covs.end(), coord_coverage({1, 1}, {1, 0}, {2, 0})) != used_covs.end());
  ASSERT_TRUE(std::find(used_covs.begin(), used_covs.end(), coord_coverage({0, 0}, {1, 0}, {1, 1})) != used_covs.end());
  ASSERT_FALSE(
          std::find(used_covs.begin(), used_covs.end(), coord_coverage({0, 1}, {0, 0}, {1, 0})) != used_covs.end());
  ASSERT_EQ(used_covs.size(), 2);
}