//
// Created by Dominik Krupke, http://krupke.cc on 9/11/17.
//

#include "penalty_sufficient_separator.h"
namespace turncostcover {
namespace ip_formulation1 {
PenaltySufficientSeparator::PenaltySufficientSeparator(IpSolver *solver)
    : FullCoverageSufficientSeparation(solver) {}
size_t PenaltySufficientSeparator::CreateConstraint(
    const std::vector<Coverage> &cycle,
    const std::vector<std::vector<Coverage>> &cycles,
    const IntegralSolution &solution) const
{

  std::set<Field> fic = GetFields(cycle);
  auto mcf = FindMinCoveredFieldInCycle(fic, solution);

  // if the minimum covered field contains multiple cycles (not possible in
  // optimal solution), merge cycles
  if (solution.GetComponentsFieldIsIn(mcf).size() > 1) {
    fic = MergeComponents(solution, mcf, cycles);
    assert(false);
  }

  IloExpr constr_expr = GetCoverageExpression(solution, fic, mcf);
  // most expensive fields
  auto mfi = details::FindMaxInternalField(*solver_, solution, fic);
  auto mfe = details::FindUsedMaxExternalField(*solver_, solution, fic);
  if (solver_->HasPenaltyVariable(mfi)) {
    constr_expr += 2 * solver_->GetPenaltyVariable(mfi);
  } else {
    if (!solver_->HasToBeCovered(mfi)) {
      std::cout << "Skipping void component" << std::endl;
    }
  }
  if (solver_->HasPenaltyVariable(mfe)) {
    constr_expr += 2 * solver_->GetPenaltyVariable(mfe);
  } else {
    assert(solver_->HasToBeCovered(mfe));
  }

  IloRange constr{solver_->cplex_env_, 1, constr_expr, IloInfinity};
  solver_->AddToModel(constr);
  return 1;
}

}
}