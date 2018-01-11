//
// Created by Dominik Krupke, http://krupke.cc on 8/8/17.
//

#include "direction.h"
namespace turncostcover {
int
GetDifference(Direction start, Direction goal)
{
  if (start == goal) { return 0; }
  if ((start == Direction::NORTH && goal == Direction::SOUTH) ||
      (start == Direction::SOUTH && goal == Direction::NORTH) ||
      (start == Direction::WEST && goal == Direction::EAST) ||
      (start == Direction::EAST && goal == Direction::WEST)) {
    return 2;
  }
  return 1;
}

Direction
DeduceNeighborDirection(Field field, Field neighbor, const GridGraph &graph)
{
  assert(graph.AreNeighbored(field, neighbor));
  auto f_pos = graph.GetCoordinate(field);
  auto n_pos = graph.GetCoordinate(neighbor);
  if (f_pos.x != n_pos.x) {
    if (f_pos.x < n_pos.x) {
      return Direction::EAST;
    } else {
      return Direction::WEST;
    }
  } else {
    if (f_pos.y < n_pos.y) {
      return Direction::NORTH;
    } else {
      return Direction::SOUTH;
    }
  }
  assert(false);
}

Direction
GetOpposite(Direction o)
{
  switch (o) {
    case Direction::NORTH:return Direction::SOUTH;
    case Direction::EAST:return Direction::WEST;
    case Direction::SOUTH:return Direction::NORTH;
    case Direction::WEST:return Direction::EAST;
    default:assert(false);
  }
  assert(false);
}

}
