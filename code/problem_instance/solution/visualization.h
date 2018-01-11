//
// Created by Dominik Krupke, http://krupke.cc on 5/12/17.
//

#ifndef TURNCOSTCOVER_SOLUTION_VISUALIZATION_H
#define TURNCOSTCOVER_SOLUTION_VISUALIZATION_H

#include "../grid_graph.h"
#include "../coverage.h"
#include "solution.h"

namespace turncostcover {

void
VisualizeIntegral(const GridGraph &g,
                  const std::map<Coverage, double> &edge_variable_assignments,
                  std::string filename);

void
VisualizeIntegral(GridGraph &g,
                  IntegralSolution &solution,
                  std::string filename);

void
VisualizeIntegralDensity(const GridGraph &g,
                         const IntegralSolution &solution,
                         const std::map<Field, double> &penalties,
                         std::string filename);
}
#endif //TURNCOSTCOVER_SOLUTION_VISUALIZATION_H
