#include <gtest.h>

TEST(SimpleTest, sum_of_five_odd_numbers_equals_square)
{
  EXPECT_EQ(1 + 3 + 5 + 7 + 9, 5 * 5);
}

TEST(SimpleTest, sum_of_six_odd_numbers_equals_square)
{
  EXPECT_EQ(1 + 3 + 5 + 7 + 9 + 11, 6 * 6);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
