//
// Created by Dominik Krupke, http://krupke.cc on 7/19/17.
//

#include "auxiliary_graph.h"

namespace turncostcover {
namespace apx {

DistanceAndTurns
ComputeCost(const GridGraph &graph, MatchingVertexInformation start,
            MatchingVertexInformation goal)
{
  //neighbor orientation
  Direction no = DeduceNeighborDirection(start.field, goal.field, graph);
  auto nr_turns = GetDifference(start.orientation, no) +
      (2 - GetDifference(no, goal.orientation));
  DistanceAndTurns ret{nr_turns, 1};
  return ret;
}

}
}
