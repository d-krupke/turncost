//
// Created by Dominik Krupke, http://krupke.cc on 6/15/17.
//

#ifndef LAZYGRIDGRAPHSHORTESTPATHWITHTURNCOSTS_LAZY_GRID_DIJSKTRA_TC_H
#define LAZYGRIDGRAPHSHORTESTPATHWITHTURNCOSTS_LAZY_GRID_DIJSKTRA_TC_H

#include <vector>
#include <algorithm>
#include <array>
#include <map>
#include <unordered_map>
#include <iostream>
#include <assert.h>
#include <queue>
#include "grid_graph.h"
#include "DFieldId.h"

namespace turncostgrid
{

  using CostUnit = int;
  struct Costs
  {
    CostUnit simple_turn;
    CostUnit distance;
  };

  class TurnCostGridDijkstra
  {


    inline DFieldId
    other_field_nbr(DFieldId v) const
    {
      return {grid_graph.adjacency_array_map[v.fieldId().internal_id()][v.direction_bits()], v.direction_bits()};
    }


    size_t
    dfield_size()
    {
      return 4 * grid_graph.adjacency_array_map.size();
    }

    struct PriorityQueueElement
    {
      DFieldId node;
      CostUnit dist;

      bool
      operator<(const PriorityQueueElement &b) const
      {
        return dist > b.dist || (dist == b.dist && node < b.node);
      }
    };


  public:

    TurnCostGridDijkstra(GridGraph &grid_graph, Costs costs, unsigned int ext_id, Direction dir);


    TurnCostGridDijkstra(GridGraph &grid_graph, Costs costs);

    void
    add_starting_point(unsigned int ext_id, Direction dir)
    {
      DFieldId v{grid_graph.external_to_internal_id(ext_id), dir};
      distance_map[v.idx()] = 0;
      queue.push({v, 0});
    }

    CostUnit
    distance(unsigned int ext_id, Direction d)
    const
    {
      DFieldId v{grid_graph.external_to_internal_id(ext_id), d};
      return distance_map[v.idx()];
    }

    bool
    already_calulated(unsigned int ext_id, Direction d)
    const
    {
      DFieldId v{grid_graph.external_to_internal_id(ext_id), d};
      return distance_map[v.idx()] < std::numeric_limits<CostUnit>::max();
    }

    //Faster than shortest_path()
    std::vector<std::pair<unsigned int, Direction>>
    reversed_shortest_path(unsigned int ext_id, Direction d)
    const;

    std::vector<std::pair<unsigned int, Direction>>
    shortest_path(unsigned int ext_id, Direction d)
    const;

    /**
     * This function allows you to pause the calculation after some shortest path has been calculated.
     * Note that first the shortest shortest paths are calculated.
     * You can continue without any overhead at any time after.
     *
     * This is useful if you usually only need the cheap connections, e.g. in a MinimumWeightPerfectMatching.
     */
    template<typename Func>
    // (unsigned int [coordinate index], Direction)->bool [true: continue, false: pause calculation]
    bool
    lazy_dijkstra(Func call_back)
    {
      while (!queue.empty()) {
        auto t = queue.top();
        queue.pop();
        auto v = t.node;
        auto v_dist = t.dist;
        assert(max_edge_value <= v_dist);
        max_edge_value = v_dist;
        if (v_dist > distance_map[v.idx()]) { continue; }

        auto n1 = v.same_field_nbr_1();
        auto n2 = v.same_field_nbr_2();
        auto n3 = other_field_nbr(v);

        auto n1_dist = costs.simple_turn + v_dist;
        if (distance_map[n1.idx()] > n1_dist) {
          queue.push({n1, n1_dist});
          distance_map[n1.idx()] = n1_dist;
          predecessor_map[n1.idx()] = v;
        }

        auto n2_dist = costs.simple_turn + v_dist;
        if (distance_map[n2.idx()] > n2_dist) {
          queue.push({n2, n2_dist});
          distance_map[n2.idx()] = n2_dist;
          predecessor_map[n2.idx()] = v;
        }

        auto n3_dist = costs.distance + v_dist;
        if (n3.fieldId() != GridGraph::FieldId{} && distance_map[n3.idx()] > n3_dist) {
          queue.push({n3, n3_dist});
          distance_map[n3.idx()] = n3_dist;
          predecessor_map[n3.idx()] = v;
        }
        assert(already_calulated(t.node.fieldId().external_id(), t.node.dir()));
        if (!call_back(t.node.fieldId().external_id(), t.node.dir())) { break; }
      }
      return queue.empty();
    }

    void
    dijkstra();

    inline CostUnit
    get_max_edge_value()
    {
      return max_edge_value;
    }

  private:

    GridGraph &grid_graph;
    Costs costs;
    std::vector<DFieldId> predecessor_map; //Predecessors. Uses DField.idx()
    std::vector<CostUnit> distance_map; //Cost to root. Uses DField.idx()
    std::priority_queue<PriorityQueueElement> queue;
    CostUnit max_edge_value = 0;
  };

}

#endif //LAZYGRIDGRAPHSHORTESTPATHWITHTURNCOSTS_LAZY_GRID_DIJSKTRA_TC_H
