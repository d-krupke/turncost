//
// Created by Dominik Krupke, http://krupke.cc on 4/28/17.
//

#ifndef TURNCOSTCOVER_IP1_S_SEPARATOR_2_H
#define TURNCOSTCOVER_IP1_S_SEPARATOR_2_H

#include "../general_cc_solver/ip_solver.h"
#include "../../problem_instance/solution/solution.h"
#include "../fc_separation/full_coverage_sufficient_separator.h"

namespace turncostcover {
namespace ip_formulation1 {
class SubsetSufficientSeparation : public FullCoverageSufficientSeparation {

 public:

  SubsetSufficientSeparation(IpSolver *solver, const std::set<Field> &subset)
      : FullCoverageSufficientSeparation{solver}, subset{subset}
  {

  }

  Field
  FindMinCoveredFieldInCycle(const std::set<Field> &fields_in_cycle,
                             const IntegralSolution &solution)
  const override;
 private:

  const std::set<Field> &subset;
};
}
}

#endif //TURNCOSTCOVER_IP_FC_SEPARATOR_2_H
