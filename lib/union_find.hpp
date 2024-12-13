#pragma once
#include <algorithm>
#include <cassert>
#include <concepts>
#include <map>
#include <unordered_set>
#include <vector>

#include "grid.hpp"

namespace std {

template <class A, class B> struct hash<pair<A, B>> {
  size_t operator()(const pair<A, B> &p) const {
    return std::rotl(hash<A>{}(p.first), 1) ^ hash<B>{}(p.second);
  }
};
} // namespace std

template <typename T> struct union_find {

  union_find(Coord const &c)
    requires std::is_same_v<T, Coord>
  {
    for (const auto y : std::views::iota(0, c.second + 1)) {
      for (const auto x : std::views::iota(0, c.first + 1)) {
        parents[{x, y}] = {x, y};
      }
    }
  }

  union_find(std::vector<T> const &els) {
    for (const auto v : els) {
      parents[v] = v;
    }
  }

  void join(const T one, const T two) {
    const auto parent_one = find(one);
    const auto parent_two = find(two);

    parents[parent_one] = parent_two;
  }

  void print_to_grid() const
    requires std::is_same_v<T, Coord>
  {
    Coord bottom_right = {0, 0};
    int current_repr = 'A';
    
    std::map<Coord, char> parent_reprs{};
    for (const auto& [el, _] : parents) {
      const auto el_parent = find(el);
      if (!parent_reprs.contains(el_parent)) {
        parent_reprs[el_parent] = current_repr;
        ++current_repr;
        if (el.first + el.second > bottom_right.first + bottom_right.second) {
          bottom_right = el;
        }
      }
    }

    for (const auto y : std::views::iota(0, bottom_right.second + 1)) {
      for (const auto x : std::views::iota(0, bottom_right.first + 1)) {
        std::printf("%c", parent_reprs[find({x, y})]);
      }
      std::printf("\n");
    }
  };

  T find(const T el) const {
#if DEBUG
    if (!parents.contains(el)) {
      if constexpr (std::same_as<T, Coord>) {
        std::printf("%d %d is not in the map\n", el.first, el.second);
        assert(false);
      } else {
        assert(parents.contains(el));
      }
    }
#endif

    const auto el_parent = parents.at(el);
    if (el_parent == el) {
      return el;
    }

    return find(el_parent);
  }

  std::vector<T> all_parents() const {
    std::unordered_set<T> p{};
    for (const auto &[v, _] : parents) {
      (void)_;
      p.insert(find(v));
    }

    return std::vector<T>{p.cbegin(), p.cend()};
  }

  size_t get_size(T const &el) const {
    const auto parent = find(el);

    size_t ret{0};
    for (const auto &[v, _] : parents) {
      if (find(v) == parent) {
        ++ret;
      }
    }

    return ret;
  }

private:
  std::map<T, T> parents{};
};