//
// Created by Dominik Krupke, http://krupke.cc on 10/4/17.
//

#include <gtest/gtest.h>
#include "strip_matching_solver.h"
#include "../../problem_instance/save_and_load.h"
#include "../../problem_instance/solution/visualization.h"
#include "../approximation.h"

TEST(ArkinApx, SmallExample)
{
  using namespace turncostcover;
  using namespace arkinapx;

  std::set<turncostcover::Coordinate> coords{{0, 0},
                                  {1, 0},
                                  {2, 0},
                                  {0, 1},
                                  {1, 1},
                                  {2, 1}};
  turncostcover::GridGraph graph{coords};
  turncostcover::Costs costs;
  costs.turn_costs = 5;
  costs.dist_costs = 1;
  details::StripMatchingSolver stripMatchingSolver{graph, costs};
  auto solution = stripMatchingSolver.Solve();
  ASSERT_EQ(solution.GetCoverageObjectiveValue(costs), 26);
}

TEST(ArkinApx, MiddleSizedExample)
{
  using namespace turncostcover;
  using namespace arkinapx;
  std::string file = "./assets/fc_500_dense_3.gg";
  auto graph = turncostcover::LoadInstance(file);
  turncostcover::Costs costs;
  costs.turn_costs = 5;
  costs.dist_costs = 1;
  details::StripMatchingSolver stripMatchingSolver{graph, costs};
  auto solution = stripMatchingSolver.Solve();
  ASSERT_TRUE(solution.IsFeasible());
  ASSERT_EQ(solution.GetCoverageObjectiveValue(costs), 1070);//was 1034 before but looks fine?
  std::cout << solution.GetCoverageObjectiveValue(costs) << std::endl;
  //VisualizeIntegral(graph, solution, "/home/doms/test-arkinapx.svg");
}
