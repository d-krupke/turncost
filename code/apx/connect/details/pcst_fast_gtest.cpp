//
// Created by Dominik Krupke, http://krupke.cc on 8/25/17.
//

#include <gtest/gtest.h>
#include "./pcst_fast/pcst_fast.h"

TEST(PcstFast, BaseTest)
{
  using namespace cluster_approx;
  const int kVerbosityLevel = 0;
  std::vector<std::pair<int, int> > edges;
  edges.emplace_back(0, 1);
  edges.emplace_back(1, 2);
  std::vector<double> prizes = {0, 5, 6};
  std::vector<double> costs = {3, 4};
  int root = -1;
  int target_num_active_clusters = 1;
  PCSTFast::PruningMethod pruning = PCSTFast::kNoPruning;

  std::vector<int> node_result;
  std::vector<int> edge_result;
  PCSTFast algo(edges, prizes, costs, root, target_num_active_clusters,
                pruning, kVerbosityLevel, [](const char *s) {
              fprintf(stderr, "%s", s);
              fflush(stderr);
          });
  algo.run(&node_result, &edge_result);
}