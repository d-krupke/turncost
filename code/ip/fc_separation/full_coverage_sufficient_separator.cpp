//
// Created by Dominik Krupke (http://krupke.cc) on 1/12/18.
//
#include "full_coverage_sufficient_separator.h"
namespace turncostcover {
namespace ip_formulation1 {

void
FullCoverageSufficientSeparator::CoverOnAConnectingWay(
    const IntegralSolution &solution,
    IloExpr *constr,
    Field v)
const
{
  bool is_only_covered_straight = solution.IsOnlyCoveredStraight(v);

  for (auto var_index: solution.GetUnusedCoverages(v)) {
    if (var_index.IsUTurn()) { continue; } //skip u-turn variables
    if (is_only_covered_straight && var_index.IsStraight(solver_->graph_)) {
      continue;
    }

    (*constr) += solver_->GetCoverageVariable(var_index);
  }
}

std::unique_ptr<IloRange>
FullCoverageSufficientSeparator::CreateConstraint(
    const std::vector<Coverage> &cycle,
    const std::vector<std::vector<Coverage>> &cycles,
    const IntegralSolution &solution)
const
{
  try {
    std::set<Field> fields_in_cycle = GetFields(cycle);
    auto min_covered_field =
        FindMinCoveredFieldInCycle(fields_in_cycle, solution);

    //if the minimum covered field contains multiple cycles (not possible in optimal solution), merge cycles
    if (solution.GetComponentsFieldIsIn(min_covered_field).size() > 1) {
      fields_in_cycle = MergeComponents(solution, min_covered_field, cycles);
      assert(false);
    }

    IloExpr constr =
        GetCoverageExpression(solution, fields_in_cycle, min_covered_field);
    return std::make_unique<IloRange>(solver_->cplex_env_, 1, constr, IloInfinity);
  } catch (MinCoveredFieldNotFoundException e) {
    std::cout
        << "Could not find min covered field in component. Skipping component."
        << std::endl;
    return std::unique_ptr<IloRange>{nullptr};
  }
}

void
FullCoverageSufficientSeparator::CoverBySomeUnusedCoverage(
    const Field field,
    const IntegralSolution &solution,
    IloExpr *constr)
const
{
  for (auto var_index: solution.GetUnusedCoverages(field)) {
    (*constr) += solver_->GetCoverageVariable(var_index);
  }
}

IloExpr
FullCoverageSufficientSeparator::GetCoverageExpression(
    const IntegralSolution &solution,
    const std::set<Field> &fields_in_cycle,
    Field min_covered_field)
const
{
  IloExpr constr{this->solver_->cplex_env_};
  this->CoverBySomeUnusedCoverage(min_covered_field, solution, &constr);

  //other fields
  for (auto v: fields_in_cycle) {
    if (v == min_covered_field) { continue; }

    this->CoverOnAConnectingWay(solution, &constr, v);
  }
  return constr;
}

std::set<Field>
FullCoverageSufficientSeparator::GetFields(const std::vector<Coverage> &coverages)
const
{
  std::set<Field> fields;
  for (const auto &c: coverages) {
    fields.insert(c.f);
  }
  return fields;
}

std::set<Field>
FullCoverageSufficientSeparator::MergeComponents(
    const IntegralSolution &solution,
    Field f,
    const std::vector<std::vector<Coverage>> &cycles)
const
{
  std::cerr << "Merging cycles for sufficient seperation constraint. "
            << "If this happens often, probably the code is buggy."
            << std::endl;
  std::set<Field> fields;
  auto comp_ids = solution.GetComponentsFieldIsIn(f);

  for (const auto &other_cycle: cycles) {
    auto cmp_id = solution.GetComponentId(other_cycle.front());
    if (std::find(comp_ids.begin(), comp_ids.end(), cmp_id) != comp_ids.end()) {
      auto field_of_other_cycle = GetFields(other_cycle);
      fields.insert(field_of_other_cycle.begin(), field_of_other_cycle.end());
    }
  }

  return fields;
}

Field
FullCoverageSufficientSeparator::FindMinCoveredFieldInCycle(
    const std::set<Field> &cycle,
    const IntegralSolution &solution)
const
{
  assert(!cycle.empty());
  auto lambda = [&](Field a, Field b) -> bool {
    return solution.GetComponentsFieldIsIn(a).size()
        < solution.GetComponentsFieldIsIn(b).size();
  };
  auto min_field_it = std::min_element(cycle.begin(), cycle.end(), lambda);

  assert(min_field_it != cycle.end());
  return *min_field_it;
}

}
}