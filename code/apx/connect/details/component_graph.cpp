//
// Created by Dominik Krupke, http://krupke.cc on 8/15/17.
//

#include "component_graph.h"
namespace turncostcover {
namespace apx {
void
ComponentGraph::AssignComponents()
{
  for (const auto &v: solution_.GetGridGraph().GetVertices()) {
    const auto cmps = solution_.GetComponentsFieldIsIn(v, true);

    if (cmps.empty()) {
      //not covered
      assert(!solution_.IsCovered(v));

      continue;
    }

    const int cmp = cmps.front();
    assert(cmps.size() == 1);
    component_of_field_[v] = cmp;
    components_[cmp].push_back(v);

  }
}

void
ComponentGraph::CalculateAllDistanceFromComp(const int cmp_id)
{
  const auto &fields_of_component = components_[cmp_id];

  turncostgrid::TurnCostGridDijkstra
      turnCostGridDijkstra{spt_graph_, {costs_.turn_costs, costs_.dist_costs}};
  //add all starting points from the component
  for (const auto f: fields_of_component) {
    int field_id = spt_graph_.GetId(f);
    for (auto d: {turncostgrid::Direction::NORTH,
                  turncostgrid::Direction::SOUTH,
                  turncostgrid::Direction::EAST,
                  turncostgrid::Direction::WEST}) {
      turnCostGridDijkstra.add_starting_point(field_id, d);
    }
  }

  turnCostGridDijkstra.lazy_dijkstra([&](int field_id,
                                         turncostgrid::Direction dir) -> bool {
    turncostcover::Field f = spt_graph_.GetField(field_id);
    if (component_of_field_.count(f) == 0
        || component_of_field_.at(f) == cmp_id) { return true; }
    const int f_cmp = component_of_field_.at(f);
    std::pair<int, int> edge{cmp_id, f_cmp};
    if (distances_.count(edge) == 0) {
      //check distance
      const auto dist = turnCostGridDijkstra.distance(field_id, dir);
      auto sp = turnCostGridDijkstra.shortest_path(field_id, dir);
      std::vector<turncostcover::Field> spv;
      for (auto id_dir: sp) {
        auto v = spt_graph_.GetField(id_dir.first);
        if (spv.empty() || spv.back() != v) { spv.push_back(v); }
      }
      distances_[edge] = dist;
      paths_[edge] = spv;
    }
    return true;
  });
}

}
}