//
// Created by Dominik Krupke, http://krupke.cc on 9/5/17.
//
#include "./connect.h"
namespace turncostcover {
namespace apx{


void
ConnectAdjacentCycles(IntegralSolution *solution, Costs costs)
{
  GreedyConnectAdjacentCycles(solution, costs);
}

void
ConnectDistantCycles(IntegralSolution *solution,
                                   Costs costs)
{
  ConnectAdjacentCycles(solution, costs);
  details::ComponentMst componentMst{*solution, costs};
  componentMst.Solve();
  for (const auto &e : componentMst.GetEdges()) {
    const auto &path = e.ab_path;
    auto cycle = PathToCycle(path);
    solution->Add(cycle);
  }
  ConnectAdjacentCycles(solution, costs);
}

void
ConnectOrRemoveCycles(
    IntegralSolution *solution,
    Costs costs,
    const std::map<Field, double> &penalties)
{
  ConnectAdjacentCycles(solution, costs);
  details::ConnectPenaltyCycleCover alg{solution, penalties, costs};
  std::vector<int> components;
  std::vector<ComponentEdge> edges;
  alg.Run(components, edges);
  // remove components
  auto comps = solution->CollectCycles();
  decltype(comps) comps_to_remove;
  for (auto cycle : comps) {
    assert(!cycle.empty());
    auto cycle_id = solution->GetComponentId(cycle.front());
    auto it = std::find(components.begin(), components.end(), cycle_id);
    if (it == components.end()) {
      comps_to_remove.push_back(cycle);
    }
  }
  for (const auto &cycle: comps_to_remove) {
    solution->Remove(cycle);
  }

  // connect remaining
  for (auto e : edges) {
    auto path = e.ab_path;
    auto cycle = PathToCycle(path);
    solution->Add(cycle);
  }
  ConnectAdjacentCycles(solution, costs);
}
}
}
