//
// Created by Dominik Krupke, http://krupke.cc on 6/8/17.
//
#include "select_orientations.h"
namespace turncostcover {
namespace apx {

std::map<turncostcover::GridGraph::FieldId, std::tuple<double, double, double>>
GetFractionalOrientations(const GridGraph &g,
                          PenaltyLP::Solution &solution)
{
  std::map<turncostcover::GridGraph::FieldId,
           std::tuple<double, double, double>> fractional_selection;
  for (auto px: solution.second) {
    std::get<2>(fractional_selection[px.first]) = px.second;
  }
  for (auto vp = boost::vertices(g); vp.first != vp.second; ++vp.first) {
    auto v = *vp.first;
    //std::cout << "v(" << v <<"):    ";
    for (auto c: turncostcover::ListUTurns(g, v)) {
      // std::cout << "u("<<std::get<0>(c) <<","<<std::get<1>(c)<<","<<std::get<2>(c)<<")";
      auto x = solution.first[c];
      //std::cout << x << "\t";
      std::get<0>(fractional_selection[v]) += 0.5 * x;
      std::get<1>(fractional_selection[v]) += 0.5 * x;
    }
    for (auto c: turncostcover::ListSimpleTurns(g, v)) {
      //std::cout << "t("<<std::get<0>(c) <<","<<std::get<1>(c)<<","<<std::get<2>(c)<<")";
      auto x = solution.first[c];
      //std::cout << x << "\t";
      std::get<0>(fractional_selection[v]) += 0.5 * x;
      std::get<1>(fractional_selection[v]) += 0.5 * x;
    }
    for (auto c: turncostcover::ListStraightCoverages(g, v)) {
      //std::cout << "s("<<std::get<0>(c) <<","<<std::get<1>(c)<<","<<std::get<2>(c)<<")";
      auto x = solution.first[c];
      // std::cout << x << "\t";
      if (c.IsHorizontal(g)) {
        std::get<0>(fractional_selection[v]) += x;
      } else {
        std::get<1>(fractional_selection[v]) += x;
      }
    }
    //std::cout <<std::endl;
  }
  return fractional_selection;
}

OrientationSolution
SelectOrientations(const GridGraph &g, PenaltyLP::Solution &solution)
{
  std::map<turncostcover::GridGraph::FieldId, Orientations> selection;
  auto frac_selection = GetFractionalOrientations(g, solution);
  for (const auto &vc: frac_selection) {
    auto v = vc.first;
    auto c = vc.second;

    std::vector<double> x = {std::get<0>(c), std::get<1>(c), std::get<2>(c)};
    // std::cout << x[0] << " " << x[1] << " " << x[2] << std::endl;
    auto m = *std::max_element(x.begin(), x.end());
    if (m < 0.33 || std::get<2>(c) == m) {
      selection[v] = Orientations::Penalty;
    } else if (std::get<0>(c) == m) {
      selection[v] = Orientations::Horizontal;
    } else {
      assert(std::get<1>(c) == m);
      selection[v] = Orientations::Vertical;
    }
  }
  return selection;
};
}
}