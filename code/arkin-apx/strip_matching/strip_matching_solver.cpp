//
// Created by Dominik Krupke, http://krupke.cc on 10/2/17.
//

#include <strip_cover/strip_cover.h>
#include "strip_matching_solver.h"
namespace turncostcover {
namespace arkinapx {
namespace details {

void
StripMatchingSolver::PushbackIfNotAlreadyAtEnd(std::vector<Coordinate> *cycle,
                                               Coordinate c) const {
  if (cycle->empty() || cycle->back() != c) { cycle->push_back(c); }
}

std::vector<turncostgrid::TurnCostGridDijkstra>
StripMatchingSolver::CalculateShortestPathTrees() {
  std::vector<turncostgrid::TurnCostGridDijkstra> shortest_path_trees;
  for (auto e: endpoints_) {
    shortest_path_trees.emplace_back(wrapped_grid_graph_, turncostgrid::Costs{costs_.turn_costs, costs_.dist_costs});
    shortest_path_trees.back().add_starting_point(wrapped_grid_graph_.GetId(graph_.GetVertex(
        e.first)),
                                                  Convert(e.second));
  }
  for (auto &spt: shortest_path_trees) {
    spt.dijkstra();
  }
  return shortest_path_trees;
}

unsigned long
StripMatchingSolver::FindUnprocessedId(const std::set<int> &processed_endpoints) const {
  auto unprocessed_id = endpoints_.size();
  for (size_t i = 0; i < endpoints_.size(); ++i) {
    if (processed_endpoints.count(i) == 0) {
      unprocessed_id = i;
      break;
    }
  }
  return unprocessed_id;
}

void
StripMatchingSolver::AddStripFields(std::vector<Coordinate> *cycle,
                                    const Coordinate &e1_coord,
                                    const Coordinate &e2_coord) const {
  auto intermediate = e1_coord;
  while (intermediate != e2_coord) {
    PushbackIfNotAlreadyAtEnd(cycle, intermediate);
    intermediate.x = AdvanceTowards(intermediate.x, e2_coord.x);
    intermediate.y = AdvanceTowards(intermediate.y, e2_coord.y);
  }
  PushbackIfNotAlreadyAtEnd(cycle, e2_coord);
}

void
StripMatchingSolver::FillSingleFieldCyclesWithRandomNbr(std::vector<Coordinate> *cycle) const {
  if (cycle->size() == 1) { //Add arbitrary neighbor
    cycle->push_back(graph_.GetCoordinate(*graph_.GetNeighbors(graph_.GetVertex(
        cycle->front())).begin()));
  }
}

int
StripMatchingSolver::AdvanceTowards(int x, int t) const {
  if (t == x) { return x; }
  if (x < t) { return ++x; }
  return --x;
}

turncostgrid::Direction
StripMatchingSolver::Convert(Direction d) const {
  switch (d) {
    case Direction::NORTH: return turncostgrid::Direction::NORTH;
    case Direction::EAST: return turncostgrid::Direction::EAST;
    case Direction::SOUTH:return turncostgrid::Direction::SOUTH;
    case Direction::WEST:return turncostgrid::Direction::WEST;
  }
  assert(false);//unreachable
}

std::unique_ptr<PerfectMatching>
StripMatchingSolver::CalculateMatching() const {
  auto pm = std::make_unique<PerfectMatching>(endpoints_.size(), endpoints_.size() * endpoints_.size());
  for (size_t i = 0; i < endpoints_.size(); ++i) {
    for (size_t j = i + 1; j < endpoints_.size(); ++j) {
      auto cost = GetCostBetweenEndpoints(i, j);
      assert(cost < std::numeric_limits<CostUnit>::max());
      pm->AddEdge(i, j, cost);
    }
  }
  pm->Solve();
  return pm;
}

IntegralSolution
StripMatchingSolver::Solve() {
  std::cout << "Calculate strip cover..." << std::endl;
  auto strips = ComputeStripCover(graph_);


  for (auto s: strips) {
    endpoints_.push_back(s.GetFirstEndPoint());
    endpoints_.push_back(s.GetSecondEndPoint());
  }

  std::cout << "Calculate distances between strips..." << std::endl;
  shortest_path_trees_ = CalculateShortestPathTrees();

  std::cout << "Match strips..." << std::endl;
  pm_ = CalculateMatching();

  std::cout << "Create solution..." << std::endl;
  //collect endpoint cycles;
  auto endpoint_cycles = ExtractCycles();

  IntegralSolution solution{graph_, endpoint_cycles};
  solution.SetLowerBound(std::max<double>(strips.size() * costs_.turn_costs
                                              + graph_.GetNumVertices()
                                                  * costs_.dist_costs,
                                          solution.GetCoverageObjectiveValue(
                                              costs_) / 2.5));
  CleanUp();
  return solution;
}

CostUnit
StripMatchingSolver::GetCostBetweenEndpoints(int i, int j) const {
  auto id_j = wrapped_grid_graph_.GetId(graph_.GetVertex(endpoints_[j].first));
  auto dir_j = Convert(GetOpposite(endpoints_[j].second));
  auto cost = shortest_path_trees_[i].distance(id_j, dir_j);
  //If they belong to the same strip
  if (endpoints_[i].first == endpoints_[j].first && endpoints_[i].second == GetOpposite(
      endpoints_[j].second)) {
    assert((i^0x1) == j);//partner?
    cost = 4 * costs_.turn_costs + 2 * costs_.dist_costs;
  }
  return cost;
}

std::vector<Coordinate>
StripMatchingSolver::GetShortestPathBetweenEndpoints(int i, int j) const {
  std::vector<Coordinate> spc;
  auto id_of_field_in_spt_graph =
      wrapped_grid_graph_.GetId(graph_.GetVertex(endpoints_[i].first));
  auto dir = Convert(GetOpposite(endpoints_.at(i).second));
  auto sp = shortest_path_trees_.at(j).reversed_shortest_path(id_of_field_in_spt_graph, dir);
  for (auto c_id: sp) {
    auto sp_c = graph_.GetCoordinate(wrapped_grid_graph_.GetField(c_id.first));
    spc.push_back(sp_c);
  }
  return spc;
}

std::vector<std::vector<Coordinate>>
StripMatchingSolver::ExtractCycles() const {
  std::vector<std::vector<Coordinate>> cycles;
  std::set<int> processed_endpoints;
  while (processed_endpoints.size() != endpoints_.size()) {
    auto unprocessed_id = FindUnprocessedId(processed_endpoints);
    std::vector<Coordinate> cycle;
    while (processed_endpoints.count(unprocessed_id) == 0) {

      auto e1_id = unprocessed_id;
      auto e1_coord = endpoints_.at(e1_id).first;
      auto e2_id = e1_id ^0x1;//flip last bit to obtain partner id, e.g. 0<->1, 4<->5
      auto e2_coord = endpoints_.at(e2_id).first;
      AddStripFields(&cycle, e1_coord, e2_coord);
      processed_endpoints.insert(e1_id);
      processed_endpoints.insert(e2_id);

      auto matched_endpoint = pm_->GetMatch(e2_id);
      unprocessed_id = matched_endpoint;

      //insert points on sp between partner and new unprocessed id
      auto sp = GetShortestPathBetweenEndpoints(e2_id, matched_endpoint);
      for (auto c: sp) {
        PushbackIfNotAlreadyAtEnd(&cycle, c);
      }
    }
    assert(!cycle.empty());
    FillSingleFieldCyclesWithRandomNbr(&cycle);
    cycles.push_back(cycle);
  }
  return cycles;
}
}
}
}
