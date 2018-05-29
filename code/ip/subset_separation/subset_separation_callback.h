//
// Created by Dominik Krupke (http://krupke.cc) on 29.05.18.
//

#ifndef GRIDGRAPH_SUBSET_SEPARATION_CONSTRAINT_H
#define GRIDGRAPH_SUBSET_SEPARATION_CONSTRAINT_H
#include "subset_voronoi_separation.h"
#include "../../utils/cplex.hpp"
#include "subset_simple_separator.h"
#include "subset_sufficient_separator.h"

namespace turncostcover {
namespace ip_formulation1 {
class SubsetSeparationCallback : public IloCplex::LazyConstraintCallbackI {
 public:
  SubsetSeparationCallback(IpSolver &solver, const std::set<turncostcover::Field> &subset)
      : LazyConstraintCallbackI(solver.GetCplexEnv()), solver{&solver}, subset{subset}
  {
    /* pass */
  }

  virtual void main() override
  {
    std::lock_guard<std::mutex> lock{solver->GetMutex()};
    auto solution = GetSolution();
    std::function<void(IloRange & )> constr_adder = [&](IloRange &constr) { this->add(constr); };
    SubsetCoverageSimpleSeparation fcss{this->solver,subset};
    fcss.Separate(solution, constr_adder);
    SubsetSufficientSeparation fcsufs{this->solver, subset};
    fcsufs.Separate(solution, constr_adder);
    SubsetVoronoiSeparation svs{this->solver, subset};
    svs.Separate(solution, constr_adder);
  }

 protected:
  CallbackI *duplicateCallback() const override
  {
    return (new SubsetSeparationCallback(*solver, subset));
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
  const std::set<turncostcover::Field> &subset;
};
}
}

#endif //GRIDGRAPH_SUBSET_SEPARATION_CONSTRAINT_H
