//
// Created by Dominik Krupke, http://krupke.cc on 10/11/17.
//

#ifndef GRIDGRAPH_INTEGER_PROGRAMMING_H
#define GRIDGRAPH_INTEGER_PROGRAMMING_H

#include "fc_separation/full_coverage_simple_separator.h"
#include "fc_separation/full_coverage_sufficient_separator.h"
#include "general_cc_solver/ip_solver.h"
#include "../problem_instance/costs.h"
#include "../problem_instance/solution/solution.h"
#include "../apx/connect/connect.h"

namespace turncostcover {
namespace ip {
using namespace turncostcover::ip_formulation1;

IntegralSolution ComputeOptimalCycleCover(const GridGraph &graph,
                                          Costs costs,
                                          double deadline_s)
{
  IpSolver ip_solver{graph, costs};
  auto apx_cc_solution = apx::ApproximateCycleCover(graph, costs);
  ip_solver.AddSolution(apx_cc_solution);
  ip_solver.Solve(deadline_s);
  return ip_solver.GetSolution();
}

IntegralSolution ComputeOptimalTour(const GridGraph &graph,
                                    Costs costs,
                                    double deadline)
{
  using clock = std::chrono::high_resolution_clock;
  using std::chrono::duration_cast;
  using time_unit = std::chrono::seconds;

  auto start_time = clock::now();
  IpSolver solver{graph, costs};

  //initial solve
  auto apx_solution_tour = apx::ApproximateTour(graph, costs);
  auto apx_solution_cc = apx::ApproximateCycleCover(graph, costs);
  solver.AddSolution(apx_solution_tour, "APXT");
  solver.AddSolution(apx_solution_cc, "APXCC");
  solver.Solve(deadline);
  IntegralSolution ip_solution = solver.GetSolution();
  // TODO: use callbacks instead (not critical).
  while (FullCoverageSimpleSeparator{&solver}.Separate(ip_solution)
      + FullCoverageSufficientSeparation{&solver}.Separate(ip_solution) > 0) {
    solver.AddSolution(apx_solution_tour, "APXT");
    apx::connect::ConnectAdjacentCycles(&ip_solution, costs);
    solver.AddSolution(ip_solution, "ConnectedPreviousSolution");
    auto passed = duration_cast<time_unit>(clock::now() - start_time).count();
    auto remaining_time_s = deadline - passed;
    if (remaining_time_s > 0) { solver.Solve(remaining_time_s); }
    else break;
    ip_solution = solver.GetSolution();
    //solver.set_lower_bound_constraint(ip_solution.GetLowerBound());
  }
  if (ip_solution.GetNumComponents() > 1) {
    std::cout
        << "Final solution of IP is not connected (probably due to timeout). "
        << "Connecting it via heuristic."
        << std::endl;
    apx::connect::ConnectAdjacentCycles(&ip_solution, costs);
  }
  return ip_solution;
}

//TODO subtour and penalty

}
}
#endif //GRIDGRAPH_INTEGER_PROGRAMMING_H
