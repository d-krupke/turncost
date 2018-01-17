//
// Created by Dominik Krupke, http://krupke.cc on 5/31/17.
//

#ifndef TURNCOSTCOVER_APX_CYCLECOVER_SELECT_ORIENTATIONS_H
#define TURNCOSTCOVER_APX_CYCLECOVER_SELECT_ORIENTATIONS_H

#include "../../../problem_instance/grid_graph.h"
#include "linear_program.h"
#include "solution.h"
namespace turncostcover {
namespace apx {

std::map<GridGraph::FieldId,
         std::tuple<double, double, double>> //Horizontal, Vertical, Penalty
GetFractionalOrientations(const GridGraph &g, PenaltyLP::Solution &solution);

OrientationSolution
SelectOrientations(const GridGraph &g, PenaltyLP::Solution &solution);

}
}
#endif //TURNCOSTCOVER_APX_CYCLECOVER_SELECT_ORIENTATIONS_H
