//
// Created by Dominik Krupke, http://krupke.cc on 10/2/17.
//

#ifndef TURNCOSTCOVER_ARKINAPX_STRIP_EXTRACTOR_H
#define TURNCOSTCOVER_ARKINAPX_STRIP_EXTRACTOR_H

#include "strip.h"
namespace turncostcover {
namespace arkinapx {
class StripExtractor {
 public:
  std::vector<Strip> ExtractStrips(const GridGraph &graph);

 private:
  Strip ExtractStripToEast(const GridGraph &graph, Field v) const;

  Strip ExtractStripToSouth(const GridGraph &graph, Field v) const;
};
}

}
#endif //TURNCOSTCOVER_ARKINAPX_STRIP_EXTRACTOR_H
