//
// Created by Dominik Krupke, http://krupke.cc on 8/18/17.
//

#include "path_to_cycle.h"
namespace turncostcover {
namespace apx {
std::vector<Coverage>
PathToCycle(const std::vector<Field> &path)
{
  assert(path.size() >= 2);
  std::vector<Coverage> cycle;
  cycle.emplace_back(path[1], path[0], path[1]);
  for (int i = 1; i < (int) path.size() - 1; ++i) {
    cycle.emplace_back(path[i - 1], path[i], path[i + 1]);
  }
  cycle.emplace_back(*(path.end() - 2), *(path.end() - 1), *(path.end() - 2));
  for (auto i = path.size() - 2; i > 0; --i) {
    cycle.emplace_back(path[i - 1], path[i], path[i + 1]);
  }
  return cycle;
}

}
}