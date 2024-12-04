#include <array>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <ranges>
#include <tuple>
#include <vector>

#include "../lib/file_views.hpp"

using grid = std::vector<std::vector<char>>;
using coord = std::tuple<int, int>;

grid build_xmas_matrix() {
  std::ifstream fstream{"inputs/4.txt"};
  auto char_iter{std::views::istream<char_file_view>(fstream)};
  grid grid{std::vector<char>{}};
  for (const auto [x, c_] : char_iter | std::views::enumerate) {
    const auto c = c_.get();
    if (c == '\n') {
      grid.push_back({});
    } else {
      grid.back().push_back(c);
    }
  }

  return grid;
};

std::vector<coord> find_xs(const grid &grid) {
  std::vector<coord> xs{};
  for (const auto &[y, row] : grid | std::views::enumerate) {
    for (const auto &[x, c] : row | std::views::enumerate) {
      if (c == 'X') {
        xs.push_back({x, y});
      }
    }
  }

  return xs;
}

bool check_in_dir(const grid &grid, const coord offset, const coord start) {
  std::vector<char> required_chars{'S', 'A', 'M', 'X'};
  coord current{start};

  const coord max_bounds{grid[0].size() - 1, grid.size() - 1};

  const auto fits_in_grid = [&grid](coord &coord) {
    const auto [x, y] = coord;
    const bool out_of_bounds_anywhere =
        x >= grid[0].size() || y >= grid.size() || x < 0 || y < 0;

    return !out_of_bounds_anywhere;
  };

  const auto has_letter = [&grid, &required_chars](coord &coord) {
    const auto [x, y] = coord;
    const auto isLetter = grid[y][x] == required_chars.back();
    return isLetter;
  };

  const auto make_next_coord = [&offset, &current]() -> coord {
    const auto [offsetX, offsetY] = offset;
    const auto [currentX, currentY] = current;

    return {offsetX + currentX, offsetY + currentY};
  };

  while (fits_in_grid(current) && has_letter(current)) {
    current = make_next_coord();
    required_chars.pop_back();
  }

  return required_chars.empty();
}

// ghetto constexpr vector
std::vector<coord> dirs = []() {
  std::vector<coord> r{};
  r.reserve(8);
  for (const auto a : std::array{-1, 0, 1}) {
    for (const auto b : std::array{-1, 0, 1}) {
      if (a == b && b == 0) {
        continue;
      }

      r.push_back({a, b});
    }
  }

  return r;
}();

int search_xmases(const grid &grid, const std::vector<coord> xs) {

  int found_xmas{0};
  for (const auto &xCoord : xs) {
    for (const auto &dir : dirs) {
      if (check_in_dir(grid, dir, xCoord)) {
        ++found_xmas;
      }
    }
  }

  return found_xmas;
}

int main() {
  const auto grid = build_xmas_matrix();
  const auto xs = find_xs(grid);

  const auto ret = search_xmases(grid, xs);
  std::printf("v: %d\n", ret);
}
