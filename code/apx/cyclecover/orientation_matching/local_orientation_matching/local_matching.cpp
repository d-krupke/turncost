//
// Created by Dominik Krupke, http://krupke.cc on 8/9/17.
//

#include "local_matching.h"
#include "auxiliary_graph.h"
namespace turncostcover {
namespace apx {
void
LocalMatching::AddTransitionalEdges()
{//create edges
  for (const auto &f: graph_->GetVertices()) {
    for (const auto &v: field_vertices_.at(f)) {
      for (const auto &n: graph_->GetNeighbors(f)) {
        for (const auto &vn: field_vertices_.at(n)) {
          auto turns_and_distance =
              ComputeCost(*graph_,
                          vertex_information_.at(
                              v),
                          vertex_information_.at(
                              vn));
          pm_->AddEdge(v, vn, turns_and_distance * costs_);
        }
      }
    }
  }
}

void
LocalMatching::AddInternalEdges()
{
  int i = 0;
  for (const auto &v: graph_->GetVertices()) {
    i = AssignVertexPair(v,
                         i,
                         Orientations::Vertical,
                         or_sol_->at(v) != Orientations::Vertical);
    i = AssignVertexPair(v, i, Orientations::Vertical, true);
    i = AssignVertexPair(v,
                         i,
                         Orientations::Horizontal,
                         or_sol_->at(v) != Orientations::Horizontal);
    i = AssignVertexPair(v, i, Orientations::Horizontal, true);
  }
  assert(i == matching_graph_cardinality_);
}

IntegralSolution
LocalMatching::ExtractSolution()
{
  IntegralSolution solution{*graph_};
  for (int j = 0; j < matching_graph_cardinality_; j += 2) {
    auto m = pm_->GetMatch(j);
    assert(m >= 0 && m < matching_graph_cardinality_);

    if (GetPartnerVertex(j) == m) { continue; }

    auto n1_match_idx = m;
    assert(n1_match_idx >= 0 && n1_match_idx < matching_graph_cardinality_);

    auto n2_match_idx = pm_->GetMatch(GetPartnerVertex(j));
    assert(n2_match_idx >= 0 && n2_match_idx < matching_graph_cardinality_);
    assert(n1_match_idx != n2_match_idx);

    solution.Add({vertex_information_[n1_match_idx].field,
                  vertex_information_[j].field,
                  vertex_information_[n2_match_idx].field});
  }
  assert(solution.IsFeasible());
  return solution;
}

IntegralSolution
LocalMatching::Solve()
{
  assert(graph_ != nullptr);
  matching_graph_cardinality_ = graph_->GetNumVertices() * 8;
  matching_graph_edge_cardinality_ = matching_graph_cardinality_ * 4 * 8;

  pm_ = std::make_unique<PerfectMatching>(matching_graph_cardinality_,
                                          matching_graph_edge_cardinality_);
  AddInternalEdges();
  AddTransitionalEdges();
  pm_->Solve();
  IntegralSolution solution = ExtractSolution();

  return solution;
}

int
LocalMatching::AssignVertexPair(Field v,
                                int i,
                                Orientations orientations,
                                bool create_internal_pair_edge)
{
  assert(i % 2 == 0);
  assert(orientations != Orientations::Penalty);

  Direction first_direction =
      (orientations == Orientations::Horizontal ? Direction::EAST
                                                : Direction::NORTH);
  vertex_information_[i] = {v, first_direction};
  vertex_information_[i + 1] = {v, GetOpposite(first_direction)};

  field_vertices_[v].push_back(i);
  field_vertices_[v].push_back(i + 1);

  if (create_internal_pair_edge) {
    pm_->AddEdge(i,
                 i + 1,
                 CostUnit{0});
  }
  return i + 2;
}

int
LocalMatching::GetPartnerVertex(int j)
{
  return j ^ 1;
}

}
}




