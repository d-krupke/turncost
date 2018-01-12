//
// Created by Dominik Krupke, http://krupke.cc on 9/12/17.
//

#ifndef TURNCOSTCOVER_IP1_S_VORONOIEXTENDER_H
#define TURNCOSTCOVER_IP1_S_VORONOIEXTENDER_H

#include <queue>
#include "../../problem_instance/grid_graph.h"

namespace turncostcover {
namespace ip_formulation1 {
namespace details {
class VoronoiExtender {
 public:
  VoronoiExtender(const GridGraph &graph) : graph(graph) {}

  void
  Extend(std::map<int, std::set<Field>> *cells);

 private:
  const GridGraph &graph;
};
}
}
}

#endif //TURNCOSTCOVER_IP1_S_VORONOIEXTENDER_H
