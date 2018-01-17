//
// Created by Dominik Krupke, http://krupke.cc on 8/9/17.
//

#ifndef TURNCOSTCOVER_APX_CYCLECOVER_LOCAL_MATCHING_H
#define TURNCOSTCOVER_APX_CYCLECOVER_LOCAL_MATCHING_H

#include <cyclecover/orientation_matching/blossom5-v205/PerfectMatching.h>
#include "../../../../problem_instance/solution/solution.h"
#include "cyclecover/orientation_selection/solution.h"
#include "auxiliary_graph.h"
namespace turncostcover {
namespace apx {
class LocalMatching {

 public:
  LocalMatching() {}

  LocalMatching(const GridGraph &graph,
                OrientationSolution *or_sol,
                Costs costs)
      : graph_{&graph}, or_sol_{or_sol}, costs_{costs}
  {

  }

  IntegralSolution Solve();

 private:
  IntegralSolution ExtractSolution();

  void AddInternalEdges();

  void AddTransitionalEdges();

  int
  AssignVertexPair(Field v,
                   int i,
                   Orientations orientations,
                   bool create_internal_pair_edge);

  int GetPartnerVertex(int j);

  const GridGraph *graph_ = nullptr;
  OrientationSolution *or_sol_ = nullptr;
  Costs costs_{0, 0};

  std::map<int, MatchingVertexInformation> vertex_information_;
  std::map<Field, std::vector<int>> field_vertices_;
  std::unique_ptr<PerfectMatching> pm_;

  int matching_graph_cardinality_;
  int matching_graph_edge_cardinality_;
};
}
}

#endif //TURNCOSTCOVER_APX_CYCLECOVER_LOCAL_MATCHING_H
