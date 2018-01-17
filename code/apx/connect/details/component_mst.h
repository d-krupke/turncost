//
// Created by Dominik Krupke, http://krupke.cc on 8/17/17.
//

#ifndef TURNCOSTCOVER_APX_CONNECT_COMPONENT_MST_H
#define TURNCOSTCOVER_APX_CONNECT_COMPONENT_MST_H

#include "../../../problem_instance/solution/solution.h"
#include "component_graph.h"
#include "greedy_connect.h"
#include "path_to_cycle.h"
#include "component_edge.h"
#include <boost/graph/prim_minimum_spanning_tree.hpp>
namespace turncostcover {
namespace apx {
namespace details {
class ComponentMst {

 public:

  ComponentMst(const IntegralSolution &integralSolution, Costs costs)
      : solution_{integralSolution}, costs_{costs}
  {

  }

  void Solve();

  std::vector<ComponentEdge> GetEdges() const { return mst_edges_; }

 private:

  using MstGraph = boost::adjacency_list<boost::vecS,
                                         boost::vecS,
                                         boost::undirectedS,
                                         boost::property<boost::vertex_distance_t,
                                                         CostUnit>,
                                         boost::property<boost::edge_weight_t,
                                                         CostUnit> >;

  MstGraph CreateGraph(ComponentGraph &comp_graph);

  std::vector<ComponentEdge>
  CalculateMst(MstGraph &mstGraph, ComponentGraph &comp_graph);

  const IntegralSolution &solution_;
  const Costs costs_;
  std::vector<ComponentEdge> mst_edges_;
};
}

}
}

#endif //TURNCOSTCOVER_APX_CONNECT_COMPONENT_MST_H
