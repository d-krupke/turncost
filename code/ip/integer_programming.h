//
// Created by Dominik Krupke, http://krupke.cc on 10/11/17.
//

#ifndef GRIDGRAPH_INTEGER_PROGRAMMING_H
#define GRIDGRAPH_INTEGER_PROGRAMMING_H

#include "subset_separation/subset_simple_separator.h"
#include "subset_separation/subset_sufficient_separator.h"
#include "subset_separation/subset_voronoi_separation.h"
#include "fc_separation/full_coverage_simple_separator.h"
#include "fc_separation/full_coverage_sufficient_separator.h"
#include "fc_separation/full_coverage_separation_callback.h"
#include "general_cc_solver/ip_solver.h"
#include "../problem_instance/costs.h"
#include "../problem_instance/solution/solution.h"
#include "../apx/connect/connect.h"
#include "./subset_separation/subset_separation_callback.h"
#include "./penalty_separation/penalty_separation_callback.h"

namespace turncostcover {
namespace ip {
using namespace turncostcover::ip_formulation1;

IntegralSolution ComputeOptimalCycleCover(const GridGraph &graph,
                                          Costs costs,
                                          double deadline_s);

IntegralSolution ComputeOptimalTour(const GridGraph &graph,
                                    Costs costs,
                                    double deadline, bool cb_mode = true);

IntegralSolution ComputeOptimalSubsetCycleCover(const GridGraph &graph,
                                                const std::set<turncostcover::Field> &subset,
                                                Costs costs,
                                                double deadline);

IntegralSolution ComputeOptimalSubsetTour(const GridGraph &graph,
                                          const std::set<turncostcover::Field> &subset,
                                          Costs costs,
                                          double deadline,
                                          bool cb_mode = true);

IntegralSolution ComputeOptimalPenaltyCycleCover(const GridGraph &graph,
                                                 const std::map<Field, double> &penalties,
                                                 Costs costs,
                                                 double deadline);

IntegralSolution ComputeOptimalPenaltyTour(const GridGraph &graph,
                                           std::map<turncostcover::Field, double> &penalties,
                                           Costs costs,
                                           double deadline,
                                           bool cb_mode = true);

}
}
#endif //GRIDGRAPH_INTEGER_PROGRAMMING_H
