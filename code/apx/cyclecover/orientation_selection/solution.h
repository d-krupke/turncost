//
// Created by Dominik Krupke, http://krupke.cc on 7/28/17.
//

#ifndef TURNCOSTCOVER_APX_CYCLECOVER_ORIENTATION_SELECTION_SOLUTION_H
#define TURNCOSTCOVER_APX_CYCLECOVER_ORIENTATION_SELECTION_SOLUTION_H

#include "../../../problem_instance/grid_graph.h"
namespace turncostcover {
namespace apx {

enum class Orientations {
  Horizontal, Vertical, Penalty
};


using OrientationSolution = std::map<turncostcover::GridGraph::FieldId,
                                     Orientations>;

}

}
#endif //TURNCOSTCOVER_APX_CYCLECOVER_ORIENTATION_SELECTION_SOLUTION_H
