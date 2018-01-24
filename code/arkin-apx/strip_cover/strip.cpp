//
// Created by Dominik Krupke, http://krupke.cc on 10/2/17.
//

#include "strip.h"
namespace turncostcover {
namespace arkinapx {

bool
Strip::DoIntersect(const Strip &other_strip) const
{
  assert(*this != other_strip);

  //They have to be orthogonal to intersect
  if (turncostcover::GetDifference(endpoint_a.second,
                                   other_strip.endpoint_a.second)
      != 1) { return false; }

  //checking using the bounding box. One strip has to either define the height, the other the width.
  auto min_x = std::min(
      {endpoint_a.first.x, endpoint_b.first.x, other_strip.endpoint_a.first.x,
       other_strip.endpoint_b.first.x});
  auto max_x = std::max(
      {endpoint_a.first.x, endpoint_b.first.x, other_strip.endpoint_a.first.x,
       other_strip.endpoint_b.first.x});
  auto min_y = std::min(
      {endpoint_a.first.y, endpoint_b.first.y, other_strip.endpoint_a.first.y,
       other_strip.endpoint_b.first.y});
  auto max_y = std::max(
      {endpoint_a.first.y, endpoint_b.first.y, other_strip.endpoint_a.first.y,
       other_strip.endpoint_b.first.y});
  bool this_strip_covers_x =
      std::min(endpoint_a.first.x, endpoint_b.first.x) == min_x &&
          std::max(endpoint_a.first.x, endpoint_b.first.x) == max_x;
  bool this_strip_covers_y =
      std::min(endpoint_a.first.y, endpoint_b.first.y) == min_y &&
          std::max(endpoint_a.first.y, endpoint_b.first.y) == max_y;
  bool other_strip_covers_x =
      std::min(other_strip.endpoint_a.first.x, other_strip.endpoint_b.first.x)
          == min_x &&
          std::max(other_strip.endpoint_a.first.x,
                   other_strip.endpoint_b.first.x) == max_x;
  bool other_strip_covers_y =
      std::min(other_strip.endpoint_a.first.y, other_strip.endpoint_b.first.y)
          == min_y &&
          std::max(other_strip.endpoint_a.first.y,
                   other_strip.endpoint_b.first.y) == max_y;

  return (this_strip_covers_x && other_strip_covers_y)
      || (this_strip_covers_y && other_strip_covers_x);
}
}
}