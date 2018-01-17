/**
 * This is the header for the first formulation for full coverage cycle cover and tours in grid graphs.
 * Details on this formulation can be found in the thesis 'Algorithmic methods for complex dynamic sweeping problems' by Dominik Krupke, 2016
 */
#ifndef TURNCOSTCOVER_APX_CYCLECOVER_LINEAR_PROGRAM_H
#define TURNCOSTCOVER_APX_CYCLECOVER_LINEAR_PROGRAM_H

#include "../../../problem_instance/grid_graph.h"
#include "../../../problem_instance/costs.h"
#include "../../../problem_instance/coverage.h"
#include <ctime>

#include "../../../utils/cplex.hpp"
namespace turncostcover {
namespace apx {


using VariableIndex = Coverage;

std::map<Field, double>
create_full_coverage_penalties(const GridGraph &instance);

std::map<Field, double>
create_subset_coverage_penalties(const GridGraph &instance,
                                 const std::set<Field> &subset);

class PenaltyLP {

 public:
  // Creates the base IP
  PenaltyLP(const GridGraph &instance,
            const std::map<Field, double> &penalties,
            const Costs costs);

  using Solution = std::pair<std::map<VariableIndex, double>,
                             std::map<Field, double>>;//{covering, penalty}
  Solution
  GetSolution();

  double GetObjectiveValue() const { return cplex_.getObjValue(); }

  bool
  Solve();

 private:

  void CreateLinearProgram();

  //Problem instance
  const Costs costs_;
  const GridGraph &instance_;
  const std::map<Field, double> &penalties_;

  //CPLEX
  IloEnv env_;
  IloModel model_;
  IloCplex cplex_;

  std::map<VariableIndex, IloNumVar> variable_map_;
  std::map<Field, IloNumVar> variable_penalty_map_;


};

}
}
#endif // TURNCOSTCOVER_APX_CYCLECOVER_LINEAR_PROGRAM_H
