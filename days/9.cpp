#include <algorithm>
#include <fstream>
#include <ranges>
#include <vector>

#include "../lib/file_views.hpp"

std::vector<int> parse_disk() {
  std::ifstream fstream{"inputs/9.txt"};
  std::vector<int> disk{};

  for (const auto [idx, v] :
       std::views::istream<char_file_view>(fstream) | std::views::enumerate) {
    for (const auto _ : std::views::iota(0, v.get() - '0')) {
      if (idx % 2 == 0) {
        disk.push_back(idx / 2);
      } else {
        disk.push_back(-1);
      }
    }
  }

  return disk;
}

void compress(std::vector<int> &disk) {
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

long checksum(std::vector<int> const &disk) {
  long sum{0};
  for (const auto [idx, v] : disk | std::views::enumerate) {
    if (v == -1) {
      break;
    }

    sum += v * idx;
  }

  return sum;
}

int main() {
  auto disk = parse_disk();
  compress(disk);

  std::printf("checksum: %ld\n", checksum(disk));
}