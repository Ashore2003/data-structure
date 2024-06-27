#include "array.h"
#include <gtest/gtest.h>

class ArrayTest : public ::testing::Test {
protected:
  userDefineDataStructure::Array<int, 5> arr;

  void SetUp() override {
    arr.fill(2); 
  }
};

TEST_F(ArrayTest, AccessWithinBounds) {
  EXPECT_NO_THROW({
    EXPECT_EQ(arr[0], 2);
    EXPECT_EQ(arr[4], 2);
    arr.at(1); 
  });
}

TEST_F(ArrayTest, AccessOutOfBounds) {
  EXPECT_THROW(arr.at(5), std::out_of_range);
  EXPECT_THROW(arr.at(-1), std::out_of_range);
}

TEST_F(ArrayTest, CheckFrontAndBack) {
  EXPECT_EQ(arr.front(), 2);
  EXPECT_EQ(arr.back(), 2);
  arr.fill(3);
  EXPECT_EQ(arr.front(), 3);
  EXPECT_EQ(arr.back(), 3);
}

TEST_F(ArrayTest, Fill) {
  arr.fill(1);
  for (size_t i = 0; i < arr.size(); i++) {
    EXPECT_EQ(arr[i], 1);
  }
}
