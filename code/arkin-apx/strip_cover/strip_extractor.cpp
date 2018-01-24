//
// Created by Dominik Krupke, http://krupke.cc on 10/2/17.
//

#include "strip_extractor.h"
namespace turncostcover {
namespace arkinapx {
Strip
StripExtractor::ExtractStripToEast(const GridGraph &graph, Field v)
const
{
  auto coord = graph.GetCoordinate(v);
  auto coord_end = coord;
  while (graph.HasVertex(coord_end)) {
    coord_end.x += 1;
  }
  coord_end.x -= 1; //last coord_end wasn't within graph;
  return {{coord, Direction::WEST},
          {coord_end, Direction::EAST}};
}

Strip
StripExtractor::ExtractStripToSouth(const GridGraph &graph, Field v)
const
{
  auto coord = graph.GetCoordinate(v);
  auto coord_end = coord;
  while (graph.HasVertex(coord_end)) {
    coord_end.y -= 1;
  }
  coord_end.y += 1; //last coord_end wasn't within graph;
  return {{coord, Direction::NORTH},
          {coord_end, Direction::SOUTH}};
}

std::vector<Strip>
StripExtractor::ExtractStrips(const GridGraph &graph)
{
  //The overall cost for this should be negligible
  std::vector<Strip> strips;
  for (auto v: graph.GetVertices()) {
    if (!graph.HasBoundary(v)) { continue; }
    if (graph.HasNorthBoundary(v)) {
      strips.push_back(ExtractStripToSouth(graph, v));
    }
    if (graph.HasWestBoundary(v)) {
      strips.push_back(ExtractStripToEast(graph, v));
    }
  }
  return strips;
}

}
}
