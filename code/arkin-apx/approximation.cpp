//
// Created by Dominik Krupke, http://krupke.cc on 10/5/17.
//

#include "approximation.h"
namespace turncostcover {

namespace arkinapx {


IntegralSolution ApproximateFullCycleCoverViaStripCover(const GridGraph &graph,
                                                        Costs costs)
{
  details::StripMatchingSolver stripMatchingSolver{graph, costs};
  return stripMatchingSolver.Solve();
}
IntegralSolution ApproximateFullTourViaStripCover(const GridGraph &graph, Costs costs)
{
  auto cc = ApproximateFullCycleCoverViaStripCover(graph, costs);
  apx::ConnectAdjacentCycles(&cc, costs);
  return cc;
}
}
}
