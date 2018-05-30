//
// Created by Dominik Krupke (http://krupke.cc) on 30.05.18.
//

#include "solver.h"

namespace turncostcover {

IntegralSolution ComputeFullCycleCover(const GridGraph &instance, Costs cost, Algorithm alg, long timeout)
{
  switch (alg) {
    case Algorithm::IP:return ip::ComputeOptimalCycleCover(instance, cost, timeout);
    case Algorithm::KRUPKE_APX:return apx::ApproximateCycleCover(instance, cost);
    case Algorithm::ARKINETAL_APX:return arkinapx::ApproximateFullCycleCoverViaStripCover(instance, cost);
    default:assert(false);
  }
}
IntegralSolution ComputeFullTour(const GridGraph &instance, Costs cost, Algorithm alg, long timeout, bool use_callbacks)
{
  switch (alg) {
    case Algorithm::IP: return ip::ComputeOptimalTour(instance, cost, timeout, use_callbacks);
    case Algorithm::KRUPKE_APX: return apx::ApproximateTour(instance, cost);
    case Algorithm::ARKINETAL_APX: return arkinapx::ApproximateFullTourViaStripCover(instance, cost);
    default:assert(false);
  }
}
IntegralSolution ComputeSubsetCycleCover(const GridGraph &instance,
                                         const std::set<Field> &subset,
                                         Costs cost,
                                         Algorithm alg,
                                         long timeout)
{
  if (alg == Algorithm::ARKINETAL_APX) {
    throw std::invalid_argument("The algorithm of Arkin et al. can only be used for full coverage!");
  }
  switch (alg) {
    case Algorithm::KRUPKE_APX: {
      return apx::ApproximateSubsetCycleCover(instance, subset, cost);
    }
    case Algorithm::IP: {
      return ip::ComputeOptimalSubsetCycleCover(instance, subset, cost, timeout);
    }
    default: assert(false);
  }
}
IntegralSolution ComputeSubsetTour(const GridGraph &instance,
                                   const std::set<Field> &subset,
                                   Costs cost,
                                   Algorithm alg,
                                   long timeout,
                                   bool use_callbacks)
{
  if (alg == Algorithm::ARKINETAL_APX) {
    throw std::invalid_argument("The algorithm of Arkin et al. can only be used for full coverage!");
  }
  switch (alg) {
    case Algorithm::KRUPKE_APX: {
      return apx::ApproximateSubsetTour(instance, subset, cost);
    }
    case Algorithm::IP: {
      return ip::ComputeOptimalSubsetTour(instance, subset, cost, timeout, use_callbacks);
    }
    default: assert(false);
  }
}
std::pair<IntegralSolution, PenaltyCoverageObjectiveValue> ComputePenaltyTour(const GridGraph &instance,
                                                                              const std::map<Field,
                                                                                             double> &penalties,
                                                                              Costs cost,
                                                                              Algorithm alg,
                                                                              long timeout,
                                                                              bool use_callbacks)
{
  if (alg == Algorithm::ARKINETAL_APX) {
    throw std::invalid_argument("The algorithm of Arkin et al. can only be used for full coverage!");
  }
  IntegralSolution solution;
  switch (alg) {
    case Algorithm::KRUPKE_APX: {
      solution = apx::ApproximatePenaltyTour(instance, penalties, cost);
      break;
    }
    case Algorithm::IP: {
      solution =  ip::ComputeOptimalPenaltyTour(instance, penalties, cost, timeout, use_callbacks);
      break;
    }
    default: assert(false);
  }
  int nr_covered_fields = 0;
  double penalty = 0.0;
  for (auto v: instance.GetVertices()) {
    if (!solution.IsCovered(v)) penalty += penalties.at(v);
    else nr_covered_fields++;
  }
  return {solution, {solution.GetCoverageObjectiveValue(cost), penalty, nr_covered_fields}};
}
std::pair<IntegralSolution, PenaltyCoverageObjectiveValue> ComputePenaltyCycleCover(const GridGraph &instance,
                                                                                    const std::map<Field,
                                                                                                   double> &penalties,
                                                                                    Costs cost,
                                                                                    Algorithm alg,
                                                                                    long timeout)
{
  if (alg == Algorithm::ARKINETAL_APX) {
    throw std::invalid_argument("The algorithm of Arkin et al. can only be used for full coverage!");
  }
  IntegralSolution solution;
  switch (alg) {
    case Algorithm::KRUPKE_APX: {
      solution = apx::ApproximatePenaltyCycleCover(instance, penalties, cost);
      break;
    }
    case Algorithm::IP: {
      solution = ip::ComputeOptimalPenaltyCycleCover(instance, penalties, cost, timeout);
      break;
    }
    default: assert(false);
  }
  int nr_covered_fields = 0;
  double penalty = 0.0;
  for (auto v: instance.GetVertices()) {
    if (!solution.IsCovered(v)) penalty += penalties.at(v);
    else nr_covered_fields++;
  }
  return {solution, {solution.GetCoverageObjectiveValue(cost), penalty, nr_covered_fields}};
}

}