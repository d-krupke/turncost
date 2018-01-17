//
// Created by Dominik Krupke, http://krupke.cc on 8/18/17.
//

#ifndef TURNCOSTCOVER_APX_CONNECT_PATH_TO_CYCLE_H
#define TURNCOSTCOVER_APX_CONNECT_PATH_TO_CYCLE_H

#include "../../../problem_instance/coverage.h"
namespace turncostcover {
namespace apx {
std::vector<Coverage> PathToCycle(const std::vector<Field> &path);
}
}

#endif //TURNCOSTCOVER_APX_CONNECT_PATH_TO_CYCLE_H
