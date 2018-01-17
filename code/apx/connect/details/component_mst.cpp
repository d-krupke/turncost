//
// Created by Dominik Krupke, http://krupke.cc on 8/17/17.
//

#include "component_mst.h"

namespace turncostcover {
namespace apx {
namespace details {
ComponentMst::MstGraph
ComponentMst::CreateGraph(ComponentGraph &comp_graph)
{
  MstGraph mstGraph{};
  for (auto i = 0; i < solution_.GetNumComponents(); ++i) {
    add_vertex(mstGraph);
  }
  for (auto i = 0; i < solution_.GetNumComponents(); ++i) {
    for (auto j = 0; j < solution_.GetNumComponents(); ++j) {
      if (i < j) {
        turncostcover::CostUnit
            edge_cost = comp_graph.GetConnectionCost(i + 1, j + 1);
        add_edge(i, j, edge_cost, mstGraph);
      }
    }
  }
  assert(boost::num_vertices(mstGraph) != 0);
  assert(boost::num_edges(mstGraph) != 0 || boost::num_vertices(mstGraph) == 1);
  return mstGraph;
}

std::vector<ComponentEdge>
ComponentMst::CalculateMst(MstGraph &mstGraph, ComponentGraph &comp_graph)
{
  std::vector<ComponentEdge> mst_edges;
  std::vector<MstGraph::vertex_descriptor>
      predecessors(boost::num_vertices(mstGraph));
  boost::prim_minimum_spanning_tree(mstGraph, &predecessors[0]);
  for (auto i = 0u; i < boost::num_vertices(mstGraph); ++i) {
    if (predecessors[i] != i) {
      //The ids of the components start at 1
      auto comp_a = i + 1;
      auto comp_b = predecessors[i] + 1;
      mst_edges.emplace_back(comp_a, comp_b, comp_graph);
    }
  }
  return mst_edges;
}
void ComponentMst::Solve()
{
  apx::ComponentGraph comp_graph{solution_, costs_};
  comp_graph.ComputeConnectionCosts();
  MstGraph mstGraph = CreateGraph(comp_graph);
  mst_edges_ = CalculateMst(mstGraph, comp_graph);
}
}
}
}