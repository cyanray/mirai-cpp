#include <gtest/gtest.h>

TEST(ABC, TEST1) {
    EXPECT_EQ(true, true);
}

TEST(ABC, TEST2) {
    ASSERT_TRUE(2 == 2);
}

TEST(ABC, TEST3) {
    ASSERT_TRUE(23 == 2);
}