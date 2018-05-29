//
// Created by Dominik Krupke, http://krupke.cc on 9/14/17.
//

#include <gtest/gtest.h>
#include "../../problem_instance/grid_graph.h"
#include "../../problem_instance/costs.h"
#include "../general_cc_solver/ip_solver.h"
#include "../../apx/approximation.h"
#include "subset_simple_separator.h"
#include "subset_sufficient_separator.h"
#include "subset_voronoi_separation.h"
#include "../../apx/connect/connect.h"
#include "../integer_programming.h"

TEST(IpSubset, BaseTestEmpty)
{
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
  std::set<turncostcover::Field> subset;


  auto solution = turncostcover::ip::ComputeOptimalSubsetCycleCover(graph, subset, costs, 120);
  auto obj = std::lround(solution.GetCoverageObjectiveValue(costs));
  ASSERT_EQ(obj, 0);
}

TEST(IpSubset, BaseTest)
{
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
  std::set<turncostcover::Field> subset;
  for (const auto c: coords) subset.insert(graph.GetVertex(c));

  turncostcover::ip_formulation1::IpSolver solver{graph, subset, costs};
  auto apx_solution = apx::ApproximateSubsetCycleCover(graph, subset, costs);
  solver.AddSolution(apx_solution);
  solver.Solve();

  auto solution = turncostcover::ip::ComputeOptimalSubsetCycleCover(graph, subset, costs, 120);
  auto obj = solution.GetCoverageObjectiveValue(costs);

  ASSERT_EQ(std::lround(obj),
            8 * costs.dist_costs + 4 * costs.turn_costs);
}

TEST(IpSubset, DifficultConnectTest)
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
  std::set<turncostcover::Field> subset;
  for (const auto c: coords_subset) subset.insert(graph.GetVertex(c));

  turncostcover::ip_formulation1::IpSolver solver{graph, subset, costs};

  //Test HasToBeCovered
  for (const auto c: coords_subset) {
    ASSERT_TRUE(solver.HasToBeCovered(graph.GetVertex(c)));
  }
  for (const auto c: coords_connect) {
    ASSERT_FALSE(solver.HasToBeCovered(graph.GetVertex(c)));
  }
  auto solution = turncostcover::ip::ComputeOptimalSubsetTour(graph, subset, costs, 120);

  ASSERT_EQ(solution.GetNumComponents(), 1);
  ASSERT_EQ(solution.GetCoverageObjectiveValue(costs),
            (4 + 4 + 8 + 4) * costs.turn_costs
                + (10 + 10 + 16 + 2) * costs.dist_costs);
}