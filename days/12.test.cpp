#include "12.hpp"

#include "gtest/gtest.h"

TEST(DAY12, COUNT_PERIMETER) {
  const std::vector<std::pair<std::vector<std::string>, int>> tcs{
      {{
           "X..",
           "...",
           "...",
       },
       4},
      {{"XX.", "...", "..."}, 6},
      {{"XX.", ".X.", "..."}, 8},
  };

  for (const auto &[tc, v] : tcs) {
    const auto map{grid<char>::build_grid<'.'>(tc)};
    EXPECT_EQ(count_perimeter(map, {0, 0}), v);
  }
}

TEST(DAY12_PT1, EXAMPLE) {
  const std::vector<std::string> input = {
      "RRRRIICCFF", "RRRRIICCCF", "VVRRRCCFFF", "VVRCCCJFFF", "VVVVCJJCFE",
      "VVIVCCJJEE", "VVIIICJJEE", "MIIIIIJJEE", "MIIISIJEEE", "MMMISSJEEE",
  };

  const auto map{grid<char>::build_grid<'.'>(input)};
  EXPECT_EQ(count_fence_price(map), 1930);
}

TEST(DAY12_PT1, EXAMPLE_R_PERIM) {
  const std::vector<std::string> input = {
      "RRRRIICCFF", "RRRRIICCCF", "VVRRRCCFFF", "VVRCCCJFFF", "VVVVCJJCFE",
      "VVIVCCJJEE", "VVIIICJJEE", "MIIIIIJJEE", "MIIISIJEEE", "MMMISSJEEE",
  };

  const auto map{grid<char>::build_grid<'.'>(input)};
  EXPECT_EQ(count_perimeter(map, {0, 0}), 18);
}

TEST(DAY12_PT1, SIMPLE) {
  const std::vector<std::string> input = {
      "OOOOO", "OXOXO", "OOOOO", "OXOXO", "OOOOO",
  };

  const auto map{grid<char>::build_grid<'.'>(input)};
  EXPECT_EQ(count_fence_price(map), 772);
}

#ifdef TEST_MAIN
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif
