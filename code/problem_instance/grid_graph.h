//
// Created by Dominik Krupke, http://krupke.cc on 6/20/16.
//

#ifndef TURNCOSTCOVER_GRID_GRAPH_H
#define TURNCOSTCOVER_GRID_GRAPH_H

#include <iostream>     // std::cout
#include <algorithm>    // std::random_shuffle
#include <vector>       // std::vector
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <boost/graph/properties.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/directed_graph.hpp>
#include <boost/multi_array.hpp>
#include <boost/graph/johnson_all_pairs_shortest.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "./coordinate.h"

namespace turncostcover {

class GridGraph :
    public boost::adjacency_list<
        boost::vecS,
        boost::vecS,
        boost::undirectedS,
        boost::property<boost::vertex_name_t, Coordinate>
    > {
 public:
  using FieldId = vertex_descriptor;

  FieldId GetVertex(Coordinate c) const { return coord_to_vertex_.at(c); }

  bool HasVertex(Coordinate c) const { return coord_to_vertex_.count(c) > 0; }

  Coordinate GetCoordinate(FieldId v) const
  {
    return boost::get(boost::get(boost::vertex_name, *this), v);
  }

  auto //a container with begin() and end()
  GetVertices() const
  {
    return boost::make_iterator_range(boost::vertices(*this));
  }

  auto //a container with begin() and end()
  GetEdges() const
  {
    return boost::make_iterator_range(boost::edges(*this));
  }

  auto //a container with begin() and end()
  GetNeighbors(FieldId f) const
  {
    return boost::make_iterator_range(boost::adjacent_vertices(f, *this));
  }

  inline bool
  IsConnected() const
  {
    std::vector<int> component(boost::num_vertices(*this));
    int num = boost::connected_components(*this, &component[0]);
    return num == 1;
  }

  bool
  AreNeighbored(FieldId a, FieldId b) const
  {
    return boost::edge(a, b, *this).second;
  }

  size_t GetNumVertices() const { return boost::num_vertices(*this); }

  bool HasBoundary(FieldId v) const;
  bool HasNorthBoundary(FieldId v) const;
  bool HasEastBoundary(FieldId v) const;
  bool HasWestBoundary(FieldId v) const;

  GridGraph() {}
  GridGraph(std::set<Coordinate> &coords);

 private:
  void BuildGraphFromCoordinates(const std::set<Coordinate> &coords);

 private:
  std::map<Coordinate, vertex_descriptor> coord_to_vertex_;
};

using Field = GridGraph::FieldId;

}

#endif //TURNCOSTCOVER_GRID_GRAPH_H
