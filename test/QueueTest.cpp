#include "queue.h"
#include <gtest/gtest.h>

class QueueTest : public ::testing::Test {
protected:
    userDefineDataStructure::Queue<int> queue;

    void SetUp() override {
        // This method will be called before each test
        // You can do any set-up work for each test here.
    }

    void TearDown() override {
        // This method will be called after each test
        // You can do any clean-up work for each test here.
    }
};

TEST_F(QueueTest, DefaultConstructor) {
    EXPECT_TRUE(queue.empty());
    EXPECT_EQ(queue.size(), 0);
}


TEST_F(QueueTest, PushAndFront) {
    queue.push(1);
    EXPECT_EQ(queue.front(), 1);
    EXPECT_EQ(queue.size(), 1);

    queue.push(2);
    EXPECT_EQ(queue.front(), 1);
    EXPECT_EQ(queue.size(), 2);
}


TEST_F(QueueTest, PushAndBack) {
    queue.push(1);
    EXPECT_EQ(queue.back(), 1);
    EXPECT_EQ(queue.size(), 1);

    queue.push(2);
    EXPECT_EQ(queue.back(), 2);
    EXPECT_EQ(queue.size(), 2);
}


TEST_F(QueueTest, Pop) {
    queue.push(1);
    queue.push(2);
    queue.push(3);

    queue.pop();
    EXPECT_EQ(queue.front(), 2);
    EXPECT_EQ(queue.size(), 2);

    queue.pop();
    EXPECT_EQ(queue.front(), 3);
    EXPECT_EQ(queue.size(), 1);

    queue.pop();
    EXPECT_TRUE(queue.empty());
}

TEST_F(QueueTest, EmptyAndSize) {
    EXPECT_TRUE(queue.empty());
    EXPECT_EQ(queue.size(), 0);

    queue.push(1);
    EXPECT_FALSE(queue.empty());
    EXPECT_EQ(queue.size(), 1);

    queue.push(2);
    EXPECT_FALSE(queue.empty());
    EXPECT_EQ(queue.size(), 2);

    queue.pop();
    queue.pop();
    EXPECT_TRUE(queue.empty());
    EXPECT_EQ(queue.size(), 0);
}

TEST_F(QueueTest, CopyConstructor) {
    queue.push(1);
    queue.push(2);
    queue.push(3);

    userDefineDataStructure::Queue<int> queue2(queue);
    EXPECT_EQ(queue2.size(), 3);
    EXPECT_EQ(queue2.front(), 1);
    EXPECT_EQ(queue2.back(), 3);
}

TEST_F(QueueTest, MoveConstructor) {
    queue.push(1);
    queue.push(2);
    queue.push(3);

    userDefineDataStructure::Queue<int> queue2(std::move(queue));
    EXPECT_EQ(queue2.size(), 3);
    EXPECT_EQ(queue2.front(), 1);
    EXPECT_EQ(queue2.back(), 3);
    EXPECT_TRUE(queue.empty());
}

TEST_F(QueueTest, CopyAssignment) {
    queue.push(1);
    queue.push(2);
    queue.push(3);

    userDefineDataStructure::Queue<int> queue2;
    queue2 = queue;
    EXPECT_EQ(queue2.size(), 3);
    EXPECT_EQ(queue2.front(), 1);
    EXPECT_EQ(queue2.back(), 3);
}

TEST_F(QueueTest, MoveAssignment) {
    queue.push(1);
    queue.push(2);
    queue.push(3);

    userDefineDataStructure::Queue<int> queue2;
    queue2 = std::move(queue);
    EXPECT_EQ(queue2.size(), 3);
    EXPECT_EQ(queue2.front(), 1);
    EXPECT_EQ(queue2.back(), 3);
    EXPECT_TRUE(queue.empty());
}

TEST_F(QueueTest, Swap) {
    queue.push(1);
    queue.push(2);

    userDefineDataStructure::Queue<int> queue2;
    queue2.push(3);
    queue2.push(4);
    queue2.push(5);

    swap(queue, queue2);

    EXPECT_EQ(queue.size(), 3);
    EXPECT_EQ(queue.front(), 3);
    EXPECT_EQ(queue.back(), 5);

    EXPECT_EQ(queue2.size(), 2);
    EXPECT_EQ(queue2.front(), 1);
    EXPECT_EQ(queue2.back(), 2);
}

TEST_F(QueueTest, Exceptions) {
    EXPECT_THROW(queue.front(), std::runtime_error);
    EXPECT_THROW(queue.back(), std::runtime_error);
    EXPECT_THROW(queue.pop(), std::runtime_error);

    queue.push(1);
    queue.pop();

    EXPECT_THROW(queue.front(), std::runtime_error);
    EXPECT_THROW(queue.back(), std::runtime_error);
    EXPECT_THROW(queue.pop(), std::runtime_error);
}
