//
// Created by Dominik Krupke, http://krupke.cc on 8/8/17.
//

#ifndef TURNCOSTCOVER_DIRECTION_H
#define TURNCOSTCOVER_DIRECTION_H

#include "grid_graph.h"

namespace turncostcover {
enum class Direction {
  NORTH, EAST, SOUTH, WEST
};

Direction GetOpposite(Direction o);

/**
 * GetDifference(NORTH, SOUTH)==2
 * GetDifference(NORTH, NORTH)==0
 * GetDifference(WEST, SOUTH) ==1
 */
int GetDifference(Direction start, Direction goal);

/// The direction of the vector heading from the field to the neighbored field
Direction
DeduceNeighborDirection(turncostcover::Field field,
                        turncostcover::Field neighbor,
                        const GridGraph &graph);
}
#endif // TURNCOSTCOVER_DIRECTION_H
