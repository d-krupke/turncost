//
// Created by Dominik Krupke, http://krupke.cc on 4/26/17.
//

#ifndef TURNCOSTCOVER_IP_FC_SEPARATOR_2_H
#define TURNCOSTCOVER_IP_FC_SEPARATOR_2_H

#include "../general_cc_solver/ip_solver.h"
#include "full_coverage_sufficient_separator.h"

namespace turncostcover {
namespace ip_formulation1 {
class FullCoverageSufficientSeparator {

 public:

  virtual size_t Separate(const IntegralSolution &solution, std::function<void(IloRange&)>& f_add_constraint)
  {
    if (solution.GetNumComponents() <= 1) { return 0; }

    size_t cuts_added = 0;
    auto cycles = solution.CollectCycles();
    for (auto cycle: cycles) {
      auto constr = CreateConstraint(cycle, cycles, solution);
      if (constr) {
        f_add_constraint(*constr);
        cuts_added += 1;
      }
    }
    assert(cuts_added > 0);
    assert(cuts_added != 1);//For every useful component there should be one cut. If there is only 1 cut,
    // there is only one useful component (possibly bug!).
    return cuts_added;
  }

  virtual size_t Separate(const IntegralSolution &solution)
  {
    assert(this->solver_!= nullptr);
    std::function<void(IloRange &)> lambda = [&](IloRange &constr) { this->solver_->AddToModel(constr); };
    return Separate(solution, lambda);
  }

  explicit FullCoverageSufficientSeparator(IpSolver *solver)
      : solver_(solver) {}

 protected:
  std::set<Field> GetFields(const std::vector<Coverage> &coverages) const;

  std::set<Field>
  MergeComponents(const IntegralSolution &solution,
                  Field f,
                  const std::vector<std::vector<Coverage>> &cycles)
  const;

  virtual Field
  FindMinCoveredFieldInCycle(const std::set<Field> &cycle,
                             const IntegralSolution &solution)
  const;

  void
  CoverOnAConnectingWay(
      const IntegralSolution &solution,
      IloExpr *constr,
      Field v)
  const;

  void
  CoverBySomeUnusedCoverage(
      const Field field,
      const IntegralSolution &solution,
      IloExpr *constr)
  const;

  IloExpr
  GetCoverageExpression(const IntegralSolution &solution,
                        const std::set<Field> &fields_in_cycle,
                        Field min_covered_field)
  const;

  virtual std::unique_ptr<IloRange>
  CreateConstraint(const std::vector<Coverage> &cycle,
                   const std::vector<std::vector<Coverage>> &cycles,
                   const IntegralSolution &solution)
  const;

 protected:
  IpSolver *solver_;
};

struct MinCoveredFieldNotFoundException {};

}
}
#endif //TURNCOSTCOVER_IP_FC_SEPARATOR_2_H
