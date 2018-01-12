//
// Created by Dominik Krupke, http://krupke.cc on 4/27/17.
//

#include "general_cc_solver/ip_solver.h"
#include "full_coverage_simple_separator.h"
namespace turncostcover {
namespace ip_formulation1 {

struct EdgeUsedInSolutionPredicate {
  const GridGraph *gg;
  const IntegralSolution *solution;
 public:
  EdgeUsedInSolutionPredicate() = default;

  EdgeUsedInSolutionPredicate(const GridGraph *gg,
                              const IntegralSolution *solution)
      :
      gg{gg}, solution{solution} {}

  template<typename Edge>
  bool
  operator()(const Edge &e) const
  {
    const auto neighbors =
        boost::make_iterator_range(boost::adjacent_vertices(boost::source(e,
                                                                          *gg),
                                                            *gg));
    for (const auto u: neighbors) {
      const auto v = boost::source(e, *gg);
      const auto w = boost::target(e, *gg);
      if (solution->GetUsageOfCoverage({u, v, w}) > 0) {
        return true;
      }
    }
    return false;
  }
};

size_t
FullCoverageSimpleSeparator::Separate(const IntegralSolution &solution)
{
  std::cout
      << "Searching for applications of simple insufficient separation constraint...\n";
  size_t cuts_added = 0;

  std::map<int, std::set<Field>> components = GetComponents(solution);
  if (components.size() > 1) {

    //create constraints
    for (const auto &comp: components) {
      cuts_added += CreateConstraint(solution, comp.second);

      std::cout << "Added " << cuts_added << " new constraints.\n";
    }
  } else {
    std::cout << "No application of this separation possible." << std::endl;
  }
  return cuts_added;
}

size_t
FullCoverageSimpleSeparator::CreateConstraint(
    const IntegralSolution &solution,
    const std::set<Field> &comp_fields)
const
{
  IloExpr constr_expr = CreateLeaveComponentExpression(solution, comp_fields);
  IloRange constr(this->solver_->cplex_env_, 2, constr_expr, IloInfinity);
  this->solver_->AddToModel(constr);
  return 1;
}

IloExpr
FullCoverageSimpleSeparator::CreateLeaveComponentExpression(
    const IntegralSolution &solution,
    const std::set<Field> &comp_fields)
const
{
  IloExpr constr_expr(this->solver_->cplex_env_);

  for (const auto v: comp_fields) {
    for (const auto uvw: ListAllPossibleCoverages(solution.GetGridGraph(),
                                                  v)) {
      const auto u = uvw.n1;
      const auto w = uvw.n2;
      if (comp_fields.count(u) == 0 || comp_fields.count(w) == 0) {
        if (uvw.IsUTurn()) {
          constr_expr += 2 * this->solver_->GetCoverageVariable(
              uvw);
        } else { constr_expr += 1 * this->solver_->GetCoverageVariable(uvw); }
      }
    }
  }
  return constr_expr;
}

std::map<int, std::set<Field>>
FullCoverageSimpleSeparator::GetComponents(
    const IntegralSolution &solution)
const
{
  boost::filtered_graph<GridGraph, EdgeUsedInSolutionPredicate>
      connected_graph(solver_->graph_,
                      EdgeUsedInSolutionPredicate{
                          &(solver_->graph_), &solution});

  //Calculate connected components in InternalGraph
  std::vector<int> components_map(boost::num_vertices(connected_graph));
  const int num_comps =
      boost::connected_components(connected_graph, &components_map[0]);
  std::map<int, std::set<Field>> components;
  for (auto v_it = vertices(solver_->graph_); v_it.first != v_it.second;
       ++v_it.first) {
    auto v = *(v_it.first);
    if (solution.IsCovered(v)) {
      auto v_comp_id =
          components_map[get(boost::vertex_index, solver_->graph_, v)];
      components[v_comp_id].insert(v);
    }
  }
  return components;
}

FullCoverageSimpleSeparator::FullCoverageSimpleSeparator(
    IpSolver *solver) : solver_(solver) {}
}
}