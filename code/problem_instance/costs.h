//
// Created by Dominik Krupke, http://krupke.cc on 6/7/17.
//

#ifndef TURNCOSTCOVER_COSTS_H
#define TURNCOSTCOVER_COSTS_H

#include <ostream>

namespace turncostcover {
using CostUnit = int;

struct Costs {
  CostUnit turn_costs;
  CostUnit dist_costs;

  Costs() : turn_costs{0}, dist_costs{0} {}

  Costs(CostUnit turn_costs, CostUnit dist_costs) : turn_costs{turn_costs},
                                                    dist_costs{dist_costs} {}

  CostUnit MinCycleCost() const
  {
    return 2 * dist_costs + 4 * turn_costs;
  }
};

struct DistanceAndTurns {
  int turns = 0;
  int distance = 0;

  DistanceAndTurns(int turns, int distance) : turns{turns},
                                              distance{distance} {/*DEFAULT*/}

  CostUnit
  operator*(Costs rhs)
  {
    return rhs.dist_costs * distance + rhs.turn_costs * turns;
  }
};

std::ostream &
operator<<(std::ostream &os, const turncostcover::Costs &costs);
}

#endif //TURNCOSTCOVER_COSTS_H
