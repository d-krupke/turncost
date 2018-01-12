//
// Created by Dominik Krupke, http://krupke.cc on 9/14/17.
//


#include <gtest/gtest.h>
#include "../../problem_instance/costs.h"
#include "../general_cc_solver/ip_solver.h"
#include "../../apx/approximation.h"
#include "../../problem_instance/save_and_load.h"
#include "penalty_voronoi_separator.h"
#include "penalty_sufficient_separator.h"
#include "../../apx/connect/connect.h"


TEST(IpPenalty, Large)
{
  std::string file = "./assets/fc_500_dense_3.gg";
  auto graph = turncostcover::LoadInstance(file);
  turncostcover::Costs costs;
  costs.turn_costs = 5;
  costs.dist_costs = 1;

  turncostcover::ip_formulation1::IpSolver solver{graph, costs};
  auto apx_solution = apx::ApproximateTour(graph, costs);
  turncostcover::CostUnit apx_obj =
      apx_solution.GetCoverageObjectiveValue(costs);
  std::cout << "APX T OBJ = " << apx_obj << std::endl;
  auto apx_cc_solution = apx::ApproximateCycleCover(graph, costs);
  turncostcover::CostUnit apx_cc_obj =
      apx_solution.GetCoverageObjectiveValue(costs);
  std::cout << "APX CC OBJ = " << apx_cc_obj << std::endl;
  solver.AddSolution(apx_solution, "APX-T");
  solver.AddSolution(apx_cc_solution, "APX-CC");
  solver.Solve();

  turncostcover::ip_formulation1::PenaltySimpleSeparator pss{&solver};
  turncostcover::ip_formulation1::PenaltySufficientSeparator pSufS{&solver};
  turncostcover::ip_formulation1::PenaltyVoronoiSeparator pvs{&solver};

  auto solution = solver.GetSolution();
  while (pss.Separate(solution) + pSufS.Separate(solution) + pvs.Separate(
      solution) > 0) {
    solver.AddSolution(apx_solution, "APX-T");
    solver.Solve();
    solution = solver.GetSolution();
  }
  ASSERT_EQ(solver.GetObjectiveValue(),
            solver.GetSolution().GetCoverageObjectiveValue(costs));
  ASSERT_EQ(solver.GetSolution().GetNumComponents(), 1);
  ASSERT_EQ(solver.GetSolution().GetCoverageObjectiveValue(costs), 998);
}


TEST(IpPenalty, DifficultConnectTest)
{
  using namespace turncostcover;
  std::set<turncostcover::Coordinate> coords_subset = {{0, 0},
                                            {0, 1},
                                            {0, 2},
                                            {0, 3},
                                            {0, 4},
                                            {1, 0},
                                            {1, 1},
                                            {1, 2},
                                            {1, 3},
                                            {1, 4},
                                            {8, 0},
                                            {8, 1},
                                            {8, 2},
                                            {8, 3},
                                            {8, 4},
                                            {9, 0},
                                            {9, 1},
                                            {9, 2},
                                            {9, 3},
                                            {9, 4}
  };
  std::set<turncostcover::Coordinate> coords_connect = {{2, 2},
                                             {3, 2},
                                             {3, 3},
                                             {4, 3},
                                             {4, 4},
                                             {5, 3},
                                             {5, 2},
                                             {6, 2},
                                             {7, 2}};
  std::set<turncostcover::Coordinate> coords;
  coords.insert(coords_subset.begin(), coords_subset.end());
  coords.insert(coords_connect.begin(), coords_connect.end());

  turncostcover::GridGraph graph{coords};
  turncostcover::Costs costs;
  costs.turn_costs = 5;
  costs.dist_costs = 1;
  std::map<turncostcover::Field, double> penalties;
  for (const auto c: coords_subset) penalties[graph.GetVertex(c)] = 100;
  for (const auto c: coords_connect) penalties[graph.GetVertex(c)] = 0;

  turncostcover::ip_formulation1::IpSolver solver{graph, penalties, costs};

  //Test HasToBeCovered
  for (const auto c: coords_subset) ASSERT_TRUE(solver.HasToBeCovered(graph.GetVertex(
        c)));
  for (const auto c: coords_connect) ASSERT_FALSE(solver.HasToBeCovered(graph.GetVertex(
        c)));


  auto apx_solution_t = apx::ApproximatePenaltyTour(graph, penalties, costs);
  std::cout << "APX-T=" << apx_solution_t.GetCoverageObjectiveValue(costs) << std::endl;
  auto apx_solution_cc =
      apx::ApproximatePenaltyCycleCover(graph, penalties, costs);
  std::cout << "APX-CC=" << apx_solution_cc.GetCoverageObjectiveValue(costs) << std::endl;
  solver.AddSolution(apx_solution_t, "apx-t");
  solver.AddSolution(apx_solution_cc, "apx-cc");
  solver.Solve();

  turncostcover::ip_formulation1::PenaltySimpleSeparator s1{&solver};
  turncostcover::ip_formulation1::PenaltySufficientSeparator s2{&solver};
  turncostcover::ip_formulation1::PenaltyVoronoiSeparator s3{&solver};

  auto solution = solver.GetSolution();
  while (solution.GetNumComponents() > 1) {
    std::cout << "Added " << s1.Separate(solution) << " simple separation constraints" << std::endl;
    std::cout << "Added " << s2.Separate(solution) << " sufficient separation constraints" << std::endl;
    std::cout << "Added " << s3.Separate(solution) << " voronoi separation constraints" << std::endl;
    auto connected_solution = solution;
    apx::connect::ConnectDistantCycles(&solution, costs);
    solver.AddSolution(connected_solution, "connected");
    solver.AddSolution(apx_solution_t, "apx-t");
    solver.Solve();
    solution = solver.GetSolution();
  }
  ASSERT_EQ(solution.GetNumComponents(), 1);
  ASSERT_EQ(solution.GetCoverageObjectiveValue(costs),
            (4 + 4 + 8 + 4) * costs.turn_costs + (10 + 10 + 16 + 2) * costs.dist_costs);
}