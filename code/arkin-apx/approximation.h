//
// Created by Dominik Krupke, http://krupke.cc on 10/5/17.
//

#ifndef TURNCOSTCOVER_ARKINAPX_APPROXIMATION_H
#define TURNCOSTCOVER_ARKINAPX_APPROXIMATION_H

#include "strip_matching/strip_matching_solver.h"
#include "../problem_instance/costs.h"
#include "../problem_instance/grid_graph.h"
#include "../problem_instance/solution/solution.h"
#include "../apx/connect/connect.h"
namespace turncostcover {
namespace arkinapx {
IntegralSolution ApproximateFullCycleCoverViaStripCover(const GridGraph &graph,
                                                        Costs costs);

IntegralSolution ApproximateFullTourViaStripCover(const GridGraph &graph,
                                                  Costs costs);
}
}
#endif //TURNCOSTCOVER_ARKINAPX_APPROXIMATION_H
