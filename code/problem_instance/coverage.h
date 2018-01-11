//
// Created by Dominik Krupke, http://krupke.cc on 7/14/17.
//

#ifndef TURNCOSTCOVER_COVERAGE_H
#define TURNCOSTCOVER_COVERAGE_H

#include <vector>
#include "grid_graph.h"
#include "costs.h"

namespace turncostcover {
/// a variable is identified by the two fields it connects and the intermediate field
struct Coverage {
  Field f; /// the primary field that is covered
  Field n1; /// the first neighbor of this coverage
  Field n2; /// the second neighbors of this coverage

  Coverage() : f{}, n1{}, n2{} {}
  Coverage(GridGraph::FieldId u, GridGraph::FieldId v, GridGraph::FieldId w);

  bool IsUTurn() const;
  bool IsStraight(const GridGraph &gg) const;
  bool IsHorizontal(const GridGraph &gg) const;
  bool IsVertical(const GridGraph &gg) const;
  bool IsSimpleTurn(const GridGraph &gg) const;

  bool operator<(const Coverage &rhs) const;
  bool operator==(const Coverage &b) const;
  bool operator!=(const Coverage &rhs) const;

  int NumberOfUTurns(const GridGraph &g) const;

  Field DeduceOtherNeighbor(const Field n) const;

  CostUnit Cost(Costs costs, const GridGraph &graph) const;
};

/**
 * Returns all possible coverages for the field. No Repetitions. Note that (n1,f,n2)==(n2,f,n1).
 *
 * @param graph The instance graph
 * @param f The field
 * @return All possible coverages of f (between 0 and 10).
 */
std::vector<Coverage>
ListAllPossibleCoverages(const GridGraph &graph, const Field f);

/**
 * All u-turns covering the field f.
 *
 * @param graph The instance graph
 * @param f The field
 * @return All u-turns on f (between 0 and 4)
 */
std::vector<Coverage>
ListUTurns(const GridGraph &graph, GridGraph::FieldId &f);

/**
 * All simple turns on field f. No repetitions. Note that (n1,f,n2)==(n2,f,n1).
 * @param graph The instance graph
 * @param f The field
 * @return All simple turns on f (between 0 and 4)
 */
std::vector<Coverage>
ListSimpleTurns(const GridGraph &graph, GridGraph::FieldId &f);

/**
 * All straight coverages of field f.
 * @param graph The instance graph
 * @param f The field
 * @return All straight coverage of f (between 0 and 2)
 */
std::vector<Coverage>
ListStraightCoverages(const GridGraph &graph, GridGraph::FieldId &f);
}

#endif //TURNCOSTCOVER_COVERAGE_H
