//
// Created by Dominik Krupke (http://krupke.cc) on 30.05.18.
//
// This file provides all the necessary interfaces to the solvers. Note that the timeout is currently only implemented
// for solving with cplex.
//

#ifndef GRIDGRAPH_SOLVER_H
#define GRIDGRAPH_SOLVER_H

#include "problem_instance/solution/solution.h"
#include "ip/integer_programming.h"
#include "arkin-apx/approximation.h"
namespace turncostcover {

enum class Algorithm {
  IP, KRUPKE_APX, ARKINETAL_APX
};

IntegralSolution ComputeFullCycleCover(const GridGraph &instance,
                                       Costs cost,
                                       Algorithm alg = Algorithm::KRUPKE_APX,
                                       long timeout = (60 * 60 * 48));

IntegralSolution ComputeFullTour(const GridGraph &instance,
                                 Costs cost,
                                 Algorithm alg = Algorithm::KRUPKE_APX,
                                 long timeout = (60 * 60 * 48),
                                 bool use_callbacks = true);

IntegralSolution ComputeSubsetCycleCover(const GridGraph &instance,
                                         const std::set<Field> &subset,
                                         Costs cost,
                                         Algorithm alg = Algorithm::KRUPKE_APX,
                                         long timeout = (60 * 60 * 48));

IntegralSolution ComputeSubsetTour(const GridGraph &instance,
                                   const std::set<Field> &subset,
                                   Costs cost,
                                   Algorithm alg = Algorithm::KRUPKE_APX,
                                   long timeout = (60 * 60 * 48),
                                   bool use_callbacks = true);

struct PenaltyCoverageObjectiveValue {
  double sum;
  double coverage_costs;
  double penalty_costs;
  size_t number_of_covered_pixel;

  PenaltyCoverageObjectiveValue(double cov, double pen, size_t n) : sum{cov + pen},
                                                                    coverage_costs{cov},
                                                                    penalty_costs{pen},
                                                                    number_of_covered_pixel{n} {/*pass*/}
};

std::pair<IntegralSolution, PenaltyCoverageObjectiveValue>
ComputePenaltyCycleCover(const GridGraph &instance,
                         const std::map<Field, double> &penalties,
                         Costs cost,
                         Algorithm alg = Algorithm::KRUPKE_APX,
                         long timeout = (60 * 60 * 48));

std::pair<IntegralSolution, PenaltyCoverageObjectiveValue>
ComputePenaltyTour(const GridGraph &instance,
                   const std::map<Field, double> &penalties,
                   Costs cost,
                   Algorithm alg = Algorithm::KRUPKE_APX,
                   long timeout = (60 * 60 * 48),
                   bool use_callbacks = true);

}

#endif //GRIDGRAPH_SOLVER_H
