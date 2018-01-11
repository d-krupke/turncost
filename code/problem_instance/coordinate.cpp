//
// Created by Dominik Krupke, http://dokrupke.cc on 8/8/17.
//

#include "coordinate.h"

namespace turncostcover{

bool
Coordinate::operator==(const Coordinate &rhs) const
{
  return x == rhs.x &&
         y == rhs.y;
}

bool
Coordinate::operator!=(const Coordinate &rhs) const
{
  return !(rhs == *this);
}

bool
Coordinate::operator<(const Coordinate &rhs) const
{
  if (x < rhs.x) {
    return true;
  }
  if (rhs.x < x) {
    return false;
  }
  return y < rhs.y;
}

bool
Coordinate::operator>(const Coordinate &rhs) const
{
  return rhs < *this;
}

bool
Coordinate::operator<=(const Coordinate &rhs) const
{
  return !(rhs < *this);
}

bool
Coordinate::operator>=(const Coordinate &rhs) const
{
  return !(*this < rhs);
}

std::ostream &
operator<<(std::ostream &os, const Coordinate &coordinate)
{
  os << "x: " << coordinate.x << " y: " << coordinate.y;
  return os;
}

Coordinate::Coordinate(int x, int y) : x{x}, y{y}
{

}
}