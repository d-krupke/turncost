//
// Created by Dominik Krupke, http://krupke.cc on 7/14/17.
//

#ifndef TURNCOSTCOVER_SOLUTION_H
#define TURNCOSTCOVER_SOLUTION_H

#include <stack>
#include <utility>
#include <map>
#include <vector>
#include "../grid_graph.h"
#include "../coverage.h"
#include "../costs.h"

namespace turncostcover {

class IntegralSolution {

 public:

  /// Only use for empty solution. A valid solution needs a graph!
  IntegralSolution() {}
  explicit IntegralSolution(const GridGraph &g,
                            std::map<Coverage, int> usages = {});
  IntegralSolution(const GridGraph &g, std::map<Coverage, double> usages);
  IntegralSolution(const GridGraph &g,
                   const std::vector<std::vector<turncostcover::Coordinate>> &cycles);

  void SetLowerBound(double lb) { lower_bound_ = lb; }
  double GetLowerBound() const { return lower_bound_; }

  // Create and modify solution
  void Add(Coverage cov, int amount = 1);
  void Add(const std::vector<Coverage> &cycle);

  void Remove(const std::vector<Coverage> &cycle);
  void Remove(Coverage coverage, int amount = 1);

  DistanceAndTurns Connect(Coverage cov_a, Coverage cov_b);

  bool IsFeasible() const;

  // Components
  std::vector<int>
  GetComponentsFieldIsIn(Field f, bool assume_unique = false)
  const;

  int GetComponentId(Coverage a) const;
  bool AreInSameComponent(Coverage a, Coverage b) const;

  DistanceAndTurns
  EvaluateConnectionCosts(Coverage cov_a, Coverage cov_b) const;

  int GetNumComponents() const;
  std::vector<std::vector<Coverage>> CollectCycles() const;

  // usage queries
  int GetUsageOfCoverage(Coverage c) const;
  bool IsCovered(Field f) const;
  std::vector<Coverage> GetUsedCoverages(Field f) const;
  std::vector<Coverage> GetUnusedCoverages(Field f) const;
  bool IsOnlyCoveredStraight(Field f) const;

  int SumOfCoverages() const;

  //Getter
  const GridGraph &GetGridGraph() const { assert(graph_); return *graph_; }

  CostUnit GetCoverageObjectiveValue(Costs costs);

  //basic
  bool operator==(const IntegralSolution &rhs) const;
  bool operator!=(const IntegralSolution &rhs) const;

 private:

  Coverage GetCoverage(Coordinate c1, Coordinate c2, Coordinate c3) const;
  void EvaluateComponentsIfNecessary() const;

  Coverage
  GetRandomCoverage(const std::map<Coverage, int> &remaining_coverages)
  const;

  std::vector<Coverage>
  ExtractCycle(std::map<Coverage, int> *remaining_coverages,
               Coverage random_coverage_contained_in_cycle)
  const;

  Field GetNextField(const std::vector<Coverage> &tour_stack) const;
  bool IsFeasible(Field a, Field b) const;
  std::vector<Coverage> GetReplacement(Coverage a, Coverage b) const;

 private:
  const GridGraph *graph_ = nullptr;
  std::map<Coverage, int> usages_ = {};
  double lower_bound_ = 0.0;

  mutable std::map<Coverage, int> components_ = {};
  mutable bool components_evaluted_ = false;
  mutable int num_comps_ = 0;
};
}

#endif //TURNCOSTCOVER_SOLUTION_H
