//
// Created by Dominik Krupke, http://krupke.cc on 4/28/17.
//

#ifndef TURNCOSTCOVER_IP1_S_SEPERATOR_1_H
#define TURNCOSTCOVER_IP1_S_SEPERATOR_1_H

#include <boost/graph/filtered_graph.hpp>
#include "../../problem_instance/solution/solution.h"
#include "../fc_separation/full_coverage_simple_separator.h"
#include "../general_cc_solver/ip_solver.h"

namespace turncostcover {
namespace ip_formulation1 {
class SubsetCoverageSimpleSeparation : public FullCoverageSimpleSeparator {
 public:
  SubsetCoverageSimpleSeparation(IpSolver *solver,
                                 const std::set<turncostcover::Field> &subset);

 protected:
  size_t
  CreateConstraint(const turncostcover::IntegralSolution &solution,
                   const std::set<Field> &comp_fields)
  const override
  {
    if (IsSkipComponent(comp_fields)) {
      return 0;
    } else {
      using FCSS = FullCoverageSimpleSeparator;
      return FCSS::CreateConstraint(solution, comp_fields);
    }
  }

  bool IsSkipComponent(const std::set<Field> &component) const;

 protected:
  const std::set<turncostcover::Field> &subset;
};
}
}
#endif //TURNCOSTCOVER_IP1_S_SEPERATOR_1_H
