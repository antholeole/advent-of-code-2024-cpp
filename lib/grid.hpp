#pragma once

#include <fstream>
#include <optional>
#include <ranges>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "file_views.hpp"

using Coord = std::pair<int, int>;

template <typename T> struct grid {
  grid(Coord &&size) : size{size} {};
  grid() {};

  template <char IgnoreChar>
  static grid<char> build_grid(std::string &&filename) {
    grid grid{};

    std::ifstream fstream{filename};
    auto lines{std::views::istream<lsv_file_view>(fstream) |
               std::views::transform(
                   [](auto &lsv) -> std::string & { return lsv.get(); })};

    return build_grid_from_iter<IgnoreChar>(lines);
  };

  template <char IgnoreChar>
  static grid<char> build_grid(std::vector<std::string> &&lines) {
    grid grid{};
    return build_grid_from_iter<IgnoreChar>(std::move(lines));
  };

  Coord get_size() const { return size; };

  bool is_out_of_bounds(Coord const &check) const {
    const auto [max_x, max_y] = size;
    const auto [x, y] = check;
    return (max_x < x || max_y < y || x < 0 || y < 0);
  };

  static std::pair<int, int> increment_position(Coord step_dir, Coord current) {
    const auto [a_x, a_y] = current;
    const auto [delta_x, delta_y] = step_dir;
    return {a_x + delta_x, a_y + delta_y};
  };

  std::unordered_set<char> get_special_objects() const {
    std::unordered_set<char> ret{};

    for (const auto &[c, _] : special_objects) {
      ret.insert(c);
    }

    return ret;
  };

  // putting something where something already exists is UB.
  void put(const T c, Coord const &at) {
    if (is_out_of_bounds(at)) {
      return;
    }

    const auto &[x, y] = at;
    backmap[x][y] = c;
    special_objects[c][x].insert(y);
  }

  std::vector<Coord> get_all_elements(char c) const {
    std::vector<Coord> ret{};
    if (!special_objects.contains(c)) {
      return ret;
    }

    ret.reserve(special_objects.at(c).size());
    for (const auto &[x, ys] : special_objects.at(c)) {
      for (const auto &y : ys) {
        ret.push_back(std::pair{x, y});
      }
    }

    return ret;
  };

  std::optional<char> element_at(Coord const &coord) const {
    const auto &[x, y] = coord;

    if (!backmap.contains(x)) {
      return std::nullopt;
    }

    if (!backmap.at(x).contains(y)) {
      return std::nullopt;
    }

    return backmap.at(x).at(y);
  };

private:
  Coord size;
  std::unordered_map<int, std::unordered_map<int, T>> backmap{};
  std::unordered_map<T, std::unordered_map<int, std::unordered_set<int>>>
      special_objects{};

  template <char IgnoreChar>
  static grid<char> build_grid_from_iter(auto &&lines) {
    grid grid{};

    for (auto [row, line] : lines | std::views::enumerate) {
      for (auto [col, element] : line | std::views::enumerate) {
        // not the best strategy but this value is left at
        // the bottom right corner due to iteration order
        grid.size = {col, row};

        if (element == IgnoreChar) {
          continue;
        }

        grid.special_objects[element][col].insert(row);
        grid.backmap[col][row] = element;
      }
    }

    return grid;
  }
};