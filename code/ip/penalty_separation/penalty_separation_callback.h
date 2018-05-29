//
// Created by Dominik Krupke (http://krupke.cc) on 29.05.18.
//

#ifndef GRIDGRAPH_PENALTY_SEPARATION_CALLBACK_H
#define GRIDGRAPH_PENALTY_SEPARATION_CALLBACK_H
#include "penalty_voronoi_separator.h"
#include "penalty_sufficient_separator.h"

namespace turncostcover {
namespace ip_formulation1 {
class PenaltySeparationCallback : public IloCplex::LazyConstraintCallbackI {
 public:
  PenaltySeparationCallback(IpSolver &solver)
      : LazyConstraintCallbackI(solver.GetCplexEnv()), solver{&solver}
  {
    /* pass */
  }

  virtual void main() override
  {
    std::lock_guard<std::mutex> lock{solver->GetMutex()};
    auto solution = GetSolution();
    std::function<void(IloRange & )> constr_adder = [&](IloRange &constr) { this->add(constr); };
    PenaltySimpleSeparator fcss{this->solver};
    fcss.Separate(solution, constr_adder);
    PenaltySufficientSeparator fcsufs{this->solver};
    fcsufs.Separate(solution, constr_adder);
    PenaltyVoronoiSeparator svs{this->solver};
    svs.Separate(solution, constr_adder);
  }

 protected:
  CallbackI *duplicateCallback() const override
  {
    return (new PenaltySeparationCallback(*solver));
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
#endif //GRIDGRAPH_PENALTY_SEPARATION_CALLBACK_H
