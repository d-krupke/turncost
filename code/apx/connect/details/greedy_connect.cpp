//
// Created by Dominik Krupke, http://krupke.cc on 7/11/17.
//

#include "greedy_connect.h"
namespace turncostcover {
namespace apx {

void
GreedyConnectAdjacentCycles(IntegralSolution *solution,
                            Costs costs)
{
  while (true) {
    auto merge = details::FindCheapestMerge(*solution, costs);
    if (!merge.IsFeasible()) { break; }
    auto nc1 = solution->GetNumComponents();
    solution->Connect(merge.cov1, merge.cov2);
    assert(solution->GetNumComponents() < nc1);
    assert(solution->IsFeasible());

  }
}

namespace details {

CycleMerge
FindCheapestMergeOfCoverage(Coverage cov,
                            const IntegralSolution &solution,
                            Costs costs)
{
  CycleMerge min_merge;

  //Same field connections
  for (auto cov2: solution.GetUsedCoverages(cov.f)) {
    if (solution.AreInSameComponent(cov, cov2)) { continue; }
    auto cost = solution.EvaluateConnectionCosts(cov, cov2) * costs;
    if (cost < min_merge.cost) {
      min_merge = {cov, cov2, cost};
    }
  }

  //neighbored coverages
  for (auto n: solution.GetGridGraph().GetNeighbors(cov.f)) {
    auto coverages_of_n = solution.GetUsedCoverages(n);

    for (auto c_n: coverages_of_n) {
      if (solution.AreInSameComponent(cov, c_n)) { continue; }
      auto cost = solution.EvaluateConnectionCosts(cov, c_n) * costs;

      if (cost < min_merge.cost) {
        min_merge = {cov, c_n, cost};
      }
    }
  }
  return min_merge;
};

CycleMerge
FindCheapestMerge(const IntegralSolution &solution,
                  Costs costs,
                  CostUnit return_immediately_if_leq)
{
  CycleMerge min_merge;

  for (auto v: solution.GetGridGraph().GetVertices()) {
    for (auto cov: solution.GetUsedCoverages(v)) {
      auto cheapest_cov = FindCheapestMergeOfCoverage(cov, solution, costs);
      if (!cheapest_cov.IsFeasible()) { continue; }

      if (cheapest_cov.cost < return_immediately_if_leq) {
        return cheapest_cov;
      }

      if (cheapest_cov.cost < min_merge.cost) {
        min_merge = cheapest_cov;
      }
    }
  }
  return min_merge;
};
}
}
}