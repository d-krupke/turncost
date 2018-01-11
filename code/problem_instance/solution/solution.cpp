//
// Created by Dominik Krupke, http://krupke.cc on 7/14/17.
//

#include "solution.h"
#include "../costs.h"

namespace turncostcover {

DistanceAndTurns
IntegralSolution::Connect(Coverage cov_a, Coverage cov_b)
{
  auto replacement = GetReplacement(cov_a, cov_b);
  DistanceAndTurns dat{0, 0};
  for (auto c: replacement) {
    Add(c);
    dat.turns += c.NumberOfUTurns(*graph_);
    dat.distance += 1;
  }
  Remove(cov_a);
  Remove(cov_b);
  dat.turns -= cov_a.NumberOfUTurns(*graph_);
  dat.turns -= cov_b.NumberOfUTurns(*graph_);
  dat.distance -= 2;
  return dat;

}

DistanceAndTurns
IntegralSolution::EvaluateConnectionCosts(Coverage cov_a, Coverage cov_b)
const
{
  auto replacement = GetReplacement(cov_a, cov_b);
  DistanceAndTurns dat{0, 0};
  for (auto c: replacement) {
    dat.turns += c.NumberOfUTurns(*graph_);
    dat.distance += 1;
  }
  dat.turns -= cov_a.NumberOfUTurns(*graph_);
  dat.turns -= cov_b.NumberOfUTurns(*graph_);
  dat.distance -= 2;
  return dat;
}

IntegralSolution::IntegralSolution(const GridGraph &g,
                                   std::map<Coverage, int> usages) :
    graph_{&g}, usages_{std::move(usages)}
{

}

IntegralSolution::IntegralSolution(const GridGraph &g,
                                   std::map<Coverage, double> usages)
    : graph_{&g}, usages_{}
{
  for (auto u: usages) {
    this->usages_[u.first] = (int) std::lround(u.second);
  }
}

bool
IntegralSolution::IsCovered(Field f)
const
{
  for (const auto &cov: ListAllPossibleCoverages(*graph_, f)) {
    const bool cov_used = GetUsageOfCoverage(cov) > 0;
    if (cov_used) { return true; }
  }
  return false;
}

std::vector<std::vector<Coverage>>
IntegralSolution::CollectCycles()
const
{
  std::map<Coverage, int> remaining_coverages = this->usages_;
  int variable_sum = SumOfCoverages();

  std::vector<std::vector<Coverage>> all_cycles;
  while (variable_sum > 0) {
    Coverage start = GetRandomCoverage(remaining_coverages);
    std::vector<Coverage> cycle = ExtractCycle(&remaining_coverages, start);
    variable_sum -= cycle.size();
    all_cycles.push_back(cycle);
  }

  return all_cycles;
}

void
IntegralSolution::Add(Coverage cov, int amount)
{
  assert(graph_ != nullptr); // you forgot to construct the solution with graph.
  components_evaluted_ = false;
  assert(amount >= 0);
  usages_[cov] += amount;
}

void
IntegralSolution::Remove(Coverage coverage, int amount)
{
  components_evaluted_ = false;
  assert(amount >= 0);
  usages_[coverage] -= amount;
  assert(usages_[coverage] >= 0);
}

bool
IntegralSolution::AreInSameComponent(Coverage a, Coverage b)
const
{
  assert(GetUsageOfCoverage(a) > 0 && GetUsageOfCoverage(b) > 0);
  EvaluateComponentsIfNecessary();
  return components_[a] == components_[b];
}

void
IntegralSolution::EvaluateComponentsIfNecessary()
const
{
  if (components_evaluted_) { return; }

  components_.clear();
  int comp_id = 1;
  for (auto comp: CollectCycles()) {
    for (auto c: comp) {
      components_[c] = comp_id;
    }
    comp_id += 1;
  }
  num_comps_ = comp_id - 1;
  components_evaluted_ = true;
}

int
IntegralSolution::GetUsageOfCoverage(Coverage c)
const
{
  if (usages_.count(c) == 0) { return 0; }
  return usages_.at(c);
}

int
IntegralSolution::GetNumComponents()
const
{
  if (!components_evaluted_) { EvaluateComponentsIfNecessary(); }
  return num_comps_;
}

int
IntegralSolution::GetComponentId(Coverage a)
const
{
  assert(GetUsageOfCoverage(a) > 0);
  EvaluateComponentsIfNecessary();
  return components_.at(a);
}

int
IntegralSolution::SumOfCoverages()
const
{
  int variable_sum = 0;
  for (auto c: usages_) variable_sum += c.second;
  return variable_sum;
}

Coverage
IntegralSolution::GetRandomCoverage(const std::map<Coverage,
                                                   int> &remaining_coverages)
const
{
  auto start =
      *std::max_element(remaining_coverages.begin(), remaining_coverages.end(),
                        [](const auto &a, const auto &b) {
                          return a.second < b.second;
                        });
  assert(start.second > 0);

  return start.first;
}

std::vector<Coverage>
IntegralSolution::ExtractCycle(std::map<Coverage, int> *remaining_coverages,
                               Coverage random_coverage_contained_in_cycle)
const
{
  std::vector<Coverage> cycle;
  std::vector<Coverage> tour_stack;

  tour_stack.push_back(random_coverage_contained_in_cycle);
  (*remaining_coverages)[random_coverage_contained_in_cycle] -= 1;

  while (!tour_stack.empty()) {
    Field predecessor_field = tour_stack.back().f;
    Field current_field = GetNextField(tour_stack);

    bool found_next = false;
    for (auto n_itp = boost::adjacent_vertices(current_field, *graph_);
         n_itp.first != n_itp.second; ++n_itp.first) {
      auto sucessor_field = *n_itp.first;
      Coverage var{predecessor_field, current_field, sucessor_field};
      if ((*remaining_coverages)[var] > 0) {
        (*remaining_coverages)[var] -= 1;
        tour_stack.push_back(var);
        found_next = true;
        break;
      }
    }
    if (!found_next) {
      assert(!tour_stack.empty());
      cycle.push_back(tour_stack.back());
      tour_stack.pop_back();
    }
  }
  assert(IsFeasible());
  return cycle;
}

Field
IntegralSolution::GetNextField(const std::vector<Coverage> &tour_stack)
const
{
  Coverage last_coverage = tour_stack.back();
  Field current_field;
  if (tour_stack.size() >= 2) {
    Field prepredecessor_field = (tour_stack.end() - 2)->f;
    current_field = last_coverage.DeduceOtherNeighbor(prepredecessor_field);
  } else {
    //Doesn't matter if n1 or n2
    current_field = tour_stack.back().n1;
  }

  return current_field;
}

bool
IntegralSolution::IsFeasible(Field a, Field b)
const
{
  int sum_a = 0;
  int sum_b = 0;

  for (auto n_itp = boost::adjacent_vertices(a, *graph_);
       n_itp.first != n_itp.second; ++n_itp.first) {
    const auto n = *n_itp.first;
    sum_a += GetUsageOfCoverage({b, a, n}) * (b == n ? 2 : 1);
  }
  for (auto n_itp = boost::adjacent_vertices(b, *graph_);
       n_itp.first != n_itp.second; ++n_itp.first) {
    const auto n = *n_itp.first;
    sum_b += GetUsageOfCoverage({a, b, n}) * (a == n ? 2 : 1);
  }

  return sum_a == sum_b;
}

bool
IntegralSolution::IsFeasible()
const
{
  for (auto e: graph_->GetEdges()) {
    auto v = boost::source(e, *graph_);
    auto w = boost::target(e, *graph_);
    if (!IsFeasible(v, w)) { return false; }
  }
  return true;
}

std::vector<Coverage>
IntegralSolution::GetUsedCoverages(Field f)
const
{
  std::vector<Coverage> used_coverages;
  for (auto c: ListAllPossibleCoverages(*graph_, f)) {
    if (GetUsageOfCoverage(c) > 0) {
      used_coverages.push_back(c);
    }
  }
  return used_coverages;
}

bool
IntegralSolution::operator==(const IntegralSolution &rhs)
const
{
  assert(graph_ == rhs.graph_);//don't compare solutions of different graphs
  for (auto u: usages_) {
    if (u.second == 0) { continue; }
    if (rhs.usages_.count(u.first) == 0) { return false; }
    if (u.second != rhs.usages_.at(u.first)) { return false; }
  }
  for (auto u: rhs.usages_) {
    if (u.second == 0) { continue; }
    if (usages_.count(u.first) == 0) { return false; }
    if (u.second != usages_.at(u.first)) { return false; }
  }
  return true;
}

bool
IntegralSolution::operator!=(const IntegralSolution &rhs)
const
{
  return !(rhs == *this);
}

CostUnit
IntegralSolution::GetCoverageObjectiveValue(Costs costs)
{
  CostUnit sum = 0;
  for (auto cp: usages_) {
    sum += cp.second * cp.first.Cost(costs, *graph_);
  }
  return sum;
}

std::vector<Coverage>
IntegralSolution::GetUnusedCoverages(Field f)
const
{
  std::vector<Coverage> unused_coverages;
  for (auto c: ListAllPossibleCoverages(*graph_, f)) {
    if (GetUsageOfCoverage(c) == 0) {
      unused_coverages.push_back(c);
    }
  }
  return unused_coverages;
}

std::vector<int>
IntegralSolution::GetComponentsFieldIsIn(Field f, bool assume_unique)
const
{
  std::vector<int> comps;
  for (const auto &cov: GetUsedCoverages(f)) {
    auto cmp = GetComponentId(cov);
    if (std::find(comps.begin(), comps.end(), cmp) == comps.end()) {
      comps.push_back(cmp);
      if (assume_unique) { break; }
    }
  }
  return comps;
}

bool
IntegralSolution::IsOnlyCoveredStraight(Field f)
const
{
  for (const auto &cov: GetUsedCoverages(f)) {
    if (cov.NumberOfUTurns(*graph_) > 0) { return false; }
  }
  return true;
}

void
IntegralSolution::Add(const std::vector<Coverage> &cycle)
{
  for (const auto c: cycle) Add(c);
}

void
IntegralSolution::Remove(const std::vector<Coverage> &cycle)
{
  for (const auto c: cycle) Remove(c);
}

std::vector<Coverage>
IntegralSolution::GetReplacement(Coverage a, Coverage b)
const
{
  if (a.f == b.f) {
    if (a.IsStraight(*graph_) && b.IsStraight(*graph_)) {
      assert(a != b);
      return {{a.n1, a.f, b.n1},
              {a.n2, b.f, b.n2}};
    } else {
      assert(a.n1 != b.n1 && a.n1 != b.n2 && a.n2 != b.n1 && a.n2 != b.n2);
      if (Coverage{a.n1, a.f, b.n1}.IsStraight(*graph_)) {
        return {{a.n1, a.f, b.n1},
                {a.n2, b.f, b.n2}};
      } else {
        return {{a.n1, a.f, b.n2},
                {a.n2, b.f, b.n1}};
      }
    }
  } else {
    assert(graph_->AreNeighbored(a.f, b.f));
    assert(!AreInSameComponent(a, b));
    Coverage aab1 = {a.n1, a.f, b.f};
    Coverage aab2 = {a.n2, a.f, b.f};
    Coverage bba1 = {b.n1, b.f, a.f};
    Coverage bba2 = {b.n2, b.f, a.f};
    return {aab1, aab2, bba1, bba2};
  }
}

IntegralSolution::IntegralSolution(const GridGraph &g,
                                   const std::vector<std::vector<Coordinate>> &cycles)
    : graph_{&g}
{
  for (const auto &cycle: cycles) {
    if (cycle.size() == 2) {
      Add(GetCoverage(cycle.at(0), cycle.at(1), cycle.at(0)));
      Add(GetCoverage(cycle.at(1), cycle.at(0), cycle.at(1)));
    } else {
      for (size_t i = 2; i < cycle.size(); ++i) {
        Add(GetCoverage(cycle.at(i - 2), cycle.at(i - 1), cycle.at(i)));
      }
      if (cycle.back() == cycle.front()) {
        Add(GetCoverage(cycle.at(cycle.size() - 2), cycle.at(0), cycle.at(1)));
      } else {
        Add(GetCoverage(cycle.at(cycle.size() - 1), cycle.at(0), cycle.at(1)));
        Add(GetCoverage(cycle.at(cycle.size() - 2),
                        cycle.at(cycle.size() - 1),
                        cycle.at(1)));
      }
    }
  }
  assert(IsFeasible());
}

Coverage IntegralSolution::GetCoverage(Coordinate c1,
                                       Coordinate c2,
                                       Coordinate c3)
const
{
  auto f1 = graph_->GetVertex(c1);
  auto f2 = graph_->GetVertex(c2);
  auto f3 = graph_->GetVertex(c3);
  assert(f1 != f2);
  assert(f2 != f3);
  assert(graph_->AreNeighbored(f1, f2));
  assert(graph_->AreNeighbored(f2, f3));
  return {f1, f2, f3};
}
}