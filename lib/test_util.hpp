#pragma once

#include <algorithm>
#include <string>
#include <vector>

inline std::vector<int> string_to_vec_int(std::string &&str) {
  std::vector<int> ret(str.cbegin(), str.cend());

  std::transform(ret.cbegin(), ret.cend(), ret.begin(),
                 [](int v) { return v - '0'; });

  return ret;
};