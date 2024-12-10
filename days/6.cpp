#include <array>
#include <cstdio>
#include <format>
#include <fstream>
#include <ranges>
#include <stdexcept>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

#include "../lib/file_views.hpp"

enum class face_direction { horizontal, vertical, both };

using Coord = std::pair<int, int>;
using CoordMap = std::unordered_map<int, std::unordered_set<int>>;
using DirectionMap =
    std::unordered_map<int, std::unordered_map<int, face_direction>>;

struct guard_map {
  CoordMap obstacles;
  Coord size;
  Coord guard_pos;
  Coord step_dir{0, -1};
};

guard_map read_map() {
  guard_map map{};

  std::ifstream fstream{"inputs/6.test.txt"};
  auto lines{std::views::istream<lsv_file_view>(fstream) |
             std::views::transform(
                 [](auto &lsv) -> std::string & { return lsv.get(); })};

  for (auto [row, line] : lines | std::views::enumerate) {
    for (auto [col, element] : line | std::views::enumerate) {
      switch (element) {
      case '.':
        break;
      case '#':
        map.obstacles[col].insert(row);
        break;
      case '^':
        map.guard_pos = {col, row};
        break;
      }

      // not the best strategy but this value is left at
      // the bottom right corner due to iteration order
      map.size = {col, row};
    }
  }

  return map;
};

template <typename T, T up, T right, T down, T left>
T on_dirs(std::pair<int, int> step_dir) {
  const auto &[x, y] = step_dir;
  if (x == 0 && y == 1) {
    return down;
  } else if (x == 0 && y == -1) {
    return up;
  } else if (x == -1 && y == 0) {
    return left;
  } else if (x == 1 && y == 0) {
    return right;
  } else {
    throw std::runtime_error(std::format("x, y %d %d is bad input", x, y));
  }
}

const auto turn_right =
    [](std::pair<int, int> step_dir) -> std::pair<int, int> {
  return on_dirs<Coord, {1, 0}, {0, 1}, {-1, 0}, {0, -1}>(step_dir);
};

const auto get_direction = [](std::pair<int, int> step_dir) {
  return on_dirs<face_direction, face_direction::vertical,
                 face_direction::horizontal, face_direction::vertical,
                 face_direction::horizontal>(step_dir);
};

const auto increment_position = [](Coord step_dir,
                                   Coord current) -> std::pair<int, int> {
  const auto [guard_x, guard_y] = current;
  const auto [delta_x, delta_y] = step_dir;
  return {guard_x + delta_x, guard_y + delta_y};
};

struct guard_positions {
  DirectionMap repeated_positions;
  CoordMap unique_positions;
  CoordMap candidate_barrel_position;
};

// returns NONE if the guard map gives a cycle.
std::optional<guard_positions> get_unique_positions(guard_map &map) {
  guard_positions positions{};

  Coord current = map.guard_pos;
  Coord step_dir = map.step_dir;

  const auto is_out_of_bounds = [&map](Coord check) {
    const auto [max_x, max_y] = map.size;
    const auto [x, y] = check;
    return (max_x < x || max_y < y || x < 0 || y < 0);
  };

  const auto is_barricade = [&map](Coord check) {
    const auto [x, y] = check;

    return map.obstacles[x].contains(y);
  };

  while (true) {
    const auto [new_x, new_y] = current;

    // if its not unique...
    if (positions.unique_positions[new_x].contains(new_y)) {
      // and we've already hit this path...
      if (positions.repeated_positions[new_x].contains(new_y)) {

        const auto prev_dir = positions.repeated_positions[new_x][new_y];

        // if we've already faced this direction at this point, we've hit a
        // loop.
        if (prev_dir == get_direction(step_dir)) {
          return std::optional<guard_positions>{};
        } else {
          // say that we've faced both directons.
          positions.repeated_positions[new_x][new_y] = face_direction::both;
        }

      } else {
        // otherwise, just insert this direction.
        positions.repeated_positions[new_x][new_y] = get_direction(step_dir);
      }

      // add a candidate barrel position where we're facing.
      const auto [infront_x, infront_y] = increment_position(step_dir, current);
      positions.candidate_barrel_position[infront_x].insert(infront_y);
    }

    positions.unique_positions[new_x].insert(new_y);

    auto next_pos = increment_position(step_dir, current);
    while (is_barricade(next_pos)) {
      step_dir = turn_right(step_dir);
      next_pos = increment_position(step_dir, current);
    }

    current = next_pos;
    if (is_out_of_bounds(next_pos)) {
      break;
    }
  }

  return positions;
}

int num_barrel_loops(guard_map &original_map, guard_positions &positions) {
  const auto place_barrel = [](guard_map new_map,
                               Coord const &barrel_pos) -> guard_map {
    const auto [x, y] = barrel_pos;
    new_map.obstacles[x].insert(y);
    return new_map;
  };

  auto cycle_creation{0};
  for (const auto &[candidate_x, y_set] : positions.candidate_barrel_position) {
    for (const auto candidate_y : y_set) {
        std::printf("%d %d\n", candidate_x, candidate_y);
      auto new_map = place_barrel(original_map, {candidate_x, candidate_y});
      const auto v = get_unique_positions(new_map);
      if (!v) {
        ++cycle_creation;
        std::printf("valid!\n");
      }
    }
  }

  return cycle_creation;
}

int main() {
  auto map = read_map();
  auto positions = get_unique_positions(map);

  // solve pt 1
  int unique_steps{0};
  for (const auto &[x, vals] : positions->unique_positions) {
    unique_steps += vals.size();
  }
  std::printf("pt1: %d\n", unique_steps);

  // solve pt 2
  const auto num_loops = num_barrel_loops(map, *positions);
  std::printf("pt2: %d\n", num_loops);
}