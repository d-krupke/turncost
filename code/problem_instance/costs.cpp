//
// Created by Dominik Krupke, http://krupke.cc on 6/7/17.
//
#include "costs.h"
namespace turncostcover {

std::basic_ostream<char, std::char_traits<char>> &
operator<<(std::ostream &os, const turncostcover::Costs &costs)
{
  os << "turn costs: " << costs.turn_costs << "\t distance costs:"
     << costs.dist_costs;
  return os;
}

}
