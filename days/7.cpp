#include <cstdio>
#include <fstream>
#include <ranges>
#include <string>
#include <vector>

#include "../lib/file_views.hpp"

struct equation {
  std::vector<int> ops{};
  long res;
};

std::vector<equation> read_equations() {
  std::vector<equation> eqs{};
  std::ifstream fstream{"inputs/7.txt"};
  for (auto line : std::views::istream<lsv_file_view>(fstream)) {
    equation eq{};
    for (auto [idx, num] :
         line.get() | std::ranges::views::split(' ') | std::views::enumerate) {
      const auto n = std::stol(num.data());
      if (idx == 0) {
        eq.res = n;
      } else {
        eq.ops.push_back(n);
      }
    }
    eqs.push_back(eq);
  }
  return eqs;
}

bool solve_equation(equation const &eq) {
  const auto concat = [](long left, long right) -> long {
    return std::stol(std::to_string(left) + std::to_string(right));
  };

  const auto solve = [&eq, &concat](size_t v_idx, long prev_total, auto &solve) -> bool {
    if (v_idx >= eq.ops.size()) {
      return prev_total == eq.res;
    }

    return solve(v_idx + 1, prev_total + eq.ops[v_idx], solve) ||
           solve(v_idx + 1, prev_total * eq.ops[v_idx], solve) ||
           solve(v_idx + 1, concat(prev_total, eq.ops[v_idx]), solve);
  };

  return solve(0, 0, solve);
}

int main() {
  const auto eqs = read_equations();

  unsigned long long res{0};
  for (const auto &eq : eqs) {
    if (solve_equation(eq)) {
      std::printf("%ld is right\n", eq.res);
      res += eq.res;
    }
  }

  std::printf("res: %lld\n", res);
}