//
// Created by Dominik Krupke, http://krupke.cc on 7/11/17.
//

#ifndef TURNCOSTCOVER_APX_CONNECT_GREEDY_CONNECT_H
#define TURNCOSTCOVER_APX_CONNECT_GREEDY_CONNECT_H

#include "../../../problem_instance/grid_graph.h"
#include "../../../problem_instance/coverage.h"
#include "../../../problem_instance/solution/solution.h"
#include "../../../problem_instance/costs.h"
namespace turncostcover {
namespace apx {

/**
 * This function connects adjacent cycles by simple local modifications. Cheap connections are tried first.
 *
 * @param cycles This input can be obtained by using collect_cycles
 * @return
 */
void GreedyConnectAdjacentCycles(IntegralSolution *solution, Costs costs);

namespace details {
struct CycleMerge {
  Coverage cov1;
  Coverage cov2;
  CostUnit
      cost = std::numeric_limits<CostUnit>::max();

  bool
  IsFeasible()
  {
    return cost < std::numeric_limits<CostUnit>::max();
  }
};

CycleMerge
FindCheapestMergeOfCoverage(Coverage cov,
                            const IntegralSolution &solution,
                            Costs costs);

CycleMerge
FindCheapestMerge(const IntegralSolution &solution,
                  Costs costs,
                  CostUnit return_immediately_if_leq = 0);
}
}
}

#endif //TURNCOSTCOVER_APX_CONNECT_GREEDY_CONNECT_H
