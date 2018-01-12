//
// Created by Dominik Krupke, http://krupke.cc on 9/11/17.
//

#ifndef TURNCOSTCOVER_IP1_P_SUFFICIENTSEPARATOR_H
#define TURNCOSTCOVER_IP1_P_SUFFICIENTSEPARATOR_H

#include "penalty_simple_separator.h"
#include "../fc_separation/full_coverage_sufficient_separator.h"
#include "max_min_fields.h"

namespace turncostcover {
namespace ip_formulation1 {

class PenaltySufficientSeparator : public FullCoverageSufficientSeparation {
 public:
  explicit PenaltySufficientSeparator(IpSolver *solver);

  size_t
  CreateConstraint(const std::vector<Coverage> &cycle,
                   const std::vector<std::vector<Coverage>> &cycles,
                   const IntegralSolution &solution)
  const override;
};

}
}

#endif //TURNCOSTCOVER_IP1_P_SUFFICIENTSEPARATOR_H
