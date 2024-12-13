#pragma once
#include <cstddef>
#include <ranges>
#include <unordered_map>

struct uf {
  uf(size_t els) {
    for (const auto v : std::views::iota(0ul, els)) {
      parents[v] = v;
    }
  }

  void join(const size_t one, const size_t two) {
    const auto parent_one = find(one);
    const auto parent_two = find(two);

    parents[parent_one] = parent_two;
  }

  size_t find(const size_t el) const {
    const auto el_parent = parents.at(el);
    if (el_parent == el) {
      return el;
    }

    return find(el_parent);
  }

private:
  std::unordered_map<size_t, size_t> parents{};
};