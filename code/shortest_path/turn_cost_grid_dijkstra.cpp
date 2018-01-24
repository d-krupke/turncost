//
// Created by Dominik Krupke, http://krupke.cc on 6/15/17.
//

#include "turn_cost_grid_dijkstra.h"
#include "DFieldId.h"


std::vector<std::pair<unsigned int, turncostgrid::Direction>>
turncostgrid::TurnCostGridDijkstra::reversed_shortest_path(unsigned int ext_id, turncostgrid::Direction d)
const
{
  assert(already_calulated(ext_id, d));
  std::vector<std::pair<unsigned int, Direction>> reversed_path;
  DFieldId v{grid_graph.external_to_internal_id(ext_id), d};
  while (v.fieldId() != GridGraph::FieldId{}) {
    reversed_path.push_back({v.fieldId().external_id(), v.dir()});
    v = predecessor_map[v.idx()];
  }
  return reversed_path;
}

std::vector<std::pair<unsigned int, turncostgrid::Direction>>
turncostgrid::TurnCostGridDijkstra::shortest_path(unsigned int ext_id, turncostgrid::Direction d)
const
{
  assert(already_calulated(ext_id, d));
  auto sp = reversed_shortest_path(ext_id, d);
  std::reverse(sp.begin(), sp.end());
  return sp;
}

turncostgrid::TurnCostGridDijkstra::TurnCostGridDijkstra(
        turncostgrid::GridGraph &grid_graph,
        turncostgrid::Costs costs,
        unsigned int ext_id, turncostgrid::Direction dir) :
        grid_graph{grid_graph}, costs{costs}
{
  predecessor_map.resize(dfield_size(), DFieldId{});
  distance_map.resize(dfield_size(), std::numeric_limits<CostUnit>::max());
  add_starting_point(ext_id, dir);
}

turncostgrid::TurnCostGridDijkstra::TurnCostGridDijkstra(turncostgrid::GridGraph &grid_graph, turncostgrid::Costs costs)
        : grid_graph{grid_graph}, costs{costs}
{
  predecessor_map.resize(dfield_size(), DFieldId{});
  distance_map.resize(dfield_size(), std::numeric_limits<CostUnit>::max());
}

void
turncostgrid::TurnCostGridDijkstra::dijkstra()
{
  lazy_dijkstra([](unsigned int a, Direction b) -> bool { return true; });
}
