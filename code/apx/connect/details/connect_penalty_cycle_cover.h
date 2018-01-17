//
// Created by Dominik Krupke, http://krupke.cc on 8/25/17.
//

#ifndef TURNCOSTCOVER_APX_CONNECT_CONNECT_PENALTY_CYCLE_COVER_H
#define TURNCOSTCOVER_APX_CONNECT_CONNECT_PENALTY_CYCLE_COVER_H

#include "../../../problem_instance/solution/solution.h"
#include "component_graph.h"
#include "pcst_fast/pcst_fast.h"
#include "component_mst.h"
namespace turncostcover {
namespace apx {
namespace details {
class ConnectPenaltyCycleCover {

 public:
  ConnectPenaltyCycleCover(IntegralSolution *solution,
                           const std::map<Field, double> &penalties,
                           Costs costs)
      : solution_{solution},
        penalties_{penalties},
        costs_{costs},
        comp_graph_{*solution, costs}
  {

  }

  void Run(std::vector<int> &components, std::vector<ComponentEdge> &edges);

 private:
  std::vector<ComponentEdge>
  ConvertToComponentEdge(const std::vector<int> &pcst_selected_edge_indices);

  void CreateGraphForPcst();
  void InitEdges();
  void InitPrizes();
  void SolvePcst();

  IntegralSolution *solution_;
  const std::map<Field, double> &penalties_;
  Costs costs_;

  // note: prize for covering == penalty for not covering
  std::vector<double> pcst_prizes_;
  std::vector<double> pcst_edge_costs_;
  std::vector<std::pair<int, int>> pcst_edges_;

  ComponentGraph comp_graph_;

  std::vector<int> node_result_;
  std::vector<int> edge_result_;
};
}

}
}

#endif //TURNCOSTCOVER_APX_CONNECT_CONNECT_PENALTY_CYCLE_COVER_H
