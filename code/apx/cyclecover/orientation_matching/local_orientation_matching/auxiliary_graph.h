//
// Created by Dominik Krupke, http://krupke.cc on 7/19/17.
//

#ifndef TURNCOSTCOVER_APX_CYCLECOVER_AUXILIARY_GRAPH_H
#define TURNCOSTCOVER_APX_CYCLECOVER_AUXILIARY_GRAPH_H

#include "../../../../problem_instance/grid_graph.h"
#include "../../../../problem_instance/solution/solution.h"
#include "../../orientation_selection/solution.h"
#include "../../../../problem_instance/direction.h"
namespace turncostcover {
namespace apx {

struct MatchingVertexInformation {
  Field field;
  Direction orientation;

};

DistanceAndTurns
ComputeCost(const GridGraph &graph,
            MatchingVertexInformation start,
            MatchingVertexInformation goal);

}
}

#endif //TURNCOSTCOVER_APX_CYCLECOVER_AUXILIARY_GRAPH_H
