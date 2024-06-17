#include "list.h"
#include <gtest/gtest.h>

class ListTest : public ::testing::Test {
protected:
  List<int> list;

  void SetUp() override {
    list.add(10);
    list.add(20);
    list.add(30);
  }

  void TearDown() override { list.clear(); }
};

TEST_F(ListTest, AddTest) {
  list.add(40);
  List<int>::Node *result = list.find(40);
  ASSERT_NE(result, nullptr);
  ASSERT_EQ(result->data, 40);
}

TEST_F(ListTest, RemoveTest) {
  list.remove(20);
  ASSERT_EQ(list.find(20), nullptr);
}

TEST_F(ListTest, FindTest) {
  List<int>::Node *result = list.find(30);
  ASSERT_NE(result, nullptr);
  ASSERT_EQ(result->data, 30);
}

TEST_F(ListTest, ClearTest) {
  list.clear();
  ASSERT_EQ(list.find(10), nullptr);
  ASSERT_EQ(list.find(20), nullptr);
  ASSERT_EQ(list.find(30), nullptr);
}

TEST_F(ListTest, IteratorTest) {
  int expected_sum = 60;
  int actual_sum = 0;
  for (int value : list) {
    actual_sum += value;
  }
  ASSERT_EQ(actual_sum, expected_sum);
}