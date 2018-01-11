//
// Created by Dominik Krupke, http://krupke.cc on 9/6/16.
//

#ifndef TURNCOSTCOVER_SAVE_AND_LOAD_H
#define TURNCOSTCOVER_SAVE_AND_LOAD_H

#include "grid_graph.h"
#include "coordinate.h"

namespace turncostcover {
struct CouldNotLoadInstanceException {
  std::string message;
};

void
SaveInstance(GridGraph &instance, std::string filename);

GridGraph
LoadInstance(std::string filename);

void
SaveInstance(GridGraph &instance,
             std::set<Coordinate> &subset,
             std::string filename);

GridGraph
LoadInstance(std::string filename, std::set<Coordinate> *subset);

void
SaveInstance(GridGraph &instance,
             std::map<Coordinate, double> &penalty,
             std::string filename);

GridGraph
LoadInstance(std::string filename, std::map<Coordinate, double> &penalty);

GridGraph
LoadDensityMap(std::string filename,
               std::map<turncostcover::Field, double> *penalty);
}

#endif //TURNCOSTCOVER_SAVE_AND_LOAD_H
