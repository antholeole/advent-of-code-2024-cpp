#include "11.hpp"

#include "gtest/gtest.h"
#include <cstdint>

TEST(DAY11_PT1, SIMPLE) {
  const std::vector<uint64_t> stones = {125, 17};

  const auto blink_6{stone_blink(stones, 6)};
  ASSERT_EQ(blink_6, 22);

  const auto blink_25{stone_blink(stones, 25)};
  ASSERT_EQ(blink_25, 55312);
}

#ifdef TEST_MAIN
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif
