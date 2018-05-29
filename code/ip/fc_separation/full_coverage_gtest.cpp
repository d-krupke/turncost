//
// Created by Dominik Krupke, http://krupke.cc on 9/14/17.
//

#include <gtest/gtest.h>
#include "../../problem_instance/grid_graph.h"
#include "../general_cc_solver/ip_solver.h"
#include "../../apx/approximation.h"
#include "../../problem_instance/save_and_load.h"
#include "../integer_programming.h"

TEST(FcIp, BaseTest) {
  using namespace turncostcover;
  std::set<turncostcover::Coordinate> coords = {{0, 0},
                                                {1, 0},
                                                {2, 0},
                                                {3, 0},
                                                {4, 0}};
  turncostcover::GridGraph graph{coords};
  turncostcover::Costs costs;
  costs.turn_costs = 5;
  costs.dist_costs = 1;

  turncostcover::ip_formulation1::IpSolver solver{graph, costs};
  auto apx_solution = apx::ApproximateTour(graph, costs);
  solver.AddSolution(apx_solution);
  solver.Solve();
  ASSERT_EQ(std::lround(solver.GetObjectiveValue()), 8 * costs.dist_costs + 4 * costs.turn_costs);
}

TEST(FcIp, Large) {
  std::string file = "./assets/fc_500_dense_3.gg";
  auto graph = turncostcover::LoadInstance(file);
  turncostcover::Costs costs;
  costs.turn_costs = 5;
  costs.dist_costs = 1;
  auto solution = turncostcover::ip::ComputeOptimalTour(graph, costs, 120);

  ASSERT_EQ(solution.GetNumComponents(), 1);
  ASSERT_EQ(solution.GetCoverageObjectiveValue(costs), 998);
  ASSERT_EQ(solution.GetCoverageObjectiveValue(costs),
            solution.GetLowerBound());
}

TEST(FcIp, DeAssis) {
  std::string file = "/home/doms/Work/master_thesis/0998-10-91-149.in";
  auto graph = turncostcover::LoadInstance(file);
  ASSERT_TRUE(graph.IsConnected());
  turncostcover::Costs costs;
  costs.turn_costs = 1;
  costs.dist_costs = 0;
  auto solution = turncostcover::ip::ComputeOptimalTour(graph, costs, 120);

  ASSERT_EQ(solution.GetNumComponents(), 1);
  ASSERT_EQ(solution.GetCoverageObjectiveValue(costs), 32);//just for testing, doesn't match
  ASSERT_EQ(solution.GetCoverageObjectiveValue(costs),
            solution.GetLowerBound());
}