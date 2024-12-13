#include "gtest/gtest.h"
#include <algorithm>
#include <numeric>
#include <ranges>
#include <unordered_map>

#include "../lib/test_util.hpp"

#include "9.hpp"

namespace {
inline void print_disk(std::vector<int> const &disk) {
  std::for_each(disk.cbegin(), disk.cend(), [](int const &a) {
    if (a == -1) {

      std::printf(".");
    } else {

      std::printf("(%d)", a);
    }
  });
}
}; // namespace

TEST(DAY9_PT1, EXAMPLE) {
  auto input = std::string("2333133121414131402");
  auto disk = pt1::parse_disk(input);

  pt1::compress(disk);

  EXPECT_EQ(checksum(disk), 1928);
}

TEST(DAY9_PT2, EXAMPLE) {
  auto input = std::string("2333133121414131402");
  auto disk = pt2::parse_disk(input);

  const auto compressed = pt2::compress(std::move(disk));
  EXPECT_EQ(checksum(compressed), 2858);
}

TEST(DAY9_PT2, UNCOMPRESSABLE) {
  auto disk = pt2::filesystem{.space = {},
                              .files = {{
                                  .id = 1,
                                  .begin = 0,
                                  .extent = 5,
                              }}};

  const auto compressed = pt2::compress(std::move(disk));
  EXPECT_EQ(checksum(compressed), 10);
}

TEST(DAY9_PT2, SEQUENTIAL_OPEN) {
  auto input = std::string("0202202");
  auto disk = pt2::parse_disk(input);

  const auto compressed = pt2::compress(std::move(disk));
  EXPECT_EQ(checksum(compressed), 13);
}

TEST(DAY9_PT2, ALL_FILES_REPRESENTED_FULLY) {
  std::ifstream fstream{"inputs/9.txt"};
  auto input = std::views::istream<char_file_view>(fstream) |
               std::views::transform([](auto &v) { return v.get(); });

  auto disk = pt2::parse_disk(input);
  const auto original_disk = disk;

  const auto compressed = pt2::compress(std::move(disk));

  for (const auto file : original_disk.files) {
    const auto file_size_on_compressed_disk = std::accumulate(
        compressed.cbegin(), compressed.cend(), 0,
        [&file](int acc, int const v) { return v == file.id ? acc + 1 : acc; });
    ASSERT_EQ(file_size_on_compressed_disk, file.extent);
  }
}

TEST(DAY9_PT2, INTERNET_CASES) {
  const std::unordered_map<std::string, int> tc = {
      {"714892711", 813},
      {"1313165", 169},
      {"12101", 4},
      {"233313312141413140111", 2860},
      {"2833133121414131402", 2184}, // this broke me...
  };

  for (const auto &[test_case, v] : tc) {
    auto disk = pt2::parse_disk(test_case);
    const auto compressed = pt2::compress(std::move(disk));
    EXPECT_EQ(checksum(compressed), v);
  }
}

#ifdef TEST_MAIN
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif
