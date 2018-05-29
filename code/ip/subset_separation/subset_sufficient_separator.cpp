//
// Created by Dominik Krupke, http://krupke.cc on 9/11/17.
//

#include "./subset_sufficient_separator.h"

namespace turncostcover {
namespace ip_formulation1 {

Field
SubsetSufficientSeparation::FindMinCoveredFieldInCycle(
    const std::set<Field> &fields_in_cycle,
    const IntegralSolution &solution)
const
{
  assert(!fields_in_cycle.empty());
  std::set<Field> f; // fields in cycle and subset
  std::set_intersection(fields_in_cycle.begin(),
                        fields_in_cycle.end(),
                        subset.begin(),
                        subset.end(),
                        std::inserter(f, f.begin()));
  if (f.empty()) { throw MinCoveredFieldNotFoundException{}; }

  assert(f.size() != subset.size());
  //TODO: This case can actually happen for non-optimal solutions and should be handled.

  using FCSS = FullCoverageSufficientSeparator;
  auto min_covered_field = FCSS::FindMinCoveredFieldInCycle(f, solution);
  return min_covered_field;
}
}
  }