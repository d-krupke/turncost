//
// Created by Dominik Krupke, http://krupke.cc on 9/12/17.
//

#include "penalty_voronoi_separator.h"
namespace turncostcover {
namespace ip_formulation1 {

PenaltyVoronoiSeparator::PenaltyVoronoiSeparator(IpSolver *solver)
    : PenaltySimpleSeparator(solver) {}

Field
PenaltyVoronoiSeparator::GetMaxInternalField(
    const IntegralSolution &solution,
    const std::set<Field> &comp_fields)
const
{
  std::set<Field> covered_comp_fields;
  std::copy_if(comp_fields.begin(), comp_fields.end(),
               std::inserter(covered_comp_fields,
                             covered_comp_fields.begin()),
               [&](auto f) -> bool {
                 return solution.IsCovered(f);
               });
  assert(!covered_comp_fields.empty());
  return ip_formulation1::PenaltySimpleSeparator::GetMaxInternalField(
      solution,
      covered_comp_fields);
}

std::map<int, std::set<Field>>
PenaltyVoronoiSeparator::GetComponents(const IntegralSolution &solution)
const
{
  auto components = PenaltySimpleSeparator::GetComponents(solution);
  details::VoronoiExtender ve{solver_->graph_};
  ve.Extend(&components);
  return components;
}

}
}
