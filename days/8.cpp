#include "../lib/grid.hpp"
#include <utility>

constexpr char antinode_marker = '@';

int count_antinodes(grid const &original_grid) {
  grid antinode_map{original_grid.get_size()};
  for (const auto frequency : original_grid.get_special_objects()) {
    for (const auto &[ax, ay] : original_grid.get_all_elements(frequency)) {
      for (const auto &[bx, by] : original_grid.get_all_elements(frequency)) {
        if (ax == bx && ay == by) {
          continue;
        }

        const auto [run, rise] = std::pair{abs(ax - bx), abs(ay - by)};

        Coord antinode_a{ax, ay};
        Coord antinode_b{bx, by};

        // make a the leftmost node
        if (antinode_a.first > antinode_b.first) {
          std::swap(antinode_b, antinode_a);
        }

        antinode_a = {antinode_a.first - run, antinode_a.second};
        antinode_b = {antinode_b.first + run, antinode_b.second};

        // make a the topmost node
        if (antinode_a.second > antinode_b.second) {
          std::swap(antinode_a, antinode_b);
        }

        antinode_a = {antinode_a.first, antinode_a.second - rise};
        antinode_b = {antinode_b.first, antinode_b.second + rise};

        antinode_map.put(antinode_marker, std::move(antinode_a));
        antinode_map.put(antinode_marker, std::move(antinode_b));
      }
    }

  }

  return antinode_map.get_all_elements(antinode_marker).size();
};

int main() {
  auto grid = grid::build_grid<'.'>("inputs/8.txt");
  const auto num_antinodes = count_antinodes(grid);
  std::printf("num antinodes: %d\n", num_antinodes);
}
