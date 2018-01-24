//
// Created by Dominik Krupke, http://krupke.cc on 10/6/17.
//

#ifndef GRIDGRAPH_DFIELDID_H
#define GRIDGRAPH_DFIELDID_H

#include "grid_graph.h"

namespace turncostgrid
{
  struct DFieldId
  {
  private:
    unsigned int bits = 0;
  public:


    DFieldId(GridGraph::FieldId f, unsigned int dir)
    {
      bits = (f.internal_id() << 2) | dir;
    }

    DFieldId(unsigned int bits) : bits{bits}
    {}

    DFieldId() : bits{0}
    {}

    inline DFieldId
    same_field_nbr_1() const
    {
      return {bits ^ 0x1};
    }

    inline DFieldId
    same_field_nbr_2() const
    {
      return {bits ^ 0x2};
    }

    inline GridGraph::FieldId
    fieldId() const
    {
      return {bits >> 2};
    }

    inline bool
    operator<(const DFieldId &b) const
    {
      return bits < b.bits;
    }

    inline int
    idx() const
    {
      return bits;
    }

    inline unsigned int
    direction_bits() const
    {
      return bits & 0x3;
    }

    inline Direction
    dir() const
    {
      switch (direction_bits()) {
        case NORTH:return NORTH;
        case EAST:return EAST;
        case SOUTH:return SOUTH;
        case WEST:return WEST;
        default:assert(false);
      }
      assert(false);
    }

  };
}
#endif //GRIDGRAPH_DFIELDID_H
