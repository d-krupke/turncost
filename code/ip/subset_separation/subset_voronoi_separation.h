//
// Created by Dominik Krupke, http://krupke.cc on 9/12/17.
//

#ifndef TURNCOSTCOVER_IP1_S_VORONOISEPARATION_H
#define TURNCOSTCOVER_IP1_S_VORONOISEPARATION_H

#include "../general_cc_solver/ip_solver.h"
#include <queue>
#include "subset_simple_separator.h"
#include "voronoi_extender.h"

namespace turncostcover {
namespace ip_formulation1 {
class SubsetVoronoiSeparation : public SubsetCoverageSimpleSeparation {
 public:
  SubsetVoronoiSeparation(IpSolver *solver, const std::set<Field> &subset);

 protected:
  std::map<int, std::set<turncostcover::Field>>
  GetComponents(const IntegralSolution &solution)
  const override
  {
    auto components = SubsetCoverageSimpleSeparation::GetComponents(solution);
    details::VoronoiExtender ve{solver_->graph_};
    ve.Extend(&components);
    return components;
  }
};

}
}

#endif //TURNCOSTCOVER_IP1_S_VORONOISEPARATION_H
