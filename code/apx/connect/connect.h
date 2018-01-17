//
// Created by Dominik Krupke, http://krupke.cc on 8/29/17.
//

#ifndef GRIDGRAPH_CONNECT_H
#define GRIDGRAPH_CONNECT_H

#include "../../problem_instance/solution/solution.h"
#include "details/connect_penalty_cycle_cover.h"

namespace turncostcover {
namespace apx {

void
ConnectAdjacentCycles(IntegralSolution *solution,
                      Costs costs);

void
ConnectDistantCycles(IntegralSolution *solution,
                     Costs costs);

void
ConnectOrRemoveCycles(
    IntegralSolution *solution,
    Costs costs,
    const std::map<Field, double> &penalties);


}
}

#endif //GRIDGRAPH_CONNECT_H
