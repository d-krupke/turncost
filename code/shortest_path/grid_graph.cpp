//
// Created by Dominik Krupke, http://krupke.cc on 6/16/17.
//

#include "grid_graph.h"

std::string
turncostgrid::to_string(Direction d)
{
  switch (d) {
    case NORTH:return "north";
    case EAST:return "east";
    case SOUTH:return "south";
    case WEST:return "west";
  }
  assert(false);
}

turncostgrid::Direction
turncostgrid::opposite(turncostgrid::Direction &d)
{
  switch (d) {
    case NORTH:return SOUTH;
    case SOUTH:return NORTH;
    case EAST:return WEST;
    case WEST:return EAST;
  }
  assert(false);
}

turncostgrid::GridGraph::GridGraph(std::vector<turncostgrid::GridCoordinate> &fields, bool sorted)
{
  construct_from_vector(fields, sorted);
}

void
turncostgrid::GridGraph::construct_from_vector(std::vector<turncostgrid::GridCoordinate> &fields, bool sorted)
{
  sort(fields.begin(), fields.end());
  auto getId = [&](turncostgrid::GridCoordinate c) -> turncostgrid::GridGraph::FieldId {
      if (!sorted) {
        auto it = find(fields.begin(), fields.end(), c);
        if (it == fields.end()) { return turncostgrid::GridGraph::FieldId{}; }
        return turncostgrid::GridGraph::FieldId{static_cast<unsigned int>(distance(fields.begin(), it) + 1)};
      }
      auto it = lower_bound(fields.begin(), fields.end(), c);
      if (it == fields.end() || *it != c) { return turncostgrid::GridGraph::FieldId{}; }
      return turncostgrid::GridGraph::FieldId{static_cast<unsigned int>(distance(fields.begin(), it) + 1)};
  };

  this->adjacency_array_map.resize(fields.size() + 1);
  unsigned int id = 1;
  for (auto vi = fields.begin(); vi != fields.end(); ++vi) {
    auto v = *vi;

    this->adjacency_array_map[id].fill(turncostgrid::GridGraph::FieldId{});

    turncostgrid::GridGraph::FieldId n_south = getId({v.x, v.y - 1});
    if (n_south != turncostgrid::GridGraph::FieldId{}) {
      this->adjacency_array_map[n_south.internal_id()][turncostgrid::NORTH] = turncostgrid::GridGraph::FieldId{id};
      this->adjacency_array_map[id][turncostgrid::SOUTH] = n_south;
    }
    turncostgrid::GridGraph::FieldId n_west = getId({v.x - 1, v.y});
    if (n_west != turncostgrid::GridGraph::FieldId{}) {
      this->adjacency_array_map[n_west.internal_id()][turncostgrid::EAST] = turncostgrid::GridGraph::FieldId{id};
      this->adjacency_array_map[id][turncostgrid::WEST] = n_west;
    }
    if (!sorted) {
      turncostgrid::GridGraph::FieldId n_north = getId({v.x, v.y + 1});
      if (n_north != turncostgrid::GridGraph::FieldId{}) {
        this->adjacency_array_map[n_north.internal_id()][turncostgrid::SOUTH] = turncostgrid::GridGraph::FieldId{id};
        this->adjacency_array_map[id][turncostgrid::NORTH] = n_north;
      }
      turncostgrid::GridGraph::FieldId n_east = getId({v.x + 1, v.y});
      if (n_east != turncostgrid::GridGraph::FieldId{}) {
        this->adjacency_array_map[n_east.internal_id()][turncostgrid::WEST] = turncostgrid::GridGraph::FieldId{id};
        this->adjacency_array_map[id][turncostgrid::EAST] = n_east;
      }
    }
    ++id;
  }
}

void
turncostgrid::GridGraph::print_graph(std::vector<turncostgrid::GridCoordinate> &fields)
{
  for (unsigned int i = 0; i < fields.size(); ++i) {
    FieldId v{i + 1};
    std::cout << v.external_id() << " (" << fields[i].x << ", " << fields[i].y << "): ";
    for (auto n: adjacency_array_map[v.internal_id()]) {
      std::cout << (n != FieldId{} ? std::to_string(n.external_id()) : "-") << " ";
    }
    std::cout << std::endl;
  }
}
