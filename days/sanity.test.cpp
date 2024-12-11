#include "gtest/gtest.h"

#include "../lib/test_util.hpp"

TEST(StringToVecInt, GivesValidOutput) {
 const auto out = string_to_vec_int("2997");

 ASSERT_EQ(out[0], 2);
 ASSERT_EQ(out[1], 9);
 ASSERT_EQ(out[2], 9);
 ASSERT_EQ(out[3], 7);
}

#ifdef TEST_MAIN
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif
