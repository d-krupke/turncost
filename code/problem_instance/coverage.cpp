//
// Created by Dominik Krupke, http://krupke.cc on 7/14/17.
//

#include "coverage.h"

namespace turncostcover {
std::vector<Coverage>
ListUTurns(const GridGraph &graph, GridGraph::FieldId &f)
{
  std::vector<Coverage> ret;
  for (auto wp = boost::adjacent_vertices(f, graph); wp.first != wp.second;
       ++wp.first) {
    auto w = *wp.first;
    ret.push_back({w, f, w});
  }
  return ret;
}

std::vector<Coverage>
ListSimpleTurns(const GridGraph &graph, GridGraph::FieldId &f)
{
  std::vector<Coverage> ret;

  for (auto up = boost::adjacent_vertices(f, graph); up.first != up.second;
       ++up.first) {
    auto u = *up.first;
    for (auto wp = boost::adjacent_vertices(f, graph); wp.first != wp.second;
         ++wp.first) {
      auto w = *wp.first;
      if (u < w) {
        Coverage c{u, f, w};
        if (c.IsSimpleTurn(graph)) { ret.push_back(c); }
      }
    }
  }
  return ret;
}

std::vector<Coverage>
ListStraightCoverages(const GridGraph &graph, GridGraph::FieldId &f)
{
  std::vector<Coverage> ret;

  for (auto up = boost::adjacent_vertices(f, graph); up.first != up.second;
       ++up.first) {
    auto u = *up.first;
    for (auto wp = boost::adjacent_vertices(f, graph); wp.first != wp.second;
         ++wp.first) {
      auto w = *wp.first;
      if (u < w) {
        Coverage c{u, f, w};
        if (c.IsStraight(graph)) { ret.push_back(c); }
      }
    }
  }
  return ret;
}

std::vector<Coverage>
ListAllPossibleCoverages(const GridGraph &graph, const Field f)
{
  {
    std::vector<Coverage> ret;

    for (auto up = boost::adjacent_vertices(f, graph); up.first != up.second;
         ++up.first) {
      auto u = *up.first;
      for (auto wp = boost::adjacent_vertices(f, graph); wp.first != wp.second;
           ++wp.first) {
        auto w = *wp.first;
        Coverage c{u, f, w};
        if (std::find(ret.begin(), ret.end(), c) == ret.end()) {
          ret.push_back(c);
        }
      }
    }
    return ret;
  }
}

bool
Coverage::IsStraight(const GridGraph &gg)
const
{
  if (IsUTurn()) { return false; }

  auto p1 = gg.GetCoordinate(n1);
  auto p2 = gg.GetCoordinate(n2);
  return p1.x == p2.x || p1.y == p2.y;
}

bool
Coverage::IsHorizontal(const GridGraph &gg)
const
{
  if (IsUTurn()) { return false; }

  auto p1 = gg.GetCoordinate(n1);
  auto p2 = gg.GetCoordinate(n2);
  return p1.y == p2.y;
}

bool
Coverage::IsVertical(const GridGraph &gg)
const
{
  if (IsUTurn()) { return false; }

  auto p1 = gg.GetCoordinate(n1);
  auto p2 = gg.GetCoordinate(n2);
  return p1.x == p2.x;
}

bool
Coverage::IsSimpleTurn(const GridGraph &gg)
const
{
  return !(IsUTurn() || IsStraight(gg));
}

int
Coverage::NumberOfUTurns(const GridGraph &g)
const
{
  if (IsSimpleTurn(g)) { return 1; }
  if (IsUTurn()) { return 2; }
  return 0;
}

bool
Coverage::operator==(const Coverage &b)
const
{
  return n1 == b.n1 && n2 == b.n2 && f == b.f;
}

bool
Coverage::operator<(const Coverage &rhs)
const
{
  return (n1 < rhs.n1) || (n1 == rhs.n1 && n2 < rhs.n2)
      || (n1 == rhs.n1 && n2 == rhs.n2 && f < rhs.f);
}

Coverage::Coverage(GridGraph::FieldId u,
                   GridGraph::FieldId v,
                   GridGraph::FieldId w) : f{v},
                                           n1{std::min(u, w)},
                                           n2{std::max(u, w)}
{
  assert(u != v);
  assert(v != w);
}

bool
Coverage::IsUTurn()
const
{
  return n1 == n2;
}

CostUnit
Coverage::Cost(Costs costs, const GridGraph &graph)
const
{
  return costs.turn_costs * this->NumberOfUTurns(graph) + costs.dist_costs;
}

bool
Coverage::operator!=(const Coverage &rhs)
const
{
  return !((*this) == rhs);
}

Field
Coverage::DeduceOtherNeighbor(const Field n)
const
{
  if (n == this->n1) { return n2; }
  return n1;
}

}