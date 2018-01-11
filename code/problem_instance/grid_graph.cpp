//
// Created by Dominik Krupke, http://krupke.cc on 6/20/16.
//

#include <iostream>
#include "grid_graph.h"
#include "coordinate.h"

namespace turncostcover {

GridGraph::GridGraph(std::set<Coordinate> &coords)
{
  BuildGraphFromCoordinates(coords);
}

void
GridGraph::BuildGraphFromCoordinates(const std::set<Coordinate> &coords)
{
  if (coords.empty()) {
    std::cout
        << "WARNING: Building empty grid graph. Probably you have inserted a bad file or something similiar."
        << std::endl;
  }
  auto vertex_name_map = boost::get(boost::vertex_name, *this);
  for (const auto &c: coords) {

    auto v = boost::add_vertex(*this);
    boost::put(vertex_name_map, v, c);
    coord_to_vertex_[c] = v;

    Coordinate c_north{c.x, c.y + 1};
    Coordinate c_east{c.x + 1, c.y};
    Coordinate c_south{c.x, c.y - 1};
    Coordinate c_west{c.x - 1, c.y};
    if (coord_to_vertex_.count(c_north)) {
      boost::add_edge(v, GetVertex(c_north), *this);
    }
    if (coord_to_vertex_.count(c_east)) {
      boost::add_edge(v, GetVertex(c_east), *this);
    }
    if (coord_to_vertex_.count(c_south)) {
      boost::add_edge(v, GetVertex(c_south), *this);
    }
    if (coord_to_vertex_.count(c_west)) {
      boost::add_edge(v, GetVertex(c_west), *this);
    }
  }
}

bool
GridGraph::has_west_boundary(FieldId v)
const
{
  auto coord = GetCoordinate(v);
  for (auto n: GetNeighbors(v)) {
    if (GetCoordinate(n).x < coord.x) { return false; }
  }
  return true;
}

bool
GridGraph::has_east_boundary(FieldId v)
const
{
  auto coord = GetCoordinate(v);
  for (auto n: GetNeighbors(v)) {
    if (GetCoordinate(n).x > coord.x) { return false; }
  }
  return true;
}

bool
GridGraph::has_north_boundary(FieldId v)
const
{
  auto coord = GetCoordinate(v);
  for (auto n: GetNeighbors(v)) {
    if (GetCoordinate(n).y > coord.y) { return false; }
  }
  return true;
}

bool
GridGraph::has_boundary(FieldId v)
const
{
  auto neighbor_container = GetNeighbors(v);
  return std::distance(neighbor_container.begin(), neighbor_container.end())
      != 4;
}

}
