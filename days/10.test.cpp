#include "10.hpp"
#include <vector>

#include "../lib/grid.hpp"

#include "gtest/gtest.h"

TEST(DAY10_PT1, SIMPLE) {
  const auto input = grid<char>::build_grid<'.'>({
      "0123",
      "1234",
      "8765",
      "9876",
  });

  EXPECT_EQ(find_trail_value_pt1(input), 1);
}

TEST(DAY10_PT1, EXAMPLE) {
  const auto input = grid<char>::build_grid<'.'>({
      "89010123",
      "78121874",
      "87430965",
      "96549874",
      "45678903",
      "32019012",
      "01329801",
      "10456732",
  });

  EXPECT_EQ(find_trail_value_pt1(input), 36);
}

TEST(DAY10_PT2, EXAMPLE) {
  const auto input = grid<char>::build_grid<'.'>({
      ".....0.",
      "..4321.",
      "..5..2.",
      "..6543.",
      "..7..4.",
      "..8765.",
      "..9....",
  });

  EXPECT_EQ(find_trail_value_pt2(input), 3);
}

#ifdef TEST_MAIN
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif
