//
// Created by Dominik Krupke, http://krupke.cc on 10/2/17.
//

#ifndef TURNCOSTCOVER_ARKINAPX_STRIP_MATCHING_SOLVER_H
#define TURNCOSTCOVER_ARKINAPX_STRIP_MATCHING_SOLVER_H

#include "../../problem_instance/grid_graph.h"
#include "../../problem_instance/costs.h"
#include "../../shortest_path/turn_cost_grid_dijkstra.h"
#include "../../apx/connect/details/tcg_grid_graph_wrapper.h"
#include "../../apx/cyclecover/orientation_matching/blossom5-v205/PerfectMatching.h"
#include "../../problem_instance/solution/solution.h"
#include "../../problem_instance/direction.h"
namespace turncostcover {
namespace arkinapx {
namespace details {

class StripMatchingSolver {
 public:
  StripMatchingSolver(const GridGraph &graph, Costs costs) :
      graph_(graph), costs_{costs}, wrapped_grid_graph_{graph}
  {
    /* - */
  }

  IntegralSolution Solve();

 private:
  std::unique_ptr<PerfectMatching> CalculateMatching() const;

  CostUnit GetCostBetweenEndpoints(int i, int j) const;

  std::vector<Coordinate>
  GetShortestPathBetweenEndpoints(int i, int j)
  const;

  std::vector<turncostgrid::TurnCostGridDijkstra>
  CalculateShortestPathTrees();

  void
  PushbackIfNotAlreadyAtEnd(std::vector<Coordinate> *cycle, Coordinate c)
  const;

  std::vector<std::vector<Coordinate>> ExtractCycles() const;

  unsigned long
  FindUnprocessedId(const std::set<int> &processed_endpoints)
  const;

  void
  AddStripFields(std::vector<Coordinate> *cycle,
                 const Coordinate &e1_coord,
                 const Coordinate &e2_coord)
  const;

  void FillSingleFieldCyclesWithRandomNbr(std::vector<Coordinate> *cycle) const;

  int AdvanceTowards(int x, int t) const;

  turncostgrid::Direction Convert(Direction d) const;

  void CleanUp()
  {
    endpoints_.clear();
    wrapped_grid_graph_ = apx::TCGGridGraphWrapper{};
    shortest_path_trees_.clear();
    pm_.release();
  }

 private:
  const GridGraph &graph_;
  Costs costs_;
  std::vector<std::pair<Coordinate, Direction>> endpoints_;
  apx::TCGGridGraphWrapper wrapped_grid_graph_;
  std::vector<turncostgrid::TurnCostGridDijkstra> shortest_path_trees_;
  std::unique_ptr<PerfectMatching> pm_;
};
};
}

}
#endif //TURNCOSTCOVER_ARKINAPX_STRIP_MATCHING_SOLVER_H
