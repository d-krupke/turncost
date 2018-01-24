//
// Created by Dominik Krupke, http://krupke.cc on 10/9/17.
//

#ifndef TURNCOSTCOVER_ARKINAPX_STRIP_COVER_H
#define TURNCOSTCOVER_ARKINAPX_STRIP_COVER_H

#include "bipartite_vertex_cover/min_vertex_cover.h"
#include "../../problem_instance/grid_graph.h"
#include "strip.h"
#include "strip_extractor.h"
namespace turncostcover {
namespace arkinapx {

std::vector<Strip>
ComputeStripCover(const GridGraph &graph);

}
}
#endif //TURNCOSTCOVER_ARKINAPX_STRIP_COVER_H
