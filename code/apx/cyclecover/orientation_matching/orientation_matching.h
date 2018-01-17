//
// Created by Dominik Krupke, http://krupke.cc on 9/5/17.
//

#ifndef TURNCOSTCOVER_APX_CYCLECOVER_ORIENTATION_MATCHING_H
#define TURNCOSTCOVER_APX_CYCLECOVER_ORIENTATION_MATCHING_H

#include <cyclecover/orientation_selection/solution.h>
#include <cyclecover/orientation_matching/local_orientation_matching/local_matching.h>
#include "../../../problem_instance/solution/solution.h"
namespace turncostcover {
namespace apx {
//ATTENTION for subset coverage!
IntegralSolution
Match(const GridGraph &graph, OrientationSolution *or_sol, Costs costs)
{
  LocalMatching lm{graph, or_sol, costs};
  return lm.Solve();
}

}
}

#endif //TURNCOSTCOVER_APX_CYCLECOVER_ORIENTATION_MATCHING_H
