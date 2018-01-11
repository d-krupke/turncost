//
// Created by Dominik Krupke, http://krupke.cc on 9/6/16.
//

#include "save_and_load.h"

void
turncostcover::SaveInstance(GridGraph &instance, std::string filename)
{
  std::ofstream file;
  file.open(filename, std::ios::out | std::ios::trunc);
  if (file.is_open()) {
    for (auto v_it = boost::vertices(instance); v_it.first != v_it.second; ++v_it.first) {
      auto v = *v_it.first;
      auto v_coord = instance.GetCoordinate(v);
      file << v_coord.x << " " << v_coord.y << std::endl;
    }
    file.close();
  } else {
    std::cerr << "Could not write grid graph to file \"" << filename << "\"" << std::endl;
  }
}

turncostcover::GridGraph
turncostcover::LoadInstance(std::string filename)
{
  std::set<Coordinate> coords;
  std::ifstream file(filename);
  if (file.is_open()) {
    std::string line;
    while (std::getline(file, line)) {
      if (line.empty()) { continue; }
      if (line.at(0) == '#') { continue; }
      std::vector<std::string> tokens;
      boost::split(tokens, line, boost::is_any_of(" \t;"));
      if (tokens.size() >= 2) {
        coords.insert(turncostcover::Coordinate(std::stoi(tokens[0]), std::stoi(tokens[1])));
      }
    }
    file.close();
  } else {
    throw CouldNotLoadInstanceException{"Could not load grid graph from file \"" + filename + "\""};
  }
  return GridGraph(coords);
}

void
turncostcover::SaveInstance(turncostcover::GridGraph &instance,
                            std::set<Coordinate> &subset,
                            std::string filename)
{
  std::ofstream file;
  file.open(filename, std::ios::out | std::ios::trunc);
  if (file.is_open()) {
    for (auto v_it = boost::vertices(instance); v_it.first != v_it.second; ++v_it.first) {
      auto v = *v_it.first;
      auto v_coord = instance.GetCoordinate(v);
      file << v_coord.x << " " << v_coord.y << " " << subset.count(v_coord) << std::endl;
    }
    file.close();
  } else {
    std::cerr << "Could not write grid graph to file \"" << filename << "\"" << std::endl;
  }
}

turncostcover::GridGraph
turncostcover::LoadInstance(std::string filename,
                            std::map<Coordinate, double> &penalty)
{
  std::set<Coordinate> coords;
  std::ifstream file(filename);
  if (file.is_open()) {
    std::string line;
    while (std::getline(file, line)) {
      if (line.empty()) { continue; }
      if (line.at(0) == '#') { continue; }
      std::vector<std::string> tokens;
      boost::split(tokens, line, boost::is_any_of(" \t;"));
      if (tokens.size() >= 3) {
        Coordinate c(std::stoi(tokens[0]), std::stoi(tokens[1]));
        coords.insert(c);
        penalty[c] = std::stod(tokens[2]);
      } else {
        assert(false);
      }
    }
    file.close();
  } else {
    std::cerr << "Could not load grid graph from file \"" << filename << "\"" << std::endl;
  }
  return GridGraph(coords);
}

void
turncostcover::SaveInstance(turncostcover::GridGraph &instance,
                            std::map<Coordinate, double> &penalty,
                            std::string filename)
{
  std::ofstream file;
  file.open(filename, std::ios::out | std::ios::trunc);
  if (file.is_open()) {
    for (auto v_it = boost::vertices(instance); v_it.first != v_it.second; ++v_it.first) {
      auto v = *v_it.first;
      auto v_coord = instance.GetCoordinate(v);
      file << v_coord.x << " " << v_coord.y << " " << penalty[v_coord] << std::endl;
    }
    file.close();
  } else {
    std::cerr << "Could not write grid graph to file \"" << filename << "\"" << std::endl;
  }
}

turncostcover::GridGraph
turncostcover::LoadInstance(std::string filename, std::set<Coordinate> *subset)
{
  std::set<Coordinate> coords;
  std::ifstream file(filename);
  if (file.is_open()) {
    std::string line;
    while (std::getline(file, line)) {
      if (line.empty()) { continue; }
      if (line.at(0) == '#') { continue; }
      std::vector<std::string> tokens;
      boost::split(tokens, line, boost::is_any_of(" \t;"));
      if (tokens.size() >= 3) {
        Coordinate c(std::stoi(tokens[0]), std::stoi(tokens[1]));
        coords.insert(c);
        auto in_subset = std::stoi(tokens[2]);
        if (in_subset != 0 && in_subset != 1) {
          std::cerr << "Bad file encoding. Third column is only allowed to consist of '0' and '1'" << std::endl;
        }
        if (in_subset == 1) {
          subset->insert(c);
        }
      } else {
        assert(false);
      }
    }
    file.close();
  } else {
    std::cerr << "Could not load grid graph from file \"" << filename << "\"" << std::endl;
  }
  return GridGraph(coords);
}

turncostcover::GridGraph
turncostcover::LoadDensityMap(std::string filename,
                              std::map<turncostcover::Field, double> *penalty)
{
  std::set<Coordinate> coords;
  std::ifstream file(filename);
  if (file.is_open()) {
    std::map<turncostcover::Coordinate, double> coord_penalties;
    std::string line;
    int x = 0;
    while (std::getline(file, line)) {
      ++x;
      if (line.empty()) { continue; }
      if (line.at(0) == '#') { continue; }
      std::vector<std::string> tokens;
      boost::split(tokens, line, boost::is_any_of(", \t;"));
      int y = 0;
      for (const auto &entry: tokens) {
        ++y;
        Coordinate c{x, y};
        auto p = std::stod(entry);
        if(p>=0) {
          coord_penalties[c] = p;
          coords.insert(c);
        }
      }
    }
    file.close();
    GridGraph gg{coords};
    for(auto e: coord_penalties){
      (*penalty)[gg.GetVertex(e.first)]=e.second;
    }
    return gg;
  } else {
    std::cerr << "Could not load grid graph from file \"" << filename << "\"" << std::endl;
  }

  return GridGraph(coords);
}
