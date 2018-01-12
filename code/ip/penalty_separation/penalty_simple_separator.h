//
// Created by Dominik Krupke, http://krupke.cc on 9/11/17.
//

#ifndef TURNCOSTCOVER_IP1_P_SIMPLESEPARATOR_H
#define TURNCOSTCOVER_IP1_P_SIMPLESEPARATOR_H

#include "../general_cc_solver/ip_solver.h"
#include "../fc_separation/full_coverage_simple_separator.h"

namespace turncostcover {
namespace ip_formulation1 {

class PenaltySimpleSeparator : public FullCoverageSimpleSeparator {
 public:
  explicit PenaltySimpleSeparator(IpSolver *solver);

 protected:

  size_t
  CreateConstraint(const IntegralSolution &solution,
                   const std::set<Field> &comp_fields)
  const override;

  virtual
  Field
  GetMaxInternalField(const IntegralSolution &solution,
                      const std::set<Field> &comp_fields)
  const;

  virtual
  Field
  GetUsedMaxExternalField(const IntegralSolution &solution,
                          const std::set<Field> &comp_fields)
  const;
};

}
}

#endif //TURNCOSTCOVER_IP1_P_SIMPLESEPARATOR_H
