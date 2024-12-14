#include <algorithm>
#include <climits>
#include <cstdio>
#include <fstream>
#include <limits>
#include <ranges>
#include <regex>
#include <utility>

#include "../lib/file_views.hpp"

constexpr int A_COST = 3;
constexpr int B_COST = 1;

struct machine {
  std::pair<int, int> a;
  std::pair<int, int> b;
  std::pair<int, int> prize;
};

inline int solve(machine const &machine) {
  int min_cost{std::numeric_limits<int>::max()};
  for (const auto a_moves : std::views::iota(0)) {
    if (machine.prize.first - (machine.a.first * a_moves) < 0 ||
        machine.prize.second - (machine.a.second * a_moves) < 0) {
      break;
    }

    for (const auto b_moves : std::views::iota(0)) {
      const std::pair<int, int> current_pos{
          machine.a.first * a_moves + machine.b.first * b_moves,
          machine.a.second * a_moves + machine.b.second * b_moves,
      };

      if (current_pos.first == machine.prize.first &&
          current_pos.second == machine.prize.second) {
        min_cost = std::min(min_cost, a_moves * A_COST + b_moves * B_COST);
      }

      if (current_pos.first > machine.prize.first ||
          current_pos.second > machine.prize.second) {
        break;
      }
    }
  }

  return min_cost == std::numeric_limits<int>::max() ? 0 : min_cost;
};

#ifdef MAIN
int main() {
  std::ifstream stream("inputs/13.txt");

  const std::regex number_regex(".*X[+|=]([0-9]+), Y[+|=]([0-9]+)");

  const auto extract_xy = [&number_regex](const std::string &str) {
    
    std::smatch matches;
    std::regex_search(str.cbegin(), str.cend(), matches, number_regex);
    
    return std::pair(std::stoi(matches[1]), std::stoi(matches[2]));
  };

  std::vector<machine> machines{{}};
  for (const auto &line : std::views::istream<lsv_file_view>(stream)) {
    const std::string& l{line};
    if (l.empty()) {
      machines.emplace_back();
      continue;
    }

    const auto xy = extract_xy(line);
    if (l.rfind("Button A", 0) == 0) {
      machines.back().a = xy;
    } else if (l.rfind("Button B", 0) == 0) {
      machines.back().b = xy;
    } else if (l.rfind("Prize", 0) == 0) {
      machines.back().prize = xy;
    }
  }

  uint64_t min{0};
  for (const auto &machine : machines) {
    min += solve(machine);
  }

  
    std::printf("pt1: %lu\n", min);
}
#endif
