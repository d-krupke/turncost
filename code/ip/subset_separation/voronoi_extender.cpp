//
// Created by Dominik Krupke, http://krupke.cc on 9/12/17.
//

#include "voronoi_extender.h"
namespace turncostcover {
namespace ip_formulation1 {
namespace details {
void VoronoiExtender::Extend(std::map<int, std::set<Field>> *cells)
{
  std::map<Field, std::pair<int, int>> distance_and_origin;
  std::queue<Field> field_queue;
  for (const auto &p: *cells) {
    auto cell_idx = p.first;
    const auto &cell = p.second;
    for (const auto f: cell) {
      distance_and_origin[f] = std::pair<int, int>{0, cell_idx};
      for (auto n: graph.GetNeighbors(f)) {
        field_queue.push(n);
      }
    }
  }

  // assert(std::pair<int, int>{1,3} < std::pair<int, int>{3,1});
  while (!field_queue.empty()) {
    const auto f = field_queue.front();
    field_queue.pop();
    if (distance_and_origin.count(f) > 0) {
      continue;
    } //unweighted grid graph so we actually only have to do a BFS no full Dijkstra.
    auto neighbors = graph.GetNeighbors(f);
    auto l = [&](auto lhs, auto rhs) -> bool {
      if (distance_and_origin.count(lhs) == 0) {
        return false;
      }
      if (distance_and_origin.count(rhs) == 0) {
        return true;
      }
      return distance_and_origin.at(lhs) < distance_and_origin.at(rhs);
    };
    auto min_nbr = std::min_element(neighbors.begin(), neighbors.end(), l);
    assert(min_nbr != neighbors.end());
    distance_and_origin[f] = distance_and_origin[*min_nbr];
    distance_and_origin.at(f).first += 1;
    for (auto n: neighbors) { field_queue.push(n); }
  }
  for (auto p: distance_and_origin) {
    cells->at(p.second.second).insert(p.first);
  }
}
}
}
}