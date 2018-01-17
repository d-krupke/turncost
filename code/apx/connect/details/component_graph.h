//
// Created by Dominik Krupke, http://krupke.cc on 8/15/17.
//

#ifndef TURNCOSTCOVER_APX_CONNECT_COMPONENT_GRAPH_H
#define TURNCOSTCOVER_APX_CONNECT_COMPONENT_GRAPH_H

#include "../../../problem_instance/costs.h"
#include "../../../problem_instance/solution/solution.h"
#include "../../../shortest_path/turn_cost_grid_dijkstra.h"
#include "tcg_grid_graph_wrapper.h"

namespace turncostcover {
namespace apx {

class ComponentGraph {

 public:
  ComponentGraph(const IntegralSolution &solution, Costs costs)
      : solution_{solution}, spt_graph_{solution.GetGridGraph()}, costs_{costs}
  {
    //precondition: solution has already been greedy connected.
    AssignComponents();

  }

  void
  ComputeConnectionCosts()
  {
    if (is_calculated_) { return; }
    for (const auto &p: components_) {
      const auto cmp_id = p.first;
      CalculateAllDistanceFromComp(cmp_id);
    }
    is_calculated_ = true;
  }

  CostUnit
  GetConnectionCost(int comp_a, int comp_b)
  {
    if (!is_calculated_) { ComputeConnectionCosts(); }
    if (distances_.count({comp_a, comp_b}) == 0) {
      return std::numeric_limits<CostUnit>::max();
    }
    return distances_.at({comp_a, comp_b});
  }

  std::vector<Field>
  GetShortestPath(int comp_a, int comp_b)
  {
    if (!is_calculated_) { ComputeConnectionCosts(); }
    return paths_.at({comp_a, comp_b});
  }

 private:
  void AssignComponents();

  void CalculateAllDistanceFromComp(const int cmp_id);

  bool is_calculated_ = false;

  const IntegralSolution &solution_;

  std::map<int, std::vector<Field>> components_;
  std::map<Field, int> component_of_field_;
  std::map<std::pair<int, int>, CostUnit> distances_;

  TCGGridGraphWrapper spt_graph_;
  Costs costs_;
  std::map<std::pair<int, int>, std::vector<Field>> paths_;

};
}
}

#endif //TURNCOSTCOVER_APX_CONNECT_COMPONENT_GRAPH_H
