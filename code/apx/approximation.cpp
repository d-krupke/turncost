//
// Created by Dominik Krupke, http://krupke.cc on 9/5/17.
//

#include <cyclecover/orientation_selection/linear_program.h>
#include <cyclecover/orientation_selection/select_orientations.h>
#include <connect/connect.h>
#include <cyclecover/orientation_matching/orientation_matching.h>
#include "approximation.h"
namespace turncostcover {
namespace apx {
IntegralSolution
ApproximateCycleCover(const GridGraph &graph, Costs costs)
{
  auto penalties = create_full_coverage_penalties(graph);
  return ApproximatePenaltyCycleCover(graph, penalties, costs);
}

IntegralSolution
ApproximateSubsetCycleCover(const GridGraph &graph,
                            const std::set<Field> &subset,
                            Costs costs)
{
  auto penalties = create_subset_coverage_penalties(graph, subset);
  return ApproximatePenaltyCycleCover(graph, penalties, costs);
}

IntegralSolution
ApproximateTour(const GridGraph &graph, Costs costs)
{
  auto cc = ApproximateCycleCover(graph, costs);
  ConnectAdjacentCycles(&cc, costs);
  return cc;
}

IntegralSolution
ApproximateSubsetTour(const GridGraph &graph,
                      const std::set<Field> &subset,
                      Costs costs)
{
  auto cc = ApproximateSubsetCycleCover(graph, subset, costs);
  ConnectDistantCycles(&cc, costs);
  return cc;
}

IntegralSolution
ApproximatePenaltyCycleCover(const GridGraph &graph,
                             const std::map<Field,
                                            double> &penalties,
                             Costs costs)
{
  PenaltyLP lpSolver{graph, penalties, costs};
  lpSolver.Solve();
  auto solution = lpSolver.GetSolution();
  auto rounded_solution = SelectOrientations(graph, solution);
  auto local_match_solution = Match(graph, &rounded_solution, costs);
  local_match_solution.SetLowerBound(lpSolver.GetObjectiveValue());
  return local_match_solution;
}

IntegralSolution
ApproximatePenaltyTour(const GridGraph &graph,
                       const std::map<Field, double> &penalties,
                       Costs costs)
{
  auto cc = ApproximatePenaltyCycleCover(graph, penalties, costs);
  ConnectOrRemoveCycles(&cc, costs, penalties);
  return cc;
}
}
}