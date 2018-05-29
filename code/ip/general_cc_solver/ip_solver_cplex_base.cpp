//
// Created by Dominik Krupke, http://krupke.cc on 4/27/17.
//

#include "ip_solver_cplex_base.h"
namespace turncostcover {
namespace ip_formulation1 {

std::string to_string(const IpSolverCplexBase::ReturnTypes &rt)
{
  using RT = IpSolverCplexBase::ReturnTypes;
  if (rt == RT::SUCCESSFUL) { return "SUCCESSFUL"; }
  if (rt == RT::ERROR) { return "ERROR"; }
  if (rt == RT::OUT_OF_TIME) { return "OUT_OF_TIME"; }
  if (rt == RT::UNSOLVED) { return "UNSOLVED"; }
  throw std::string("Undefined ip::BaseSolver::ReturnTypes in to_string()");
}

std::basic_ostream<char, std::char_traits<char>> &
operator<<(std::ostream &os, const IpSolverCplexBase::ReturnTypes &rt)
{
  os << to_string(rt);
  return os;
}

IpSolverCplexBase::ReturnTypes IpSolverCplexBase::Solve(double max_time_s)
{
  statistics_.solve_iterations++;
  auto start_time = clock::now();

  if (max_time_s < std::numeric_limits<double>::max()) {
    std::cout << "Solving with time limit " << max_time_s << " seconds"
              << std::endl;
    if (max_time_s <= 0) {
      std::cout << "Abort Solve(): OUT_OF_TIME" << std::endl;
      return status_ = ReturnTypes::OUT_OF_TIME;
    }
    cplex_solver_.setParam(IloCplex::TiLim, max_time_s);
  }

  std::cout << "Solving IP...." << std::endl;
  //Solve
  try {
    if (!cplex_solver_.solve()) {
      std::cout << "Failed Solve(): " << cplex_solver_.getCplexStatus()
                << std::endl;
      return status_ = ReturnTypes::ERROR;
    }
    lower_bound_ = std::max(lower_bound_, cplex_solver_.getBestObjValue());
  } catch (IloException& e) {
    std::cerr << "Error while solving: " << e << std::endl;
    std::cout << "Failed Solve(): " << cplex_solver_.getCplexStatus()
              << std::endl;
    return status_ = ReturnTypes::ERROR;
  }
  std::cout << "CPLEX_STATUS=" << cplex_solver_.getCplexStatus() << std::endl;

  auto stop_time = clock::now();
  auto duration = duration_cast<time_unit>(stop_time - start_time);
  statistics_.duration_solving += duration;
  if (statistics_.duration_solving_initial.count() == 0) {
    statistics_.duration_solving_initial = duration;
  }

  status_ = EvaluateSolveStatus();
  return status_;
}
IpSolverCplexBase::IpSolverCplexBase(const GridGraph &instance,
                                     const turncostcover::Costs costs)
    :
    costs_{costs},
    graph_{instance},
    cplex_model_{cplex_env_},
    cplex_solver_{cplex_model_}
{
  std::cout << "Instance with turn_cost=" << costs.turn_costs
            << " and dist_cost=" << costs.dist_costs << std::endl;
  cplex_solver_.setParam(IloCplex::EpInt, 0);
  cplex_solver_.setParam(IloCplex::EpGap, 0);
  cplex_solver_.setParam(IloCplex::EpOpt, 1e-9);
  cplex_solver_.setParam(IloCplex::EpAGap, 0);
}
IpSolverCplexBase::ReturnTypes IpSolverCplexBase::EvaluateSolveStatus()
{
  if (cplex_solver_.getCplexStatus() == IloCplex::AbortTimeLim) {
    std::cout << "Abort Solve(): OUT_OF_TIME" << std::endl;
    return ReturnTypes::OUT_OF_TIME;
  } else if (cplex_solver_.getCplexStatus() == IloCplex::Optimal) {
    std::cout << "Solved IP to optimality with solution: "
              << cplex_solver_.getObjValue() << std::endl;
    return ReturnTypes::SUCCESSFUL;
  } else if (cplex_solver_.getCplexStatus() == IloCplex::OptimalTol) {
    std::cout << "Solved IP within tolerance with solution: "
              << cplex_solver_.getObjValue() << std::endl;
    return ReturnTypes::SUCCESSFUL;
  } else {
    std::cout << "Failed Solve(): " << cplex_solver_.getCplexStatus()
              << std::endl;
    return ReturnTypes::ERROR;
  }
}
}
}