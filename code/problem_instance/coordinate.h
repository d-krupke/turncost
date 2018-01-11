//
// Created by Dominik Krupke, http://krupke.cc on 8/8/17.
//

#ifndef TURNCOSTCOVER_COORDINATE_H
#define TURNCOSTCOVER_COORDINATE_H

#include <iostream>
#include <fstream>
#include <boost/graph/properties.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/algorithm/string/split.hpp>
#include "boost/multi_array.hpp"

namespace turncostcover {

struct Coordinate {
  int x;
  int y;

  Coordinate(int x, int y);

  Coordinate() : x{0}, y{0} { /*unused default*/}

  bool
  operator==(const Coordinate &rhs) const;

  bool
  operator!=(const Coordinate &rhs) const;

  bool
  operator<(const Coordinate &rhs) const;

  bool
  operator>(const Coordinate &rhs) const;

  bool
  operator<=(const Coordinate &rhs) const;

  bool
  operator>=(const Coordinate &rhs) const;

};

std::ostream &
operator<<(std::ostream &os, const turncostcover::Coordinate &coordinate);
}

#endif //TURNCOSTCOVER_COORDINATE_H
