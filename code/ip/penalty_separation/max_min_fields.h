//
// Created by Dominik Krupke, http://krupke.cc on 9/14/17.
//

#ifndef TURNCOSTCOVER_IP1_P_MAX_MIN_FIELDS_H
#define TURNCOSTCOVER_IP1_P_MAX_MIN_FIELDS_H

namespace turncostcover
{
  namespace ip_formulation1
  {
    namespace details
    {
      Field
      FindMaxInternalField(const IpSolver &solver,
                           const IntegralSolution &solution,
                           const std::set<Field> &comp_fields);

      Field
      FindUsedMaxExternalField(const IpSolver &solver,
                               const IntegralSolution &solution,
                               const std::set<Field> &comp_fields);
    }

  }
}

#endif //TURNCOSTCOVER_IP1_P_MAX_MIN_FIELDS_H
