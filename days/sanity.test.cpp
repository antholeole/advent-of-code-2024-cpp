#include "gtest/gtest.h"

#include "../lib/grid.hpp"
#include "../lib/test_util.hpp"
#include "../lib/union_find.hpp"

TEST(StringToVecInt, GivesValidOutput) {
  const auto out = string_to_vec_int("2997");

  ASSERT_EQ(out[0], 2);
  ASSERT_EQ(out[1], 9);
  ASSERT_EQ(out[2], 9);
  ASSERT_EQ(out[3], 7);
}

TEST(UnionFind, Works) {
  union_find<Coord> uf({5, 5});

  ASSERT_EQ(uf.find({3, 3}), uf.find({3, 3})) << "A should be its own parent";

  uf.join({3, 3}, {3, 2});
  ASSERT_EQ(uf.find({3, 3}), uf.find({3, 2}))
      << "after joining parents should be the same";

  uf.join({3, 2}, {3, 3}); // backjoin should not change anything0

  uf.join({3, 3}, {3, 1});
  ASSERT_EQ(uf.find({3, 2}), uf.find({3, 1}))
      << "siblings should have same parent";

  ASSERT_NE(uf.find({3, 4}), uf.find({3, 1}))
      << "unrelated should have different same parent";

  ASSERT_EQ(uf.all_parents().size(), (6 * 6) - 2)
      << "num parents should be equal to num groups";

  uf.join({5, 5}, {5, 4});
  
  ASSERT_EQ(uf.find({5, 5}), uf.find({5, 4}))
      << "matches along edges should work";
}

#ifdef TEST_MAIN
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif
