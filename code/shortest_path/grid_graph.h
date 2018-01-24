//
// Created by Dominik Krupke, http://krupke.cc on 6/16/17.
//

#ifndef LAZYGRIDGRAPHSHORTESTPATHWITHTURNCOSTS_GRID_GRAPH_H
#define LAZYGRIDGRAPHSHORTESTPATHWITHTURNCOSTS_GRID_GRAPH_H

#include <assert.h>
#include <array>
#include <vector>
#include <algorithm>
#include <iostream>

namespace turncostgrid
{

  enum Direction
  {
    NORTH = 0x0,
    EAST = 0x1,
    SOUTH = 0x3,
    WEST = 0x2 //This odd 2-bit encoding allows to obtain the bit pattern for the left and right direction via d^0x1 and d^0x2.
  };

  Direction
  opposite(Direction &d);

  std::string
  to_string(Direction d);

  struct GridCoordinate
  {
    int x;
    int y;

    inline bool
    operator<(const GridCoordinate &b) const
    {
      return x < b.x || (x == b.x && y < b.y);
    }

    inline bool
    operator==(const GridCoordinate &b) const
    {
      return x == b.x && y == b.y;
    }

    inline bool
    operator!=(const GridCoordinate &b) const
    {
      return !(*this == b);
    }
  };

  struct DGridCoordinate
  {
    GridCoordinate coordinate;
    Direction direction;
  };


  class GridGraph
  {
  public:

    /**
    * This FieldId reserves the 0 value for representing null/no-field.
    */
    struct FieldId
    {
    private:
      unsigned int bits = 0;

    public:
      FieldId(unsigned int internal_bits) : bits{internal_bits}
      {}

      FieldId() : bits{0}
      {}


      inline bool
      operator==(const FieldId &b) const
      {
        return bits == b.bits;
      }

      inline bool
      operator!=(const FieldId &b) const
      {
        return !(*this == b);
      }

      inline int
      external_id() const
      {
        assert(bits != 0);//if this fails, you asked for the external id of a null-field.
        return bits - 1;
      }

      inline unsigned int
      internal_id() const
      {
        return bits;
      }

    };


    static inline FieldId
    external_to_internal_id(unsigned int id)
    {
      return FieldId{id + 1};
    }


    std::vector<std::array<FieldId, 4>> adjacency_array_map;//indexed by internal id.

    std::vector<FieldId>
    get_neighbors(FieldId v)
    {
      std::vector<FieldId> nbrs;
      for (auto n: adjacency_array_map[v.internal_id()]) {
        if (n != FieldId{}) {
          nbrs.push_back(n);
        }
      }
      return nbrs;
    }

    GridGraph(std::vector<GridCoordinate> &fields, bool sorted);

    GridGraph()
    {}

    void
    print_graph(std::vector<GridCoordinate> &fields);

    size_t
    size()
    {
      return adjacency_array_map.size() - 1;
    }

    void
    construct_from_vector(std::vector<GridCoordinate> &fields, bool sorted);
  };

}
#endif //LAZYGRIDGRAPHSHORTESTPATHWITHTURNCOSTS_GRID_GRAPH_H
