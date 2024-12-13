#include "../lib/grid.hpp"
#include "../lib/union_find.hpp"
#include <cstdio>
#include <queue>

inline int count_perimeter(grid<char> const &map, Coord const &at) {
  const auto char_at = map.element_at(at).value();

  std::queue<Coord> to_explore{};
  to_explore.emplace(at);
  grid<char> explored{map.get_size()};
  explored.put(char_at, at);

  int perimeter{0};
  while (!to_explore.empty()) {
    const auto curr = to_explore.front();
    to_explore.pop();

    for (const auto &cardinal : grid<char>::cardinals) {
      const auto neighbor = map.increment_position(cardinal, curr);

      const auto neighbor_element = map.element_at(neighbor).value_or('.');

      if (neighbor_element != char_at) {
        ++perimeter;
        continue;
      }

      if (!explored.element_at(neighbor).has_value()) {
        explored.put(char_at, neighbor);
        to_explore.push(neighbor);
      }
    }
  }

  return perimeter;
}

inline int count_fence_price(grid<char> const &map) {
  union_find<Coord> uf(map.get_size());
  for (const auto y : std::views::iota(0, map.get_size().second)) {
    for (const auto x : std::views::iota(0, map.get_size().first)) {
      for (const auto &cardinal : grid<size_t>::cardinals) {
        const auto neighbor = map.increment_position(cardinal, {x, y});

        // join them if they should be joined
        if (map.element_at({x, y}).value_or('.') ==
            map.element_at(neighbor).value_or(':')) {
          uf.join(neighbor, {x, y});
        }
      }
    }
  }

  int acc{0};
  for (const auto &parent : uf.all_parents()) {
    acc += count_perimeter(map, parent) * uf.get_size(parent);
  }

  return acc;
};

#ifdef MAIN
int main() {}
#endif
