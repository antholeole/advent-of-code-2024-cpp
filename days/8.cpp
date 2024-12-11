#include "../lib/grid.hpp"
#include <cstdio>
#include <utility>

constexpr char antinode_marker = '@';

std::pair<int, int> count_antinodes(grid const &original_grid) {
  // for pt 1
  grid spot_antinode_map{original_grid.get_size()};
  grid linear_antinode_map{original_grid.get_size()};
  for (const auto frequency : original_grid.get_special_objects()) {
    for (const auto &[ax, ay] : original_grid.get_all_elements(frequency)) {
      for (const auto &[bx, by] : original_grid.get_all_elements(frequency)) {
        if (ax == bx && ay == by) {
          continue;
        }

        const auto [run, rise] = std::pair{abs(ax - bx), abs(ay - by)};

        Coord antinode_a{ax, ay};
        Coord antinode_b{bx, by};

        bool first_iter{true};
        while (!original_grid.is_out_of_bounds(antinode_a) ||
               !original_grid.is_out_of_bounds(antinode_b)) {
          linear_antinode_map.put(antinode_marker, antinode_a);
          linear_antinode_map.put(antinode_marker, antinode_b);
                
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

        //   std::printf("ax: %d, ay: %d, bx: %d, by: %d\n", antinode_a.first,
        //               antinode_a.second, antinode_b.first, antinode_b.second);

          if (first_iter) {
            spot_antinode_map.put(antinode_marker, antinode_a);
            spot_antinode_map.put(antinode_marker, antinode_b);
            first_iter = false;
          }
        }
      }
    }
  }

  return {spot_antinode_map.get_all_elements(antinode_marker).size(),
          linear_antinode_map.get_all_elements(antinode_marker).size()};
};

int main() {
  auto grid = grid::build_grid<'.'>("inputs/8.txt");
  const auto [num_spot_antinodes, num_linear_antinodes] = count_antinodes(grid);
  std::printf("spot: %d. linear: %d\n", num_spot_antinodes,
              num_linear_antinodes);
}
