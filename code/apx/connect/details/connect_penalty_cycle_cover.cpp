//
// Created by Dominik Krupke, http://krupke.cc on 8/25/17.
//

#include "connect_penalty_cycle_cover.h"
namespace turncostcover {
namespace apx {
namespace details {
void ConnectPenaltyCycleCover::Run(std::vector<int> &components,
                                   std::vector<ComponentEdge> &edges) {
  assert(components.empty());
  assert(edges.empty());

  if (solution_->GetNumComponents() == 0) { return; }

  CreateGraphForPcst();
  SolvePcst();
  edges = ConvertToComponentEdge(edge_result_);
  for (auto c: node_result_) components.push_back(c + 1);
}
std::vector<ComponentEdge> ConnectPenaltyCycleCover::ConvertToComponentEdge(
    const std::vector<int> &pcst_selected_edge_indices) {
  std::vector<ComponentEdge> component_edges;
  for (auto i: pcst_selected_edge_indices) {
    int comp_a = pcst_edges_.at(i).first + 1;
    int comp_b = pcst_edges_.at(i).second + 1;
    component_edges.emplace_back(comp_a, comp_b, comp_graph_);
  }
  return component_edges;
}
void ConnectPenaltyCycleCover::CreateGraphForPcst() {
  comp_graph_.ComputeConnectionCosts();
  InitPrizes();
  InitEdges();

}
void ConnectPenaltyCycleCover::InitPrizes() {
  pcst_prizes_.resize(static_cast<unsigned int>(solution_->GetNumComponents()), 0);
  for (auto p: penalties_) {
    if (solution_->IsCovered(p.first)) {
      auto cmp_id = solution_->GetComponentsFieldIsIn(p.first).front();
      pcst_prizes_[cmp_id - 1] += p.second;
    }
  }
}
void ConnectPenaltyCycleCover::InitEdges() {
  for (int i = 0; i < solution_->GetNumComponents(); ++i) {
    for (int j = 0; j < solution_->GetNumComponents(); ++j) {
      if (i < j) { continue; }
      auto edge_cost = comp_graph_.GetConnectionCost(i + 1, j + 1);
      pcst_edges_.emplace_back(i, j);
      pcst_edge_costs_.push_back(edge_cost);
    }
  }
}
void ConnectPenaltyCycleCover::SolvePcst() {
  int root = -1;
  int target_num_active_clusters = 1;
  const int kVerbosityLevel = 0;
  cluster_approx::PCSTFast::PruningMethod pruning = cluster_approx::PCSTFast::kNoPruning;


  cluster_approx::PCSTFast algo(pcst_edges_,
                                pcst_prizes_,
                                pcst_edge_costs_,
                                root,
                                target_num_active_clusters,
                                pruning,
                                kVerbosityLevel,
                                [](const char *s) { std::cerr << s << std::endl; });
  algo.run(&node_result_, &edge_result_);
}

}
}
}
