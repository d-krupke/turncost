//
// Created by Dominik Krupke, http://krupke.cc on 8/28/17.
//

#ifndef TURNCOSTCOVER_APX_CONNECT_COMPONENT_EDGE_H
#define TURNCOSTCOVER_APX_CONNECT_COMPONENT_EDGE_H

#include <vector>
#include "../../../problem_instance/coverage.h"
#include "./component_graph.h"
namespace turncostcover {
namespace apx
{
    class ComponentEdge
    {
    public:
      const int component_a;
      const int component_b;
      const std::vector<Field> ab_path;

      ComponentEdge(int comp_a, int comp_b, ComponentGraph &comp_graph) :
              component_a{std::min(comp_a, comp_b)}, component_b{std::max(comp_a, comp_b)},
              ab_path{comp_graph.GetShortestPath(component_a, component_b)}
      {
        assert(comp_a != comp_b);
      }

      ComponentEdge(int comp_a, int comp_b) : component_a{std::min(comp_a, comp_b)},
                                              component_b{std::max(comp_a, comp_b)}
      {

      }

      bool
      operator==(const ComponentEdge &rhs) const
      {
        return component_a == rhs.component_a &&
               component_b == rhs.component_b;
      }

      bool
      operator!=(const ComponentEdge &rhs) const
      {
        return !(rhs == *this);
      }
    };
  }
}

#endif //GRIDGRAPH_COMPONENTEDGE_H
