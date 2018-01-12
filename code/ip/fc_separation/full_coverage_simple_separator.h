//
// Created by Dominik Krupke, http://krupke.cc on 4/26/17.
//

#ifndef TURNCOSTCOVER_IP1_FC_SEPARATOR_1_H
#define TURNCOSTCOVER_IP1_FC_SEPARATOR_1_H

#include <cstddef>
#include "../general_cc_solver/ip_solver.h"
#include <boost/graph/filtered_graph.hpp>

namespace turncostcover
{
  namespace ip_formulation1
  {

    class FullCoverageSimpleSeparator
    {
    public:

      explicit FullCoverageSimpleSeparator(IpSolver *solver);

      virtual size_t Separate(const IntegralSolution &solution);

     protected:
      virtual
      std::map<int, std::set<turncostcover::Field>>
      GetComponents(const IntegralSolution &solution)
      const;

      virtual
      size_t
      CreateConstraint(const IntegralSolution &solution,
                       const std::set<Field> &comp_fields)
      const;

      IloExpr
      CreateLeaveComponentExpression(const IntegralSolution &solution,
                                     const std::set<Field> &comp_fields)
      const;

     protected:
      IpSolver *solver_;

    };
  }
}
#endif //TURNCOSTCOVER_IP1_FC_SEPARATOR_1_H
