//
// Created by Dominik Krupke, http://krupke.cc on 9/12/17.
//

#include <gtest/gtest.h>
#include "../../problem_instance/coordinate.h"
#include "../../problem_instance/coverage.h"
#include "subset_voronoi_separation.h"
#include "voronoi_extender.h"

TEST(IpSubsetVoronoiExtender, BaseTest)
{
  using namespace turncostcover;
  std::set<Coordinate> coords = {{0, 0},
                                 {1, 0},
                                 {2, 0},
                                 {3, 0},
                                 {4, 0},
                                 {5, 0},
                                 {6, 0},
                                 {7, 0}
  };

  turncostcover::GridGraph graph{coords};

  turncostcover::IntegralSolution int_sol{graph};

  auto coord_coverage = [&](const Coordinate a, const Coordinate b, const Coordinate c) -> turncostcover::Coverage {
      return {graph.GetVertex(a), graph.GetVertex(b), graph.GetVertex(c)};
  };

  //comp_a
  int_sol.Add(coord_coverage({1, 0}, {0, 0}, {1, 0}), 1);
  int_sol.Add(coord_coverage({0, 0}, {1, 0}, {0, 0}), 1);

  //comp_b
  int_sol.Add(coord_coverage({7, 0}, {6, 0}, {7, 0}), 1);
  int_sol.Add(coord_coverage({6, 0}, {7, 0}, {6, 0}), 1);

  ASSERT_EQ(int_sol.GetNumComponents(), 2);

  std::map<int, std::set<turncostcover::Field>> components;
  components[1].insert(graph.GetVertex({1, 0}));
  components[1].insert(graph.GetVertex({0, 0}));
  components[2].insert(graph.GetVertex({6, 0}));
  components[2].insert(graph.GetVertex({7, 0}));

  turncostcover::ip_formulation1::details::VoronoiExtender ve{graph};
  ve.Extend(&components);
  ASSERT_EQ(components.size(), 2);
  ASSERT_EQ(components.at(1).size(), 4);
  ASSERT_EQ(components.at(2).size(), 4);
  ASSERT_EQ(components.at(1).count(graph.GetVertex({0, 0})), 1);
  ASSERT_EQ(components.at(1).count(graph.GetVertex({1, 0})), 1);
  ASSERT_EQ(components.at(1).count(graph.GetVertex({2, 0})), 1);
  ASSERT_EQ(components.at(1).count(graph.GetVertex({3, 0})), 1);
  ASSERT_EQ(components.at(2).count(graph.GetVertex({3, 0})), 0);
  ASSERT_EQ(components.at(2).count(graph.GetVertex({4, 0})), 1);
  ASSERT_EQ(components.at(2).count(graph.GetVertex({5, 0})), 1);
  ASSERT_EQ(components.at(2).count(graph.GetVertex({6, 0})), 1);
  ASSERT_EQ(components.at(2).count(graph.GetVertex({7, 0})), 1);


}