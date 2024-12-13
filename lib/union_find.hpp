#pragma once
#include <unordered_map>
#include <vector>

template <typename T> struct union_find {
  union_find(std::vector<T> els) {
    for (const auto v : els) {
      parents[v] = v;
    }
  }

  void join(const T one, const T two) {
    const auto parent_one = find(one);
    const auto parent_two = find(two);

    parents[parent_one] = parent_two;
  }

  T find(const T el) const {
    const auto el_parent = parents.at(el);
    if (el_parent == el) {
      return el;
    }

    return find(el_parent);
  }

private:
  std::unordered_map<T, T> parents{};
};