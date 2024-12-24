#include <atomic>
#include <climits>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <ranges>
#include <regex>
#include <utility>

#include "../lib/file_views.hpp"

constexpr int A_COST = 3;
constexpr int B_COST = 1;

struct machine {
  std::pair<uint64_t, uint64_t> a;
  std::pair<uint64_t, uint64_t> b;
  std::pair<uint64_t, uint64_t> prize;
};

// I think this is silly: in reality, there is more than one possible solution
// to each input. the test cases only have one solution, but it does not
// consider a case where there are two possible solutions, or where you should
// take more higher cost steps.
inline int solve(machine const &machine, uint64_t const prize_offset) {
  double D = (machine.a.first * machine.b.second) -
             (machine.a.second * machine.b.first);

  double Dx = ((machine.prize.first + prize_offset) * machine.b.second) -
              ((machine.prize.second + prize_offset) * machine.b.second);

  double Dy = (machine.a.first * (machine.prize.second + prize_offset)) -
              (machine.b.second * (machine.prize.first + prize_offset));

  double x = Dx / D;
  double y = Dy / D;

  // if we can't land exactly, leave.
  if ((unsigned long long)x == x && (unsigned long long)y == y) {
    return x * A_COST + y * B_COST;
  }

  return 0;
};

#ifdef MAIN
int main() {
  std::ifstream stream("inputs/13.txt");

  const std::regex number_regex(".*X[+|=]([0-9]+), Y[+|=]([0-9]+)");

  const auto extract_xy = [&number_regex](const std::string &str) {
    std::smatch matches;
    std::regex_search(str.cbegin(), str.cend(), matches, number_regex);

    return std::pair(std::stoll(matches[1]), std::stoll(matches[2]));
  };

  std::vector<machine> machines{{}};
  for (const auto &line : std::views::istream<lsv_file_view>(stream)) {
    const std::string &l{line};
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

  std::atomic_uint64_t result_1;
  std::atomic_uint64_t result_2;
  std::vector<std::thread> handles{};
  for (const auto &[idx, machine] : machines | std::views::enumerate) {
    handles.push_back(std::thread{[machine, &result_1, &result_2] {
      const auto res_1{solve(machine, 0)};
      result_1.fetch_add(res_1);

      const auto res_2{solve(machine, 10000000000000)};
      result_2.fetch_add(res_2);
    }});
  }

  for (auto &handle : handles) {
    handle.join();
  }

  std::printf("pt1: %lu\n", result_1.load());
  std::printf("pt2: %lu\n", result_2.load());
}
#endif
