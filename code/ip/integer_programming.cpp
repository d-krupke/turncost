//
// Created by Dominik Krupke, http://krupke.cc on 10/11/17.
//

#include "integer_programming.h"

namespace turncostcover {
namespace ip {
IntegralSolution ComputeOptimalCycleCover(const GridGraph &graph,Costs costs,double deadline_s)
{
  IpSolver ip_solver{graph, costs};
  auto apx_cc_solution = apx::ApproximateCycleCover(graph, costs);
  ip_solver.AddSolution(apx_cc_solution);
  ip_solver.Solve(deadline_s);
  return ip_solver.GetSolution();
}
IntegralSolution ComputeOptimalTour(const GridGraph &graph, Costs costs, double deadline, bool cb_mode)
{
  using clock = std::chrono::high_resolution_clock;
  using std::chrono::duration_cast;
  using time_unit = std::chrono::seconds;

  auto start_time = clock::now();
  IpSolver solver{graph, costs};

  //initial Solve
  auto apx_solution_tour = apx::ApproximateTour(graph, costs);
  auto apx_solution_cc = apx::ApproximateCycleCover(graph, costs);
  solver.AddSolution(apx_solution_tour, "APXT");
  solver.AddSolution(apx_solution_cc, "APXCC");
  FullCoverageSeparationCallback cb{solver};
  if (cb_mode) {
    solver.SetCallback(&cb);
  }
  solver.Solve(deadline);
  IntegralSolution ip_solution = solver.GetSolution();

  if (!cb_mode) {
    while (FullCoverageSimpleSeparator{&solver}.Separate(ip_solution)
        + FullCoverageSufficientSeparator{&solver}.Separate(ip_solution) > 0) {
      solver.AddSolution(apx_solution_tour, "APXT");
      apx::ConnectAdjacentCycles(&ip_solution, costs);
      solver.AddSolution(ip_solution, "ConnectedPreviousSolution");
      auto passed = duration_cast<time_unit>(clock::now() - start_time).count();
      auto remaining_time_s = deadline - passed;
      if (remaining_time_s > 0) { solver.Solve(remaining_time_s); }
      else break;
      ip_solution = solver.GetSolution();
      //solver.set_lower_bound_constraint(ip_solution.GetLowerBound());
    }
  }

  if (ip_solution.GetNumComponents() > 1) {
    std::cout
        << "Final solution of IP is not connected (probably due to timeout). "
        << "Connecting it via heuristic."
        << std::endl;
    apx::ConnectAdjacentCycles(&ip_solution, costs);
  }
  return ip_solution;
}
IntegralSolution ComputeOptimalSubsetTour(const GridGraph &graph,
                                          const std::set<turncostcover::Field> &subset,
                                          Costs costs,
                                          double deadline,
                                          bool cb_mode)
{
  using clock = std::chrono::high_resolution_clock;
  using std::chrono::duration_cast;
  using time_unit = std::chrono::seconds;

  auto start_time = clock::now();
  IpSolver solver{graph, subset, costs};
  //Add apx to initial solutions
  auto apx_solution_t = apx::ApproximateSubsetTour(graph, subset, costs);
  auto apx_solution_cc = apx::ApproximateSubsetCycleCover(graph, subset, costs);
  solver.AddSolution(apx_solution_t, "apx-t");
  solver.AddSolution(apx_solution_cc, "apx-cc");

  SubsetSeparationCallback cb{solver, subset};
  if(cb_mode){
    solver.SetCallback(&cb);
  }

  //initial Solve
  solver.Solve();
  auto solution = solver.GetSolution();

  if(!cb_mode) {
    //eliminate subtours
    turncostcover::ip_formulation1::SubsetCoverageSimpleSeparation
        s1{&solver, subset};
    turncostcover::ip_formulation1::SubsetSufficientSeparation
        s2{&solver, subset};
    turncostcover::ip_formulation1::SubsetVoronoiSeparation s3{&solver, subset};
    while (solution.GetNumComponents() > 1) {
      std::cout << "Added " << s1.Separate(solution)
                << " simple separation constraints" << std::endl;
      std::cout << "Added " << s2.Separate(solution)
                << " sufficient separation constraints" << std::endl;
      std::cout << "Added " << s3.Separate(solution)
                << " voronoi separation constraints" << std::endl;
      auto connected_solution = solution;
      apx::ConnectDistantCycles(&solution, costs);
      solver.AddSolution(connected_solution, "connected");
      solver.AddSolution(apx_solution_t, "apx-t");
      auto passed = duration_cast<time_unit>(clock::now() - start_time).count();
      auto remaining_time_s = deadline - passed;
      if (remaining_time_s > 0) { solver.Solve(remaining_time_s); } else {break;}
      solution = solver.GetSolution();
    }
  }
  if (solution.GetNumComponents() > 1) {
    std::cout
        << "Final solution of IP is not connected (probably due to timeout). "
        << "Connecting it via heuristic."
        << std::endl;
    apx::ConnectDistantCycles(&solution, costs);
  }
  return solution;
}

IntegralSolution ComputeOptimalSubsetCycleCover(const GridGraph &graph,
                                                const std::set<turncostcover::Field> &subset,
                                                Costs costs,
                                                double deadline)
{
  IpSolver ip_solver{graph, subset, costs};
  auto apx_cc_solution = apx::ApproximateSubsetCycleCover(graph, subset, costs);
  ip_solver.AddSolution(apx_cc_solution);
  ip_solver.Solve(deadline);
  return ip_solver.GetSolution();
}
IntegralSolution ComputeOptimalPenaltyTour(const GridGraph &graph,
                                           const std::map<Field, double> &penalties,
                                           Costs costs,
                                           double deadline,
                                           bool cb_mode)
{
  using clock = std::chrono::high_resolution_clock;
  using std::chrono::duration_cast;
  using time_unit = std::chrono::seconds;

  auto start_time = clock::now();
  turncostcover::ip_formulation1::IpSolver solver{graph, penalties, costs};


  auto apx_solution_t = apx::ApproximatePenaltyTour(graph, penalties, costs);
  std::cout << "APX-T=" << apx_solution_t.GetCoverageObjectiveValue(costs) << std::endl;
  auto apx_solution_cc =
      apx::ApproximatePenaltyCycleCover(graph, penalties, costs);
  std::cout << "APX-CC=" << apx_solution_cc.GetCoverageObjectiveValue(costs) << std::endl;
  solver.AddSolution(apx_solution_t, "apx-t");
  solver.AddSolution(apx_solution_cc, "apx-cc");

  PenaltySeparationCallback cb{solver};
  if(cb_mode) {
    solver.SetCallback(&cb);
  }

  solver.Solve(deadline);
  auto solution = solver.GetSolution();

  if(!cb_mode) {
    turncostcover::ip_formulation1::PenaltySimpleSeparator s1{&solver};
    turncostcover::ip_formulation1::PenaltySufficientSeparator s2{&solver};
    turncostcover::ip_formulation1::PenaltyVoronoiSeparator s3{&solver};

    while (solution.GetNumComponents() > 1) {
      std::cout << "Added " << s1.Separate(solution)
                << " simple separation constraints" << std::endl;
      std::cout << "Added " << s2.Separate(solution) << " sufficient separation constraints" << std::endl;
      std::cout << "Added " << s3.Separate(solution)
                << " voronoi separation constraints" << std::endl;
      auto connected_solution = solution;
      apx::ConnectDistantCycles(&solution, costs);
      solver.AddSolution(connected_solution, "connected");
      solver.AddSolution(apx_solution_t, "apx-t");
      auto passed = duration_cast<time_unit>(clock::now() - start_time).count();
      auto remaining_time_s = deadline - passed;
      if (remaining_time_s > 0) { solver.Solve(remaining_time_s); } else {break;}
      solution = solver.GetSolution();
    }
  }
  if (solution.GetNumComponents() > 1) {
    std::cout
        << "Final solution of IP is not connected (probably due to timeout). "
        << "Connecting it via heuristic."
        << std::endl;
    apx::ConnectOrRemoveCycles(&solution, costs, penalties);
  }
  return solution;
}
IntegralSolution ComputeOptimalPenaltyCycleCover(const GridGraph &graph,
                                                 const std::map<Field, double> &penalties,
                                                 Costs costs,
                                                 double deadline)
{
  IpSolver ip_solver{graph, penalties, costs};
  auto apx_cc_solution = apx::ApproximatePenaltyCycleCover(graph, penalties, costs);
  ip_solver.AddSolution(apx_cc_solution);
  ip_solver.Solve(deadline);
  return ip_solver.GetSolution();
}
}
}