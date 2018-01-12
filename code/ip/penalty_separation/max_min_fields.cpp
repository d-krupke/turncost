//
// Created by Dominik Krupke, http://krupke.cc on 9/14/17.
//

#include "penalty_simple_separator.h"
#include "fc_separation/full_coverage_sufficient_separator.h"
#include "max_min_fields.h"

namespace turncostcover {

Field
ip_formulation1::details::FindMaxInternalField(
    const IpSolver &solver,
    const IntegralSolution &solution,
    const std::set<Field> &comp_fields)
{

  auto max_field_internal = max_element(
      comp_fields.begin(),
      comp_fields.end(),
      [&](auto v_a, auto v_b) -> bool {
        return solver.GetPenalty(v_a) < solver.GetPenalty(v_b);
      });
  assert(max_field_internal != comp_fields.end());
  assert(solution.IsCovered(*max_field_internal));
  return *max_field_internal;
}

Field
ip_formulation1::details::FindUsedMaxExternalField(
    const ip_formulation1::IpSolver &solver,
    const IntegralSolution &solution,
    const std::set<Field> &comp_fields)
{
  //most expensive field exterior

  const auto &gg = solution.GetGridGraph();
  std::vector<Field> used_exterior_fields;
  std::copy_if(gg.GetVertices().begin(), gg.GetVertices().end(),
               std::back_inserter(used_exterior_fields),
               [&](auto f) -> bool {
                 bool is_covered = solution.IsCovered(f);
                 bool is_external =
                     std::find(comp_fields.begin(), comp_fields.end(), f)
                         == comp_fields.end();
                 return is_covered && is_external;
               });

  auto max_field_external =
      std::max_element(used_exterior_fields.begin(), used_exterior_fields.end(),
                       [&](auto v_a, auto v_b) -> bool {
                         return solver.GetPenalty(v_a)
                             < solver.GetPenalty(v_b);
                       });
  assert(max_field_external != used_exterior_fields.end());
  assert(solution.IsCovered(*max_field_external));
  return *max_field_external;
}
}