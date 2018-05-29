/**
 * This is the header for the first formulation for full coverage cycle cover and tours in grid graphs.
 * Details on this formulation can be found in the thesis 'Algorithmic methods for complex dynamic sweeping problems' by Dominik Krupke, 2016
 */
#ifndef KRUPKE_IP_F1_H
#define KRUPKE_IP_F1_H

#include "../../utils/cplex.hpp"
#include "../../problem_instance/grid_graph.h"
#include <ctime>
#include <mutex>
#include "ip_solver_cplex_base.h"
#include "../../problem_instance/coverage.h"
#include "../../problem_instance/solution/solution.h"
#include "../../apx/approximation.h"

namespace turncostcover {

namespace ip_formulation1 {

using VariableIndex = Coverage;

class IpSolver : public IpSolverCplexBase {

 public:
  /// Creates the base IP
  IpSolver(const GridGraph &instance, const Costs costs);

  IpSolver(const GridGraph &instance,
           const std::set<turncostcover::Field> &subset,
           const turncostcover::Costs costs);

  IpSolver(const GridGraph &instance,
           const std::map<turncostcover::Field, double> &penalties,
           const turncostcover::Costs costs);

  void
  AddSolution(const IntegralSolution &solution,
              const std::string &name = "CustomStart");

  IntegralSolution GetSolution();

  bool HasToBeCovered(Field f) const { return GetPenalty(f) > 0.0; }

  const IloNumVar GetCoverageVariable(VariableIndex v) const
  {
    return variable_map_.at(v);
  }

  bool HasPenaltyVariable(Field f) const
  {
    return penalty_variable_map_.count(f) > 0;
  }

  const IloBoolVar GetPenaltyVariable(Field f) const
  {
    return penalty_variable_map_.at(f);
  }

  double GetPenalty(turncostcover::Field f) const { return penalties_.at(f); }

  ///TODO ugly but the callback needs access to the variable map
  std::map<VariableIndex, IloIntVar> &GetEdgeVariableMap() { return variable_map_; };
  std::mutex &GetMutex() { return mutex; }
 protected:

  void CreateIp();
  void AddVariables();

  /// Enforces every cycle coverage to be consistent, i.e., everything is a
  /// cycle and there are no dead ends.
  void AddTransitionConstraints();

  /// Adds a constraint to the ip that everything that for e=vw everything that
  /// leaves v through w has a partner in w and vice versa.
  /// Or in simple `in==out'
  void AddTransitionConstraint(GridGraph::edge_descriptor e);

  /// Enforces every field to be covered.
  virtual void AddCoverageConstraints();

  void AddCoverageConstraint(Field v);
  void AddObjectiveFunction();



 private:
  std::mutex mutex;
  std::map<turncostcover::Field, IloIntVar> penalty_variable_map_;
  std::map<turncostcover::Field, double> penalties_;
  std::map<VariableIndex, IloIntVar> variable_map_;
};

}
}

#endif // KRUPKE_IP_F1_H
