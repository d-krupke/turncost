//
// Created by Dominik Krupke, http://krupke.cc on 9/5/17.
//

#include <gtest/gtest.h>
#include "../problem_instance/coordinate.h"
#include "approximation.h"
#include "../problem_instance/save_and_load.h"
#include "../problem_instance/solution/visualization.h"

std::set<turncostcover::Field>
to_field_set(std::set<turncostcover::Coordinate> coords, turncostcover::GridGraph &graph) {
  std::set<turncostcover::Field> fields;
  for (auto c: coords) {
    fields.insert(graph.GetVertex(c));
  }
  return fields;
}

TEST(Approximation, ArkinWorstCase) {
  using namespace turncostcover;
  std::set<Coordinate> coords;
  for (int x = 0; x < 12; ++x) {
    coords.insert({x, 0});
    coords.insert({x, 1});
    coords.insert({x, 2});
    coords.insert({x, 3});
    coords.insert({x, 8});
    coords.insert({x, 9});
    coords.insert({x, 10});
    coords.insert({x, 11});
  }
  for (int y = 0; y < 12; ++y) {
    coords.insert({0, y});
    coords.insert({1, y});
    coords.insert({2, y});
    coords.insert({3, y});
    coords.insert({8, y});
    coords.insert({9, y});
    coords.insert({10, y});
    coords.insert({11, y});
  }
  turncostcover::GridGraph graph{coords};
  turncostcover::Costs costs;
  costs.turn_costs = 5;
  costs.dist_costs = 1;
  auto solution = apx::ApproximateCycleCover(graph, costs);
  //VisualizeIntegral(graph, solution, "/home/doms/arkin_worst_case.svg");

}

TEST(Approximation, SingleCycleAll) {
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
                                 {4, 1}};
  turncostcover::GridGraph graph{coords};
  turncostcover::Costs costs;
  costs.turn_costs = 5;
  costs.dist_costs = 1;

  //full
  auto full_cc = apx::ApproximateCycleCover(graph, costs);
  auto full_t = apx::ApproximateTour(graph, costs);
  ASSERT_TRUE(full_cc == full_t);

  //subset
  auto subset = to_field_set(coords, graph);
  auto subset_cc = apx::ApproximateSubsetCycleCover(graph, subset, costs);
  ASSERT_TRUE(full_cc == subset_cc); //because subset==all fields
  auto subset_t = apx::ApproximateSubsetTour(graph, subset, costs);
  ASSERT_TRUE(subset_cc == subset_t);
  ASSERT_TRUE(subset_t == full_t);

  //penalty
  std::map<turncostcover::Field, double> penalties;
  for (auto c: coords) penalties[graph.GetVertex(c)] = 100;
  auto penalty_cc = apx::ApproximatePenaltyCycleCover(graph, penalties, costs);
  ASSERT_TRUE(penalty_cc == full_cc);
  auto penalty_t = apx::ApproximatePenaltyTour(graph, penalties, costs);
  ASSERT_TRUE(penalty_t == full_t);
}

TEST(Approximation, PenaltyNoCoverage_Zero) {
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
                                 {4, 1}};
  turncostcover::GridGraph graph{coords};
  turncostcover::Costs costs;
  costs.turn_costs = 5;
  costs.dist_costs = 1;

  //penalty
  std::map<turncostcover::Field, double> penalties;
  for (auto c: coords) penalties[graph.GetVertex(c)] = 0.0;
  auto penalty_cc = apx::ApproximatePenaltyCycleCover(graph, penalties, costs);
  penalty_cc.GetNumComponents();
  ASSERT_EQ(penalty_cc.GetNumComponents(), 0);
  ASSERT_EQ(penalty_cc.GetCoverageObjectiveValue(costs), 0);
  auto penalty_t = apx::ApproximatePenaltyTour(graph, penalties, costs);
  ASSERT_EQ(penalty_t.GetNumComponents(), 0);
}

TEST(Approximation, PenaltyNoCoverage_NearlyZero) {
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
                                 {4, 1}};
  turncostcover::GridGraph graph{coords};
  turncostcover::Costs costs;
  costs.turn_costs = 5;
  costs.dist_costs = 1;

  //penalty
  std::map<turncostcover::Field, double> penalties;
  for (auto c: coords) penalties[graph.GetVertex(c)] = 0.01;
  auto penalty_cc = apx::ApproximatePenaltyCycleCover(graph, penalties, costs);
  ASSERT_EQ(penalty_cc.GetNumComponents(), 0);
  ASSERT_EQ(penalty_cc.GetCoverageObjectiveValue(costs), 0);
  auto penalty_t = apx::ApproximatePenaltyTour(graph, penalties, costs);
  ASSERT_EQ(penalty_t.GetNumComponents(), 0);
}

TEST(Approximation, Large) {
  std::string file = "./assets/fc_500_dense_3.gg";
  auto graph = turncostcover::LoadInstance(file);
  turncostcover::Costs costs;
  costs.turn_costs = 5;
  costs.dist_costs = 1;
  auto solution = turncostcover::apx::ApproximateCycleCover(graph, costs);
  std::cout << solution.GetCoverageObjectiveValue(costs) << std::endl;
}


TEST(Approximation, PenaltyLarge) {
  using namespace turncostcover;
  //std::string file = "/home/doms/Repositories/IBR-SVN/thesis-alg-2015-krupke-ma-robots/MasterThesis/ALENEX/Instances/rectangle_union/gg_rect_n10000_s10052_scale5_10052.csv";
  std::string file = "/home/doms/Repositories/IBR-SVN/thesis-alg-2015-krupke-ma-robots/MasterThesis/ALENEX/Instances/rectangle_union/gg_rect_n10000_s10004_scale1_10004.csv";
  auto graph = turncostcover::LoadInstance(file);
  turncostcover::Costs costs;
  costs.turn_costs = 500;
  costs.dist_costs = 1;

  //penalty
  std::map<turncostcover::Field, double> penalties;
  for (auto v: graph.GetVertices()) penalties[v] = 100;
  auto penalty_cc = apx::ApproximatePenaltyCycleCover(graph, penalties, costs);
  auto penalty_t = apx::ApproximatePenaltyTour(graph, penalties, costs);

  auto best_solution = penalty_cc.GetCoverageObjectiveValue(costs);
  double penalty = 0;
  for(auto v: graph.GetVertices()){
    if(!penalty_cc.IsCovered(v)) penalty+=penalties.at(v);
  }
  std::cout << "Cover "<<best_solution<<" Penalty"<<penalty<<std::endl;
  //VisualizeIntegral(graph, penalty_cc, "/home/doms/penalty_test_cc.svg");
  //VisualizeIntegral(graph, penalty_t, "/home/doms/penalty_test_t.svg");
}
