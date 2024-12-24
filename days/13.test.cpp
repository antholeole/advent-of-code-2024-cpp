#include "13.hpp"

#include "gtest/gtest.h"

TEST(DAY13, EXAMPLE) {
  const std::vector<machine> maks{{
                                      .a = {94, 34},
                                      .b = {22, 67},
                                      .prize = {8400, 5400},
                                  },

                                  {
                                      .a = {26, 66},
                                      .b = {67, 21},
                                      .prize = {12748, 12176},
                                  },
                                  {
                                      .a = {17, 86},
                                      .b = {84, 37},
                                      .prize = {7870, 6450},
                                  }};

  EXPECT_EQ(solve(maks[0], 0), 280);
  EXPECT_EQ(solve(maks[1], 0), 0);
  EXPECT_EQ(solve(maks[2], 0), 200);
}

TEST(DAY13, EXAMPLE_LONG) {
  EXPECT_EQ(solve(
                {
                    .a = {26, 66},
                    .b = {67, 21},
                    .prize = {12748, 12176},
                },
                10000000000000),
            10);
  EXPECT_EQ(
      solve(
          {.a = {10000000000000, 0}, .b = {0, 10000000000000}, .prize = {0, 0}},
          10000000000000),
      4);
}

#ifdef TEST_MAIN
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif
