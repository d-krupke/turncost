//
// Created by Dominik Krupke (http://krupke.cc) on 28.05.18.
//

#ifndef GRIDGRAPH_FULL_COVERAGE_SEPARATION_CALLBACK_H
#define GRIDGRAPH_FULL_COVERAGE_SEPARATION_CALLBACK_H
#include "../general_cc_solver/ip_solver.h"
#include "full_coverage_simple_separator.h"
#include "full_coverage_sufficient_separator.h"

namespace turncostcover {
namespace ip_formulation1 {

class FullCoverageSeparationCallback : public IloCplex::LazyConstraintCallbackI {
 public:
  FullCoverageSeparationCallback(IpSolver &solver)
      : LazyConstraintCallbackI(solver.GetCplexEnv()), solver{&solver}
  {
    /* pass */
  }

  virtual void main() override
  {
    std::lock_guard<std::mutex> lock{solver->GetMutex()};
    auto solution = GetSolution();
    std::function<void(IloRange&)> constr_adder = [&](IloRange &constr) { this->add(constr); };
    FullCoverageSimpleSeparator fcss{this->solver};
    fcss.Separate(solution, constr_adder);
    FullCoverageSufficientSeparator fcsufs{this->solver};
    fcsufs.Separate(solution, constr_adder);
  }

 protected:
  CallbackI *duplicateCallback() const override
  {
    return (new FullCoverageSeparationCallback(*solver));
  }

 private:
  IntegralSolution GetSolution()
  {
    std::map<VariableIndex, IloNum> edge_variable_assignments;
    turncostcover::IntegralSolution solution{this->solver->graph_};
    for (auto p: this->solver->GetEdgeVariableMap()) {
      auto val = static_cast<int>(std::lround(this->getValue(p.second)));
      solution.Add(p.first, val);
    }
    return solution;
  }

  IpSolver *solver;
};

}
}

#endif //GRIDGRAPH_FULL_COVERAGE_SEPARATION_CALLBACK_H
