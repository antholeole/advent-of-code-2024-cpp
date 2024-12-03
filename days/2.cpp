#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <ranges>
#include <string>
#include <vector>

#include "../lib/file_views.hpp"

constexpr auto MAX_DELTA = 3;

const auto floor_view = [](auto &lsv) {
  auto floor =
      lsv.get() | std::views::split(' ') | std::views::transform([](auto &&num) {
        std::string report;
        std::ranges::copy(num, std::back_inserter(report));
        return std::stoi(report);
      });

  std::vector<int> vec{};
  for (auto v : floor) {
    vec.push_back(v);
  }
  return vec;
};

const auto is_valid_pair = [](int first, int second, int direction) -> bool {
  const int diff = second - first;

  if (diff == 0 || abs(diff) > MAX_DELTA) {
    return false;
  }

  if (diff > 0 && direction < 0) {
    return false;
  }

  if (diff < 0 && direction > 0) {
    return false;
  }

  return true;
};

int main() {
  std::ifstream fstream{"inputs/2.txt"};
  auto each_floor{std::views::istream<lsv_file_view>(fstream) |
                  std::views::transform(floor_view)};

  int num_valid{0};
  for (auto [floor_idx, floor_vec] : each_floor | std::views::enumerate) {
    for (int idx_to_skip : std::views::iota(0uz, floor_vec.size())) {

      int direction{0};
      bool invalid{false};
      for (int idx : std::views::iota(0uz, floor_vec.size())) {
        if (idx == idx_to_skip) {
          continue;
        }

        const auto prev_idx{idx_to_skip == idx - 1 ? idx - 2 : idx - 1};
        if (prev_idx < 0) {
          continue;
        }

        const auto prev{floor_vec[prev_idx]};
        const auto curr{floor_vec[idx]};

        if (direction == 0) {
          direction = curr > prev ? 1 : -1;
        }

        if (!is_valid_pair(prev, curr, direction)) {
          invalid = true;
          continue;
        }
      }

      if (!invalid) {
        ++num_valid;
        break;
      }
    }
  }

  std::printf("num valid floors: %d", num_valid);
}