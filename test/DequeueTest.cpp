#include "dequeue.h"
#include <gtest/gtest.h>

class DequeTest : public ::testing::Test {
protected:
  Deque<int> deque;

  void SetUp() override {
   
  }

  void TearDown() override {
    
  }
};

TEST_F(DequeTest, PushAndPopFront) {
  ASSERT_TRUE(deque.empty());
  deque.push_front(10);
  ASSERT_EQ(deque.front(), 10);
  ASSERT_EQ(deque.back(), 10);
  ASSERT_FALSE(deque.empty());

  deque.push_front(20);
  ASSERT_EQ(deque.front(), 20);
  ASSERT_EQ(deque.back(), 10);

  deque.pop_front();
  ASSERT_EQ(deque.front(), 10);
  deque.pop_front();
  ASSERT_TRUE(deque.empty());
}

TEST_F(DequeTest, PushAndPopBack) {
  ASSERT_TRUE(deque.empty());
  deque.push_back(10);
  ASSERT_EQ(deque.front(), 10);
  ASSERT_EQ(deque.back(), 10);

  deque.push_back(20);
  ASSERT_EQ(deque.front(), 10);
  ASSERT_EQ(deque.back(), 20);

  deque.pop_back();
  ASSERT_EQ(deque.back(), 10);
  deque.pop_back();
  ASSERT_TRUE(deque.empty());
}

TEST_F(DequeTest, MixedOperations) {
  deque.push_back(1);
  deque.push_front(2);
  deque.push_back(3);
  deque.push_front(4);

  ASSERT_EQ(deque.front(), 4);
  ASSERT_EQ(deque.back(), 3);
  ASSERT_EQ(deque.size(), 4);

  deque.pop_front();
  deque.pop_back();
  ASSERT_EQ(deque.front(), 2);
  ASSERT_EQ(deque.back(), 1);
  ASSERT_EQ(deque.size(), 2);
}
