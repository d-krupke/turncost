//
// Created by Dominik Krupke, http://krupke.cc on 8/18/17.
//

#include <gtest/gtest.h>
#include "../../../problem_instance/coordinate.h"
#include "../../../problem_instance/solution/solution.h"
#include "component_mst.h"
#include "path_to_cycle.h"
#include "greedy_connect.h"

TEST(ComponentMst, BaseTest)
{
  using namespace turncostcover;
  using namespace apx;
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

  ASSERT_TRUE(int_sol.IsFeasible());

  //ids of components
  int comp_a = int_sol.GetComponentsFieldIsIn(graph.GetVertex({1, 0}), true).front();
  int comp_b = int_sol.GetComponentsFieldIsIn(graph.GetVertex({4, 0}), true).front();
  int comp_c = int_sol.GetComponentsFieldIsIn(graph.GetVertex({4, 2}), true).front();

  //Solve mst
  turncostcover::Costs costs{3, 1};
  apx::details::ComponentMst mst{int_sol, costs};
  mst.Solve();

  //check if it returns the right edges
  auto edges = mst.GetEdges();
  ASSERT_EQ(edges.size(), 2);
  ASSERT_TRUE(std::find(edges.begin(), edges.end(), ComponentEdge{comp_a, comp_b}) != edges.end());
  ASSERT_TRUE(std::find(edges.begin(), edges.end(), ComponentEdge{comp_b, comp_c}) != edges.end());
  ASSERT_TRUE(std::find(edges.begin(), edges.end(), ComponentEdge{comp_a, comp_c}) == edges.end());

  //check the paths
  std::vector<turncostcover::Field> path_ab = std::find(edges.begin(), edges.end(), ComponentEdge{comp_a, comp_b})->ab_path;
  ASSERT_EQ(graph.GetCoordinate(path_ab.at(0)), turncostcover::Coordinate(1, 0));
  ASSERT_EQ(graph.GetCoordinate(path_ab.at(1)), turncostcover::Coordinate(2, 0));
  ASSERT_EQ(graph.GetCoordinate(path_ab.at(2)), turncostcover::Coordinate(3, 0));

  std::vector<turncostcover::Field> path_bc = std::find(edges.begin(), edges.end(), ComponentEdge{comp_b, comp_c})->ab_path;
  ASSERT_EQ(graph.GetCoordinate(path_bc.at(0)), turncostcover::Coordinate(4, 0));
  ASSERT_EQ(graph.GetCoordinate(path_bc.at(1)), turncostcover::Coordinate(4, 1));
  ASSERT_EQ(graph.GetCoordinate(path_bc.at(2)), turncostcover::Coordinate(4, 2));

  //add connecting cycles
  std::vector<turncostcover::Coverage> cycle_ab =
      apx::PathToCycle(path_ab);
  std::vector<turncostcover::Coverage> cycle_bc =
      apx::PathToCycle(path_bc);
  int_sol.Add(cycle_ab);
  int_sol.Add(cycle_bc);
  ASSERT_TRUE(int_sol.IsFeasible());

  apx::GreedyConnectAdjacentCycles(&int_sol, costs);
  ASSERT_TRUE(int_sol.IsFeasible());
  ASSERT_EQ(int_sol.GetCoverageObjectiveValue(costs), 14 * costs.dist_costs + 6 * costs.turn_costs);

}

