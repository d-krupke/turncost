#ifndef FC_BASESOLVER_H
#define FC_BASESOLVER_H

#include "../../utils/cplex.hpp"
#include <ctime>
#include "../../problem_instance/grid_graph.h"
#include "../../problem_instance/costs.h"

namespace turncostcover {

namespace ip_formulation1 {
//time definitions
using clock = std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using time_unit = std::chrono::milliseconds;

class IpSolverCplexBase {
 public:
  enum class ReturnTypes {
    SUCCESSFUL, OUT_OF_TIME, ERROR, UNSOLVED
  };

  struct Statistics {
    time_unit duration_solving;
    time_unit duration_creation;
    time_unit duration_solving_initial;

    long solve_iterations;
    long cuts_added;
  };

  void
  AddToModel(IloRange c)
  {
    cplex_model_.add(c);
    if (status_ == ReturnTypes::UNSOLVED) { statistics_.cuts_added++; }
  }

  void AddToModel(IloObjective c) { cplex_model_.add(c); }

  Statistics GetStatics() { return statistics_; }

  //Solves the current integer program
  ReturnTypes Solve(double max_time_s = std::numeric_limits<double>::max());

  // Returns the current objective value of the integer program or -1
  double GetObjectiveValue() { return cplex_solver_.getObjValue(); }

  double GetLowerBound()
  {
    assert(lower_bound_ >= cplex_solver_.getBestObjValue());
    return lower_bound_;
  }

  IpSolverCplexBase(const GridGraph &instance, const Costs costs);
  virtual ~IpSolverCplexBase() = default;

 private:
  ReturnTypes EvaluateSolveStatus();

 protected:
  //instance
  Statistics statistics_;
  const Costs costs_;
  ReturnTypes status_ = ReturnTypes::UNSOLVED;
  double lower_bound_ = 0;

 public:

  //Problem instance
  const GridGraph &graph_;
  //CPLEX
  IloEnv cplex_env_;
  IloModel cplex_model_;
  IloCplex cplex_solver_;
};

std::string
to_string(const IpSolverCplexBase::ReturnTypes &rt);

std::ostream &
operator<<(std::ostream &os, const IpSolverCplexBase::ReturnTypes &rt);
}
}

#endif
