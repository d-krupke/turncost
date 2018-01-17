//
// Created by Dominik Krupke, http://krupke.cc on 8/16/17.
//

#ifndef GRIDGRAPH_SPTGRIDGRAPHWRAPPER_H
#define GRIDGRAPH_SPTGRIDGRAPHWRAPPER_H

#include "../../../shortest_path/grid_graph.h"
#include "../../../problem_instance/grid_graph.h"
#include <boost/bimap.hpp>
namespace turncostcover {
namespace apx {
class TCGGridGraphWrapper : public turncostgrid::GridGraph {

 public:

  TCGGridGraphWrapper(const turncostcover::GridGraph &graph);

  TCGGridGraphWrapper() = default;

  int
  GetId(Field f)
  const
  {
    return id_field_bimap_.right.at(f);
  }

  Field
  GetField(int id)
  const
  {
    return id_field_bimap_.left.at(id);
  }
 private:

  std::vector<turncostgrid::GridCoordinate> coordinates_;
  boost::bimap<int, Field> id_field_bimap_;
};

}
}
#endif //GRIDGRAPH_SPTGRIDGRAPHWRAPPER_H
