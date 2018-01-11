//
// Created by Dominik Krupke, http://krupke.cc on 5/12/17.
//

#include "visualization.h"
#include "./simple_svg_1.0.0.hpp"
namespace turncostcover {
using namespace svg;

class solution_printer {
 public:

  Document *simpleSvg;

  solution_printer(std::string filename, float x_dim, float y_dim);

  void
  drawEdge(turncostcover::Coordinate u,
           turncostcover::Coordinate v,
           turncostcover::Coordinate w,
           int c);

  void
  finish();

  void
  print_field(turncostcover::Coordinate c);
};

Point
north(turncostcover::Coordinate c)
{
  return Point{c.x * 10.0 + 5.0, c.y * 10.0 + 10.0};
}

Point
south(turncostcover::Coordinate c)
{
  return Point{c.x * 10.0 + 5.0, c.y * 10.0};
}

Point
east(turncostcover::Coordinate c)
{
  return Point{c.x * 10.0 + 10.0, c.y * 10.0 + 5.0};
}

Point
west(turncostcover::Coordinate c)
{
  return Point{c.x * 10.0, c.y * 10.0 + 5.0};
}

Point
center(turncostcover::Coordinate c)
{
  return Point{c.x * 10.0 + 5.0, c.y * 10.0 + 5.0};
}

void
solution_printer::drawEdge(turncostcover::Coordinate u,
                           turncostcover::Coordinate v,
                           turncostcover::Coordinate w,
                           int c)
{
  assert(u != v && v != w);
  if (u == w) { //loop
    auto center_u = center(u);
    auto center_v = center(v);
    auto between =
        Point((center_u.x + center_v.x) * 0.5, (center_u.y + center_v.y) * 0.5);
    auto between_between =
        Point(0.5 * (center_v.x + between.x), 0.5 * (center_v.y + between.y));
    *simpleSvg << Line(between, between_between, Stroke(0.5, Color::Green));
    *simpleSvg << Text(between_between,
                       std::to_string(c),
                       Color::Green,
                       Font(2.0, "Verdana"));
  } else if (u.x == w.x || u.y == w.y) { //Straight
    auto center_u = center(u);
    auto center_v = center(v);
    auto center_w = center(w);
    auto uv =
        Point((center_u.x + center_v.x) * 0.5, (center_u.y + center_v.y) * 0.5);
    auto vw =
        Point((center_w.x + center_v.x) * 0.5, (center_w.y + center_v.y) * 0.5);
    auto text_pot = Point(uv.x * 0.1 + vw.x * 0.9, uv.y * 0.1 + vw.y * 0.9);
    *simpleSvg << Line(uv, vw, Stroke(0.3, Color::Red));
    *simpleSvg
        << Text(text_pot, std::to_string(c), Color::Red, Font(2.0, "Verdana"));
  } else {
    auto center_u = center(u);
    auto center_v = center(v);
    auto center_w = center(w);
    auto uv =
        Point((center_u.x + center_v.x) * 0.5, (center_u.y + center_v.y) * 0.5);
    auto vw =
        Point((center_w.x + center_v.x) * 0.5, (center_w.y + center_v.y) * 0.5);
    auto text_pot = Point(uv.x * 0.5 + vw.x * 0.5, uv.y * 0.5 + vw.y * 0.5);
    *simpleSvg << Line(uv, vw, Stroke(0.3, Color::Blue));
    *simpleSvg
        << Text(text_pot, std::to_string(c), Color::Blue, Font(2.0, "Verdana"));
  }
}

void
solution_printer::print_field(turncostcover::Coordinate c)
{
  *simpleSvg << Rectangle(Point(c.x * 10, c.y * 10 + 10),
                          10,
                          10,
                          Fill(Color::Yellow),
                          Stroke(0.5, Color::Black));
}

solution_printer::solution_printer(std::string filename,
                                   float x_dim,
                                   float y_dim) : simpleSvg{
    new Document(filename,
                 Layout(Dimensions(x_dim * 10 + 10, y_dim * 10 + 10),
                        Layout::BottomLeft))}
{
  std::cout << "Creating SVG: " + filename << std::endl;
}

void
solution_printer::finish()
{
  std::cout << "Saved SVG." << std::endl;
  simpleSvg->save();
  delete simpleSvg;
}

void
VisualizeIntegral(const turncostcover::GridGraph &g,
                  const std::map<turncostcover::Coverage,
                                 double> &edge_variable_assignments,
                  std::string filename)
{
  solution_printer sp{filename, 1000, 1000};
  for (auto v_it = boost::vertices(g); v_it.first != v_it.second;
       ++v_it.first) {
    sp.print_field(g.GetCoordinate(*v_it.first));
  }
  for (auto p: edge_variable_assignments) {
    if (p.second == 0) { continue; }
    turncostcover::Coordinate u = g.GetCoordinate(p.first.n1);
    turncostcover::Coordinate v = g.GetCoordinate(p.first.f);
    turncostcover::Coordinate w = g.GetCoordinate(p.first.n2);
    sp.drawEdge(u, v, w, (int) std::lround(p.second));
    // std::cout<< p.first << " = " << p.second <<std::endl;
  }
  sp.finish();
}

void
VisualizeIntegral(turncostcover::GridGraph &g,
                  turncostcover::IntegralSolution &solution,
                  std::string filename)
{
  solution_printer sp{filename, 1000, 1000};
  for (auto v_it = boost::vertices(g); v_it.first != v_it.second;
       ++v_it.first) {
    sp.print_field(g.GetCoordinate(*v_it.first));
  }
  for (const auto &v: g.GetVertices()) {
    for (auto cov: solution.GetUsedCoverages(v)) {
      turncostcover::Coordinate u = g.GetCoordinate(cov.n1);
      turncostcover::Coordinate v = g.GetCoordinate(cov.f);
      turncostcover::Coordinate w = g.GetCoordinate(cov.n2);
      sp.drawEdge(u, v, w, solution.GetUsageOfCoverage(cov));
    }
  }
  sp.finish();
}

svg::Color density_color(double d)
{
  assert(d >= 0 && d <= 1);
  double r1 = 255;
  double r2 = 232;
  double g1 = 217;
  double g2 = 92;
  double b1 = 142;
  double b2 = 1;
  return svg::Color{(int) std::floor(r1 * (1 - d) + d * r2),
                    (int) std::floor((1 - d) * g1 + d * g2),
                    (int) std::floor((1 - d) * b1 + d * b2)};
}

void
VisualizeIntegralDensity(const turncostcover::GridGraph &graph,
                         const turncostcover::IntegralSolution &solution,
                         const std::map<turncostcover::Field,
                                        double> &penalties,
                         std::string filename)
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
  svg::Document
      file{filename, svg::Dimensions{2.0 + max_x - min_x, 2.0 + max_y - min_y}};

  auto field_stroke = svg::Stroke{0.05, svg::Color::Black};

  auto max_density = std::max_element(penalties.cbegin(), penalties.cend(),
                                      [](const auto &lhs,
                                         const auto &rhs) -> bool {
                                        return lhs.second < rhs.second;
                                      })->second;
  auto field_fill = svg::Fill{svg::Color{169, 169, 169}};
  for (const auto &v: graph.GetVertices()) {
    auto coord = graph.GetCoordinate(v);

    file << svg::Rectangle{
        svg::Point{1.0 + coord.x - min_x, 1.0 + coord.y - min_y}, //Position
        1, //height
        1, //width
        svg::Fill{density_color(penalties.at(v) / max_density)},
        field_stroke};

  }

  std::map<std::pair<turncostcover::Field, turncostcover::Field>, int> usage;
  for (const auto &v: graph.GetVertices()) {
    for (const auto &cov: turncostcover::ListAllPossibleCoverages(graph, v)) {
      auto u = solution.GetUsageOfCoverage(cov);
      std::pair<turncostcover::Field, turncostcover::Field>
          t1{std::min(cov.f, cov.n1), std::max(cov.f, cov.n1)};
      std::pair<turncostcover::Field, turncostcover::Field>
          t2{std::min(cov.f, cov.n2), std::max(cov.f, cov.n2)};
      usage[t1] += u;
      usage[t2] += u;
    }
  }
  for (auto e: usage) {
    auto c1 = graph.GetCoordinate(e.first.first);
    auto c2 = graph.GetCoordinate(e.first.second);
    file << svg::Line{svg::Point{1.5 + c1.x - min_x, 0.5 + c1.y - min_y},
                      svg::Point{1.5 + c2.x - min_x, 0.5 + c2.y - min_y},
                      svg::Stroke{0.1 * e.second, svg::Color::Blue}};
  }
  file.save();
}
}
