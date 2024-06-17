#include "queue.h"
#include <gtest/gtest.h>

class QueueTest : public ::testing::Test {
protected:
  Queue<int> queue{5};

  void SetUp() override {
    queue.enqueue(1);
    queue.enqueue(2);
    queue.enqueue(3);
  }
};

TEST_F(QueueTest, EnqueueDequeueTest) {
  ASSERT_NO_THROW(queue.enqueue(4));
  ASSERT_NO_THROW(queue.enqueue(5));
  ASSERT_THROW(queue.enqueue(6), std::runtime_error); 
  ASSERT_EQ(queue.front(), 1);
  ASSERT_NO_THROW(queue.dequeue());
  ASSERT_EQ(queue.front(), 2);
}

TEST_F(QueueTest, FrontEmptyTest) {
  ASSERT_EQ(queue.front(), 1);
  queue.dequeue();
  queue.dequeue();
  queue.dequeue();
  ASSERT_THROW(queue.front(), std::runtime_error);
  ASSERT_TRUE(queue.empty());
}

TEST_F(QueueTest, IteratorTest) {
  int sum = 0;
  for (auto it = queue.begin(); it != queue.end(); ++it) {
    sum += *it;
  }
  ASSERT_EQ(sum, 6); 
}

