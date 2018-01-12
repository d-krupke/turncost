//
// Created by Dominik Krupke, http://krupke.cc on 6/20/16.
//

#include "ip_solver.h"
#include <stdexcept>

namespace turncostcover {
namespace ip_formulation1 {

ip_formulation1::IpSolver::IpSolver(const GridGraph &instance,
                                    const Costs costs)
    : IpSolverCplexBase{instance, costs}
{
  for (const auto v: instance.GetVertices()) {
    penalties_[v] = std::numeric_limits<double>::infinity();
  }
  CreateIp();
}

void
IpSolver::CreateIp()
{
  std::cout
      << "Creating IP Formulation for full coverage using the first formulation..."
      << std::endl;

  AddVariables();
  AddCoverageConstraints();
  AddTransitionConstraints();
  AddObjectiveFunction();

  std::cout << "Created IP Formulation." << std::endl;
}

void
IpSolver::AddVariables()
{
  for (auto v: graph_.GetVertices()) {
    for (auto uvw: ListAllPossibleCoverages(graph_, v)) {
      assert(this->variable_map_.count(uvw) == 0);
      this->variable_map_[uvw] = IloIntVar{this->cplex_env_, 0, 4};
    }
  }
}

void
IpSolver::AddTransitionConstraints()
{
  //in == out
  for (auto e: graph_.GetEdges()) {
    AddTransitionConstraint(e);
  }
}

void
IpSolver::AddTransitionConstraint(GridGraph::edge_descriptor e)
{
  auto v1 = source(e, this->graph_);
  auto v2 = target(e, this->graph_);

  IloExpr edge_constr_expr{this->cplex_env_};

  for (const auto n: this->graph_.GetNeighbors(v1)) {
    auto v_coverage = VariableIndex{v2, v1, n};
    edge_constr_expr +=
        (v_coverage.IsUTurn() ? 2 : 1) * this->variable_map_[v_coverage];
  }

  for (const auto n: this->graph_.GetNeighbors(v2)) {
    auto w_coverage = VariableIndex{v1, v2, n};
    edge_constr_expr +=
        (w_coverage.IsUTurn() ? -2 : -1) * this->variable_map_[w_coverage];
  }

  this->AddToModel(IloRange{this->cplex_env_, 0, edge_constr_expr, 0});
}

void
IpSolver::AddCoverageConstraints()
{
  for (auto v: this->graph_.GetVertices()) {
    AddCoverageConstraint(v);
  }
}

void
IpSolver::AddCoverageConstraint(Field v)
{
  const double penalty = penalties_.at(v);
  assert(penalty >= 0);//only postive penalties are allowed.
  if (penalty == 0) { return; }//no need for coverage constraint

  IloExpr coverage_constr_expr{this->cplex_env_};
  if (penalty < 2.0 * costs_.dist_costs + 4.0 * costs_.turn_costs) {
    std::cout << "create penalty variable" << std::endl;
    //add penalty variable to constraint
    assert(penalty_variable_map_.count(v) == 0);
    penalty_variable_map_[v] = IloBoolVar(this->cplex_env_);
    coverage_constr_expr += 4 * penalty_variable_map_.at(v);
  }

  for (auto cov: ListAllPossibleCoverages(this->graph_, v)) {
    coverage_constr_expr += 1 * this->variable_map_[cov];
  }

  //every field is covered at least once, at most four times
  this->AddToModel(IloRange{this->cplex_env_, 1, coverage_constr_expr, 4});
}

void
IpSolver::AddObjectiveFunction()
{
  IloExpr obj_expr(this->cplex_env_);
  for (auto v: graph_.GetVertices()) {
    for (auto uvw: ListAllPossibleCoverages(graph_, v)) {
      CostUnit turn_costs =
          uvw.NumberOfUTurns(this->graph_) * this->costs_.turn_costs;
      CostUnit dist_costs = 1 * this->costs_.dist_costs;
      obj_expr += (turn_costs + dist_costs) * this->variable_map_[uvw];
    }
    if (penalty_variable_map_.count(v) > 0) {
      obj_expr += this->penalties_.at(v) * penalty_variable_map_.at(v);
      std::cout << "add penalty variable " << this->penalties_.at(v)
                << std::endl;
    }
  }
  //std::cout << obj_expr << std::endl;
  this->AddToModel(IloMinimize(this->cplex_env_, obj_expr));
}
IpSolver::IpSolver(const GridGraph &instance,
                   const std::set<turncostcover::Field> &subset,
                   const turncostcover::Costs costs)
    : IpSolverCplexBase{instance, costs}
{
  for (const auto v: instance.GetVertices()) {
    if (subset.count(v) > 0) {
      penalties_[v] = std::numeric_limits<double>::infinity();
    } else {
      penalties_[v] = 0.0;
    }
  }
  CreateIp();
}
IpSolver::IpSolver(const GridGraph &instance,
                   const std::map<turncostcover::Field, double> &penalties,
                   const turncostcover::Costs costs)
    : IpSolverCplexBase{instance, costs}
{
  assert(instance.GetNumVertices() == penalties.size());
  for (const auto v: instance.GetVertices()) {
    this->penalties_[v] = penalties.at(v);
  }
  CreateIp();
}
void IpSolver::AddSolution(const IntegralSolution &solution,
                           const std::string &name)
{
  IloNumVarArray start_variables{cplex_env_};
  IloNumArray start_values{cplex_env_};

  //coverage variables
  for (const auto f: graph_.GetVertices()) {
    for (const auto c: turncostcover::ListAllPossibleCoverages(graph_, f)) {
      start_variables.add(variable_map_.at(c));
      start_values.add(solution.GetUsageOfCoverage(c));
    }
  }

  //penalty variables
  for (const auto &pp: penalty_variable_map_) {
    start_variables.add(pp.second);
    start_values.add((solution.IsCovered(pp.first) ? 0 : 1));
  }
  cplex_solver_.addMIPStart(start_variables,
                           start_values,
                           IloCplex::MIPStartEffort::MIPStartRepair,
                           name.c_str());
}
IntegralSolution IpSolver::GetSolution()
{
  std::map<VariableIndex, IloNum> edge_variable_assignments;
  turncostcover::IntegralSolution solution{this->graph_};
  for (auto p: variable_map_) {
    auto val = static_cast<int>(std::lround(cplex_solver_.getValue(p.second)));
    solution.Add(p.first, val);
  }
  solution.SetLowerBound(GetLowerBound());
  return solution;
}

}
}