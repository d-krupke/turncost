//
// Created by Dominik Krupke, http://krupke.cc on 10/9/17.
//

#include "strip_cover.h"
namespace turncostcover {
namespace arkinapx {


std::vector<Strip>
ComputeStripCover(const GridGraph &graph) {
  StripExtractor stripExtractor{};
  auto strips = stripExtractor.ExtractStrips(graph);
  using AuxGraph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>;
  AuxGraph vcGraph{strips.size()};
  for (unsigned i = 0; i < strips.size(); ++i) {
    for (unsigned j = i + 1; j < strips.size(); ++j) {
      if (strips.at(i).DoIntersect(strips.at(j))) {
        boost::add_edge(i, j, vcGraph);
      }
    }
  }
  assert(boost::num_edges(vcGraph) == graph.GetNumVertices());

  auto vc_indices = bipartvc::get_minimal_vertex_cover(vcGraph);
  std::vector<Strip> strip_cover;
  for (auto i: vc_indices) {
    strip_cover.push_back(strips.at(i));
  }
  return strip_cover;
}
}
}
