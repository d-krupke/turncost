//
// Created by Dominik Krupke, http://krupke.cc on 8/16/17.
//

#include "tcg_grid_graph_wrapper.h"
namespace turncostcover {
namespace apx {

TCGGridGraphWrapper::TCGGridGraphWrapper(const turncostcover::GridGraph &graph)
    : coordinates_{}, id_field_bimap_{}
{
  for (const auto &v: graph.GetVertices()) {
    auto coord = graph.GetCoordinate(v);
    int id = static_cast<int>(coordinates_.size());
    id_field_bimap_.insert({id, v});
    assert(id_field_bimap_.left.at(id) == v);
    assert(id_field_bimap_.right.at(v) == id);
    coordinates_.push_back({coord.x, coord.y});
  }
  construct_from_vector(coordinates_, false);
}
}
}