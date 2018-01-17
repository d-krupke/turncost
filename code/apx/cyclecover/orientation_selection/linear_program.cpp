//
// Created by Dominik Krupke, http://krupke.cc on 6/20/16.
//

#include "linear_program.h"
namespace turncostcover {
namespace apx {

PenaltyLP::PenaltyLP(const GridGraph &instance,
                     const std::map<Field, double> &penalties,
                     const turncostcover::Costs costs) :
    costs_{costs},
    instance_{instance},
    penalties_{penalties},
    model_{env_},
    cplex_{model_} {
  CreateLinearProgram();
}

void
PenaltyLP::CreateLinearProgram() {
  IloExpr obj_expr(env_);

  for (auto vitp = boost::vertices(instance_); vitp.first != vitp.second;
       ++vitp.first) {

    auto v = *vitp.first;
    assert(penalties_.count(v));
    auto p = penalties_.at(v);
    assert(p >= 0);
    const bool is_optional = (p == 0.0);
    const bool is_necessary = (p == std::numeric_limits<double>::infinity());
    const bool use_penalty_variable = !(is_necessary || is_optional);

    IloExpr coverage_constr_expr{env_};

    if (use_penalty_variable) {
      auto pv = IloNumVar{env_, 0, IloInfinity};
      variable_penalty_map_[v] = pv;
      coverage_constr_expr += pv;
      obj_expr += penalties_.at(v) * pv;
    }

    for (auto witp = boost::adjacent_vertices(v, instance_);
         witp.first != witp.second; ++witp.first) {
      auto w = *witp.first;
      for (auto uitp = boost::adjacent_vertices(v, instance_);
           uitp.first != uitp.second; ++uitp.first) {
        auto u = *uitp.first;

        auto uvw = VariableIndex{u, v, w};
        if (variable_map_.count(uvw) > 0) { continue; }
        variable_map_[uvw] = IloNumVar{env_, 0};

        if (uvw.IsStraight(instance_)) {
          obj_expr += costs_.dist_costs * variable_map_[uvw];
        } else if (uvw.IsSimpleTurn(instance_)) {
          obj_expr += (costs_.dist_costs + costs_.turn_costs) * variable_map_[uvw];
        } else if (uvw.IsUTurn()) {
          obj_expr +=
              (costs_.dist_costs + 2 * costs_.turn_costs) * variable_map_[uvw];
        } else { assert(false); }

        coverage_constr_expr += 1 * variable_map_[uvw];
      }
    }

    if (!is_optional) {
      model_.add(IloRange{env_, 1, coverage_constr_expr, IloInfinity});
    } //every field is covered at least once, at most four times
  }
  model_.add(IloMinimize(env_, obj_expr));

  //in == out
  for (auto eitp = boost::edges(instance_); eitp.first != eitp.second;
       ++eitp.first) {
    auto e = *eitp.first;
    auto v = boost::source(e, instance_);
    auto w = boost::target(e, instance_);

    IloExpr edge_constr_expr{env_};

    for (auto nitp = boost::adjacent_vertices(v, instance_);
         nitp.first != nitp.second; ++nitp.first) {
      auto n = *nitp.first;

      auto v_coverage = VariableIndex{w, v, n};
      edge_constr_expr +=
          (v_coverage.IsUTurn() ? 2 : 1) * variable_map_[v_coverage];
    }

    for (auto nitp = boost::adjacent_vertices(w, instance_);
         nitp.first != nitp.second; ++nitp.first) {
      auto n = *nitp.first;

      auto w_coverage = VariableIndex{v, w, n};
      edge_constr_expr +=
          (w_coverage.IsUTurn() ? -2 : -1) * variable_map_[w_coverage];
    }

    model_.add(IloRange{env_, 0, edge_constr_expr, 0});
  }
}

PenaltyLP::Solution
PenaltyLP::GetSolution() {
  //covering variables
  std::map<VariableIndex, IloNum> edge_variable_assignments;
  for (auto p: variable_map_) {
    IloNum v = cplex_.getValue(p.second);
    edge_variable_assignments[p.first] = v;
    //std::cout << "(" << std::get<0>(p.first) <<", " <<std::get<1>(p.first)<<" , "<<std::get<2>(p.first) <<")= "<<v<<std::endl;
  }

  //penalty variables
  std::map<Field, double> penalty_variable_assignments;
  for (auto vp = boost::vertices(instance_); vp.first != vp.second;
       ++vp.first) {
    auto v = *vp.first;
    if (variable_penalty_map_.count(v) > 0) {
      penalty_variable_assignments[v] =
          cplex_.getValue(variable_penalty_map_[v]);
    } else {
      penalty_variable_assignments[v] = 0;
    }
  }

  return {edge_variable_assignments, penalty_variable_assignments};
}

bool
PenaltyLP::Solve() {
  try {
    return cplex_.solve();
  } catch (...) {
    std::cerr << "Something happend in CPLEX. Possibly bad." << std::endl;

  }
  return false;
}

std::map<Field, double>
create_full_coverage_penalties(const GridGraph &instance) {
  std::map<Field, double> penalties;

  for (const auto &v: instance.GetVertices()) {
    penalties[v] = std::numeric_limits<double>::infinity();
  }
  return penalties;
}

std::map<Field, double>
create_subset_coverage_penalties(const GridGraph &instance,
                                 const std::set<Field> &subset) {
  std::map<Field, double> penalties;

  for (const auto &v: instance.GetVertices()) {
    if (subset.count(v) > 0) {
      penalties[v] = std::numeric_limits<double>::infinity();
    } else { penalties[v] = 0.0; }
  }
  return penalties;
}

}
}


