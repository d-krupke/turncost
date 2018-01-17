//
// Created by Dominik Krupke, http://krupke.cc on 8/25/17.
//

#include <gtest/gtest.h>
#include "../connect.h"


TEST(ConnectPenaltyCycleCover, BaseTest)
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

  std::map<turncostcover::Field, double> penalties;
  penalties[graph.GetVertex({1, 0})] = 100;
  penalties[graph.GetVertex({0, 0})] = 100;
  penalties[graph.GetVertex({4, 0})] = 100;
  penalties[graph.GetVertex({3, 0})] = 100;
  penalties[graph.GetVertex({4, 2})] = 100;
  penalties[graph.GetVertex({4, 3})] = 100;

  ASSERT_EQ(int_sol.GetNumComponents(), 3);
  turncostcover::Costs costs;
  costs.turn_costs = 3;
  costs.dist_costs = 1;
  apx::ConnectOrRemoveCycles(&int_sol, costs, penalties);

  ASSERT_EQ(int_sol.GetNumComponents(), 1);
  ASSERT_EQ(int_sol.GetCoverageObjectiveValue(costs), 14 * costs.dist_costs + 6 * costs.turn_costs);
}


TEST(ConnectPenaltyCycleCover, BaseTestRemove)
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

  std::map<turncostcover::Field, double> penalties;
  penalties[graph.GetVertex({1, 0})] = 100;
  penalties[graph.GetVertex({0, 0})] = 100;
  penalties[graph.GetVertex({4, 0})] = 100;
  penalties[graph.GetVertex({3, 0})] = 100;
  penalties[graph.GetVertex({4, 2})] = 0;
  penalties[graph.GetVertex({4, 3})] = 0;

  ASSERT_EQ(int_sol.GetNumComponents(), 3);
  turncostcover::Costs costs;
  costs.turn_costs = 3;
  costs.dist_costs = 1;
  apx::ConnectOrRemoveCycles(&int_sol, costs, penalties);

  ASSERT_EQ(int_sol.GetNumComponents(), 1);
  ASSERT_EQ(int_sol.GetCoverageObjectiveValue(costs), 8 * costs.dist_costs + 4 * costs.turn_costs);
}