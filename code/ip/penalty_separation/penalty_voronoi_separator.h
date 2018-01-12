//
// Created by Dominik Krupke, http://krupke.cc on 9/12/17.
//

#ifndef TURNCOSTCOVER_IP1_P_VORONOISEPARATION_H
#define TURNCOSTCOVER_IP1_P_VORONOISEPARATION_H

#include "../subset_separation/voronoi_extender.h"
#include "penalty_simple_separator.h"

namespace turncostcover {
namespace ip_formulation1 {
class PenaltyVoronoiSeparator : public PenaltySimpleSeparator {
 public:
  PenaltyVoronoiSeparator(IpSolver *solver);

 protected:
  std::map<int, std::set<turncostcover::Field>>
  GetComponents(const IntegralSolution &solution)
  const override;

  turncostcover::Field
  GetMaxInternalField(const turncostcover::IntegralSolution &solution,
                      const std::set<turncostcover::Field> &comp_fields)
  const override;
};
}
}

#endif //TURNCOSTCOVER_IP1_P_VORONOISEPARATION_H
