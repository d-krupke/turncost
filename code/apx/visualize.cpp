//
// Created by Dominik Krupke (http://krupke.cc) on 1/11/18.
//

/*
#include <map>
#include <algorithm>
#include <boost/graph/graph_concepts.hpp>
#include <boost/graph/detail/adjacency_list.hpp>
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/filtered_graph.hpp>
#include "./cyclecover/orientation_selection/solution.h"
#include "visualize.h"
#include "../problem_instance/grid_graph.h"
#include "../problem_instance/solution/simple_svg_1.0.0.hpp"

void
visualize_fractional(basic_string<char>
path, turncostcover::GridGraph &graph, std::map<apx::os::VariableIndex, double> coverage,
                     std::map<Field, double> penalty)
{

  auto max_x =
      graph.GetCoordinate(*std::max_element(boost::vertices(graph).first,
                                            boost::vertices(graph).second,
                                            [&](const turncostcover::GridGraph::FieldId &a,
                                                const turncostcover::GridGraph::FieldId &b) {
                                              return graph.GetCoordinate(a).x
                                                  < graph.GetCoordinate(
                                                      b).x;
                                            })).x;
  auto min_x =
      graph.GetCoordinate(*std::max_element(boost::vertices(graph).first,
                                            boost::vertices(graph).second,
                                            [&](const turncostcover::GridGraph::FieldId &a,
                                                const turncostcover::GridGraph::FieldId &b) {
                                              return graph.GetCoordinate(a).x
                                                  > graph.GetCoordinate(
                                                      b).x;
                                            })).x;
  auto max_y =
      graph.GetCoordinate(*std::max_element(boost::vertices(graph).first,
                                            boost::vertices(graph).second,
                                            [&](const turncostcover::GridGraph::FieldId &a,
                                                const turncostcover::GridGraph::FieldId &b) {
                                              return graph.GetCoordinate(a).y
                                                  < graph.GetCoordinate(
                                                      b).y;
                                            })).y;
  auto min_y =
      graph.GetCoordinate(*std::max_element(boost::vertices(graph).first,
                                            boost::vertices(graph).second,
                                            [&](const turncostcover::GridGraph::FieldId &a,
                                                const turncostcover::GridGraph::FieldId &b) {
                                              return graph.GetCoordinate(a).y
                                                  > graph.GetCoordinate(
                                                      b).y;
                                            })).y;

  svg::Document file{path, svg::Dimensions{2.0 + max_x - min_x, 2.0 + max_y - min_y}};

  auto field_stroke = svg::Stroke{0.05, svg::Color::Black};
  auto field_fill = svg::Fill{svg::Color{169, 169, 169}};

  for (auto vp = boost::vertices(graph); vp.first != vp.second; ++vp.first) {
    auto v = *vp.first;
    auto coord = graph.GetCoordinate(v);

    file << svg::Rectangle{
svg::Point{1.0 + coord.x - min_x, 1.0 + coord.y - min_y}, //Position
                           1, //height
                           1, //width
                           field_fill,
                           field_stroke};

  }

  std::map<Field, double> horizontal;
  std::map<Field, double> vertical;
  for (auto x: coverage) {
    auto v = x.first.f;
    if (x.first.IsStraight(graph)) {
      auto w = x.first.n2;
      if (graph.GetCoordinate(v).x != graph.GetCoordinate(w).x) {
        horizontal[v] += x.second;
      } else {
        vertical[v] += x.second;
      }
    } else {
      horizontal[v] += 0.5 * x.second;
      vertical[v] += 0.5 * x.second;
    }
  }

  for (auto vp = boost::vertices(graph); vp.first != vp.second; ++vp.first) {
    auto v = *vp.first;
    auto coord = graph.GetCoordinate(v);
    if (horizontal[v] > 0) {
      file << svg::Line{
svg::Point{1.0 + coord.x - min_x, 0.5 + coord.y - min_y},
                        svg::Point{2.0 + coord.x - min_x, 0.5 + coord.y - min_y},
                        svg::Stroke{0.1 * horizontal[v], svg::Color::Red}};
    }
    if (vertical[v] > 0) {
      file << svg::Line{
svg::Point{1.5 + coord.x - min_x, 0.0 + coord.y - min_y},
                        svg::Point{1.5 + coord.x - min_x, 1.0 + coord.y - min_y},
                        svg::Stroke{0.1 * vertical[v], svg::Color::Red}};
    }

  }
  file.save();

}
void
visualize_rounded(basic_string<char>
path, turncostcover::GridGraph &graph,
                  std::
    map<turncostcover::GridGraph::FieldId, apx::os::Orientations> orientations)
{
  auto max_x =
      graph.GetCoordinate(*std::max_element(boost::vertices(graph).first,
                                            boost::vertices(graph).second,
                                            [&](const turncostcover::GridGraph::FieldId &a,
                                                const turncostcover::GridGraph::FieldId &b) {
                                              return graph.GetCoordinate(a).x
                                                  < graph.GetCoordinate(
                                                      b).x;
                                            })).x;
  auto min_x =
      graph.GetCoordinate(*std::max_element(boost::vertices(graph).first,
                                            boost::vertices(graph).second,
                                            [&](const turncostcover::GridGraph::FieldId &a,
                                                const turncostcover::GridGraph::FieldId &b) {
                                              return graph.GetCoordinate(a).x
                                                  > graph.GetCoordinate(
                                                      b).x;
                                            })).x;
  auto max_y =
      graph.GetCoordinate(*std::max_element(boost::vertices(graph).first,
                                            boost::vertices(graph).second,
                                            [&](const turncostcover::GridGraph::FieldId &a,
                                                const turncostcover::GridGraph::FieldId &b) {
                                              return graph.GetCoordinate(a).y
                                                  < graph.GetCoordinate(
                                                      b).y;
                                            })).y;
  auto min_y =
      graph.GetCoordinate(*std::max_element(boost::vertices(graph).first,
                                            boost::vertices(graph).second,
                                            [&](const turncostcover::GridGraph::FieldId &a,
                                                const turncostcover::GridGraph::FieldId &b) {
                                              return graph.GetCoordinate(a).y
                                                  > graph.GetCoordinate(
                                                      b).y;
                                            })).y;

  svg::Document file{path, svg::Dimensions{2.0 + max_x - min_x, 2.0 + max_y - min_y}};

  auto field_stroke = svg::Stroke{0.05, svg::Color::Black};
  auto field_fill = svg::Fill{svg::Color{169, 169, 169}};

  for (auto vp = boost::vertices(graph); vp.first != vp.second; ++vp.first) {
    auto v = *vp.first;
    auto coord = graph.GetCoordinate(v);

    file << svg::Rectangle{
svg::Point{1.0 + coord.x - min_x, 1.0 + coord.y - min_y}, //Position
                           1, //height
                           1, //width
                           field_fill,
                           field_stroke};

  }


  for (auto vp = boost::vertices(graph); vp.first != vp.second; ++vp.first) {
    auto v = *vp.first;
    auto coord = graph.GetCoordinate(v);
    switch (orientations[v]) {
      case ::apx::os::Orientations::Vertical:
        file << svg::Line{
svg::Point{1.5 + coord.x - min_x, 0.0 + coord.y - min_y},
                          svg::Point{1.5 + coord.x - min_x, 1.0 + coord.y - min_y},
                          svg::Stroke{0.1, svg::Color::Red}};
        break;
      case ::apx::os::Orientations::Horizontal:
        file
                << svg::Line{
svg::Point{1.0 + coord.x - min_x, 0.5 + coord.y - min_y},
                             svg::Point{2.0 + coord.x - min_x, 0.5 + coord.y - min_y},
                             svg::Stroke{0.1, svg::Color::Red}};
        break;
      default:break;
    }
  }
  file.save();
}*/