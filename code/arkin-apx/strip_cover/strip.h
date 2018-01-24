//
// Created by Dominik Krupke, http://krupke.cc on 10/2/17.
//

#ifndef TURNCOSTCOVER_ARKINAPX_STRIP_H
#define TURNCOSTCOVER_ARKINAPX_STRIP_H

#include "../../problem_instance/direction.h"
namespace turncostcover {
namespace arkinapx {
class Strip {
 public:

  Strip(std::pair<Coordinate, Direction> endpoint_a,
        std::pair<Coordinate, Direction> endpoint_b) :
      endpoint_a{endpoint_a}, endpoint_b{endpoint_b}
  {
    assert(
        GetDifference(endpoint_a.second, endpoint_b.second)
            == 2);
  }

  bool
  DoIntersect(const Strip &other_strip)
  const;

  bool
  operator==(const Strip &rhs) const
  {
    return (endpoint_a == rhs.endpoint_a && endpoint_b == rhs.endpoint_b) ||
        (endpoint_a == rhs.endpoint_b && endpoint_b == rhs.endpoint_a);
  }

  bool
  operator!=(const Strip &rhs) const
  {
    return !(rhs == *this);
  }

  bool
  IsHorizontal()
  const
  {
    return endpoint_a.second == Direction::EAST
        || endpoint_a.second == Direction::WEST;
  }

  bool
  IsVertical()
  const
  {
    return !IsHorizontal();
  }

  std::pair<Coordinate, Direction> GetFirstEndPoint() const { return endpoint_a; };
  std::pair<Coordinate, Direction> GetSecondEndPoint() const { return endpoint_b; };

 private:
  std::pair<Coordinate, Direction> endpoint_a;
  std::pair<Coordinate, Direction> endpoint_b;
};

}
}
#endif //TURNCOSTCOVER_ARKINAPX_STRIP_H
