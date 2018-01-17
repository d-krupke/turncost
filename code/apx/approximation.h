//
// Created by Dominik Krupke, http://krupke.cc on 9/5/17.
//

#ifndef TURNCOSTCOVER_APPROXIMATION_H
#define TURNCOSTCOVER_APPROXIMATION_H

#include "../problem_instance/solution/solution.h"
#include "../shortest_path/grid_graph.h"

namespace turncostcover {
namespace apx {

IntegralSolution
ApproximateCycleCover(const GridGraph &graph, Costs costs);

IntegralSolution
ApproximateTour(const GridGraph &graph,
                Costs costs);

IntegralSolution
ApproximateSubsetCycleCover(const GridGraph &graph,
                            const std::set<Field> &subset,
                            Costs costs);

IntegralSolution
ApproximateSubsetTour(const GridGraph &graph,
                      const std::set<Field> &subset,
                      Costs costs);

IntegralSolution
ApproximatePenaltyCycleCover(
    const GridGraph &graph,
    const std::map<Field, double> &penalties,
    Costs costs);

IntegralSolution
ApproximatePenaltyTour(const GridGraph &graph,
                       const std::map<Field, double> &penalties,
                       Costs costs);
}  //namespace apx
}

#endif //TURNCOSTCOVER_APPROXIMATION_H
