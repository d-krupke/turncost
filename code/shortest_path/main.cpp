#include <iostream>
#include "turn_cost_grid_dijkstra.h"
#include "DFieldId.h"
#include <fstream>
#include <boost/algorithm/string.hpp>

int
main()
{
  using namespace turncostgrid;
  std::vector<GridCoordinate> coords;
  std::string filename{
          "/home/doms/Repositories/IBR-SVN/thesis-alg-2015-krupke-ma-robots/MasterThesis/ALENEX/Code/gridgraph/apx/p_1400_dense_2.gg"};
  std::ifstream file(filename);
  if (file.is_open()) {
    std::string line;
    while (std::getline(file, line)) {
      if (line.empty()) { continue; }
      if (line.at(0) == '#') { continue; }
      std::vector<std::string> tokens;
      boost::split(tokens, line, boost::is_any_of(" "));
      if (tokens.size() >= 2) {
        coords.push_back({std::stoi(tokens[0]), std::stoi(tokens[1])});
      }
    }
    file.close();
  } else {
    std::cerr << "Could not load grid graph from file \"" << filename << "\"" << std::endl;
  }
  std::sort(coords.begin(), coords.end());

  //std::vector<GridCoordinate> g{{0,0},{1,0}, {1,1}, {0,1}, {0,2}};
  // std::sort(g.begin(), g.end());
  GridGraph gd{coords, true};
  gd.print_graph(coords);

  for (unsigned int i = 0; i < coords.size(); ++i) {
    TurnCostGridDijkstra lgd{gd, {3, 1}, i, NORTH};
    lgd.lazy_dijkstra([&](int id, Direction d) {
        //std::cout << id << " " << to_string(d) << ": " << lgd.distance(state, id, d) << std::endl;
        return true;
    });
  }

  return 0;
}