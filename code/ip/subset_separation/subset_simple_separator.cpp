//
// Created by Dominik Krupke, http://krupke.cc on 4/28/17.
//

#include "./subset_simple_separator.h"

namespace turncostcover {
namespace ip_formulation1 {

bool
SubsetCoverageSimpleSeparation::IsSkipComponent(const std::set<Field> &component)
const
{
  //check if the cycle covers at least one but not all subset fields
  auto l = [&](Field v) -> bool { return subset.count(v) > 0; };
  auto n = std::count_if(component.begin(), component.end(), l);
  return n == 0 || n == subset.size();
}

SubsetCoverageSimpleSeparation::SubsetCoverageSimpleSeparation(
    IpSolver *solver,
    const std::set<Field> &subset)
    : FullCoverageSimpleSeparator(solver), subset(subset)
{

}

}
}
