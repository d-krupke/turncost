//
// Created by Dominik Krupke, http://krupke.cc on 9/12/17.
//

#include "subset_voronoi_separation.h"
namespace turncostcover {
namespace ip_formulation1 {
SubsetVoronoiSeparation::SubsetVoronoiSeparation(
    IpSolver *solver,
    const std::set<Field> &subset)
    : SubsetCoverageSimpleSeparation(solver, subset) {}
}
}
