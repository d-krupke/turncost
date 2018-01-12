//
// Created by Dominik Krupke, http://krupke.cc on 9/11/17.
//

#include "fc_separation/full_coverage_sufficient_separator.h"
#include "penalty_simple_separator.h"
#include "max_min_fields.h"
namespace turncostcover {
namespace ip_formulation1 {
PenaltySimpleSeparator::PenaltySimpleSeparator(IpSolver *solver)
    : FullCoverageSimpleSeparator(solver) {}

size_t
PenaltySimpleSeparator::CreateConstraint(const IntegralSolution &solution,
                                         const std::set<Field> &comp_fields)
const
{
  IloExpr constr_expr = CreateLeaveComponentExpression(solution, comp_fields);

  //most expensive fields
  auto max_field_internal = GetMaxInternalField(solution, comp_fields);
  auto max_field_external = GetUsedMaxExternalField(solution, comp_fields);

  if (solver_->HasPenaltyVariable(max_field_internal)) {
    constr_expr += 2 * solver_->GetPenaltyVariable(max_field_internal);
  } else {
    if (solver_->GetPenalty(max_field_internal) == 0) {
      constr_expr.end();
      std::cout << "Skipping separator for void component." << std::endl;
      return 0;
    }
  }
  if (solver_->HasPenaltyVariable(max_field_external)) {
    constr_expr += 2 * solver_->GetPenaltyVariable(max_field_external);
  } else {
    assert(solver_->GetPenalty(max_field_external) != 0);
  }

  this->solver_->AddToModel(IloRange(this->solver_->cplex_env_,
                                    2,
                                    constr_expr,
                                    IloInfinity));
  return 1;
}

Field
PenaltySimpleSeparator::GetMaxInternalField(
    const IntegralSolution &solution,
    const std::set<Field> &comp_fields)
const
{
  return details::FindMaxInternalField(*solver_, solution, comp_fields);
}

Field
PenaltySimpleSeparator::GetUsedMaxExternalField(
    const IntegralSolution &solution,
    const std::set<Field> &comp_fields)
const
{
  return details::FindUsedMaxExternalField(*solver_, solution, comp_fields);
}

}
}

