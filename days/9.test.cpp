#include "gtest/gtest.h"

#include "../lib/test_util.hpp"

#include "9.hpp"

TEST(TestSuiteName, TestName) {
  auto input = std::string("2333133121414131402");
  auto disk = parse_disk(input);

  compress(disk);

  EXPECT_EQ(checksum(disk), 1928);
}

#ifdef TEST_MAIN
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif
