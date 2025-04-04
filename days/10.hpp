#include "../lib/grid.hpp"
#include <queue>

constexpr char EXPLORED_MARKER = 'x';

constexpr std::array<Coord, 4> directions = {
    std::pair{-1, 0},
    {1, 0},
    {0, 1},
    {0, -1},
};

inline int find_trail_value_pt1(grid<char> const &map) {
  int res{0};

  for (const auto &start : map.get_all_elements('0')) {
    std::vector<Coord> to_explore = {start};
    grid<char> explored{map.get_size()};

    while (!to_explore.empty()) {
      const auto here = to_explore.back();
      to_explore.pop_back();

      for (const auto &dir : directions) {
        const auto neighbor_pos = grid<char>::increment_position(dir, here);
        if (!map.is_out_of_bounds(neighbor_pos) &&
            explored.element_at(neighbor_pos) != EXPLORED_MARKER) {
          const auto this_value = map.element_at(here).value_or('.') - '0';
          const auto there_value =
              map.element_at(neighbor_pos).value_or('.') - '0';

          if (there_value != this_value + 1) {
            continue;
          }

          if (there_value == 9) {
            ++res;
          } else {
            to_explore.push_back(neighbor_pos);
          }

          explored.put(EXPLORED_MARKER, neighbor_pos);
        }
      }
    }
  }

  return res;
};

inline int find_trail_value_pt2(grid<char> const &map) {
  size_t res{0};

  for (const auto &start : map.get_all_elements('0')) {
    std::queue<Coord> to_explore{};
    to_explore.push(start);

    grid<size_t> explored{map.get_size()};
    explored.put(1, start);

    while (!to_explore.empty()) {
      const auto here = to_explore.front();
      to_explore.pop();

      for (const auto &dir : directions) {
        const auto neighbor_pos = grid<char>::increment_position(dir, here);
        if (!map.is_out_of_bounds(neighbor_pos)) {
          const auto this_num_paths = explored.element_at(here).value();
          const auto there_num_paths{
              explored.element_at(neighbor_pos).value_or(0)};
          const auto this_value = map.element_at(here).value_or('.') - '0';
          const auto there_value =
              map.element_at(neighbor_pos).value_or('.') - '0';

          if (there_value != this_value + 1) {
            continue;
          }

          if (there_value == 9) {
            res += this_num_paths;
          } else if (there_num_paths == 0) {
            to_explore.push(neighbor_pos);
          }

          explored.put(there_num_paths + this_num_paths, neighbor_pos);
        }
      }
    }
  }

  return res;
};

#ifdef MAIN
int main() {
  const grid map = grid<char>::build_grid<'.'>("inputs/10.txt");
  std::printf("pt1: %d", find_trail_value_pt1(map));
  std::printf("pt2: %d", find_trail_value_pt2(map));
}
#endif
