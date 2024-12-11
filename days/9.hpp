#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <ranges>
#include <vector>

#include "../lib/file_views.hpp"

namespace pt1 {
inline std::vector<int> parse_disk(std::ranges::input_range auto &&input) {
  std::ifstream fstream{"inputs/9.txt"};
  std::vector<int> disk{};

  for (const auto [idx, v] : input | std::views::enumerate) {
    for (const auto _ : std::views::iota(0, v - '0')) {
      (void)_;
      if (idx % 2 == 0) {
        disk.push_back(idx / 2);
      } else {
        disk.push_back(-1);
      }
    }
  }

  return disk;
}
inline void compress(std::vector<int> &disk) {
  size_t space_idx = 0;
  auto file_idx = disk.size() - 1;

  while (true) {
    while (disk[space_idx] != -1) {
      ++space_idx;
    }

    while (disk[file_idx] == -1) {
      --file_idx;
    }

    if (space_idx >= file_idx) {
      break;
    }

    std::swap(disk[space_idx], disk[file_idx]);
  }
}
} // namespace pt1

namespace pt2 {
// one cache line omg
struct begin {
  int begin;
  int extent;
};

struct begin_id {
  long id;
  int begin;
  int extent;
};

struct filesystem {
  std::vector<begin> space{};
  std::vector<begin_id> files{};
};

inline filesystem parse_disk(std::ranges::input_range auto charstream) {
  filesystem disk{};
  int current_begin{0};
  for (const auto [idx, v] : charstream | std::views::enumerate) {
    const auto extent{v - '0'};
    if (idx % 2 == 0) {
      disk.files.push_back(
          {.id = idx / 2, .begin = current_begin, .extent = extent});
    } else {
      disk.space.push_back({.begin = current_begin, .extent = extent});
    }

    current_begin += extent;
  }

  return disk;
}

inline std::vector<int> compress(filesystem &&filesystem) {
  auto current_file_idx = (signed)(filesystem.files.size() - 1);
  while (current_file_idx >= 0) {
    for (auto space_idx : std::views::iota(0ul, filesystem.space.size())) {
      if (filesystem.space[space_idx].begin >=
          filesystem.files[current_file_idx].begin) {
        // any further move is backwards
        break;
      }

      if (filesystem.space[space_idx].extent >=
          filesystem.files[current_file_idx].extent) {
        // move the file to the correct place - this breaks sequentiallity
        // filesystem, but its not a property we need to preserve to rebuild the
        // checksum.
        filesystem.files[current_file_idx].begin =
            filesystem.space[space_idx].begin;

        // decrease the extent of the space...
        filesystem.space[space_idx].extent -=
            filesystem.files[current_file_idx].extent;
        assert(filesystem.space[space_idx].extent >= 0);

        // ...and increase the beginning of the space.
        filesystem.space[space_idx].begin +=
            filesystem.files[current_file_idx].extent;

        break;
      }
    }

    --current_file_idx;
  }

  // reformat it into a disk
  const auto last_file{std::ranges::max_element(
      filesystem.files, [](begin_id const &a, begin_id const &b) {
        return a.begin + a.extent < b.begin + b.extent;
      })};

  std::vector<int> disk(last_file->begin + last_file->extent, -1);

  for (const auto file : filesystem.files) {
    for (const auto file_idx :
         std::views::iota(file.begin, file.begin + file.extent)) {
      assert(disk[file_idx] == -1);
      disk[file_idx] = file.id;
    }
  }

  return disk;
};
} // namespace pt2

inline long checksum(std::vector<int> const &disk) {
  long sum{0};
  for (const auto [idx, v] : disk | std::views::enumerate) {
    if (v == -1) {
      continue;
    }

    sum += v * idx;
  }

  return sum;
}

#ifdef MAIN
int main() {
  std::ifstream fstream{"inputs/9.txt"};
  const auto charstream = [&fstream]() {
    fstream.clear();
    fstream.seekg(0, std::ios::beg);

    return std::views::istream<char_file_view>(fstream) |
           std::views::transform([](auto &v) { return v.get(); });
  };

  auto disk1 = pt1::parse_disk(charstream());
  pt1::compress(disk1);
  std::printf("checksum pt1: %ld\n", checksum(disk1));

  auto disk2 = pt2::parse_disk(charstream());
  const auto compressed = pt2::compress(std::move(disk2));
  std::printf("checksum pt2: %ld\n", checksum(compressed));
}
#endif
