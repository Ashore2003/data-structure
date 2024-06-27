#include "list.h"
#include <gtest/gtest.h>

TEST(ListTest, PushBack) {
    userDefineDataStructure::List<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(list.back(), 3);
}

TEST(ListTest, PushFront) {
    userDefineDataStructure::List<int> list;
    list.push_front(1);
    list.push_front(2);
    list.push_front(3);

    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.front(), 3);
    EXPECT_EQ(list.back(), 1);
}

TEST(ListTest, PopBack) {
    userDefineDataStructure::List<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    list.pop_back();
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.back(), 2);

    list.pop_back();
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.back(), 1);

    list.pop_back();
    EXPECT_EQ(list.size(), 0);
}

TEST(ListTest, PopFront) {
    userDefineDataStructure::List<int> list;
    list.push_front(1);
    list.push_front(2);
    list.push_front(3);

    list.pop_front();
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.front(), 2);

    list.pop_front();
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.front(), 1);

    list.pop_front();
    EXPECT_EQ(list.size(), 0);
}

TEST(ListTest, Clear) {
    userDefineDataStructure::List<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    list.clear();
    EXPECT_EQ(list.size(), 0);
    EXPECT_TRUE(list.empty());
}

TEST(ListTest, CopyConstructor) {
    userDefineDataStructure::List<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    userDefineDataStructure::List copy = list;
    EXPECT_EQ(copy.size(), 3);
    EXPECT_EQ(copy.front(), 1);
    EXPECT_EQ(copy.back(), 3);
}

TEST(ListTest, AssignmentOperator) {
    userDefineDataStructure::List<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    userDefineDataStructure::List copy = list;
    EXPECT_EQ(copy.size(), 3);
    EXPECT_EQ(copy.front(), 1);
    EXPECT_EQ(copy.back(), 3);
}

TEST(ListTest, MoveConstructor) {
    userDefineDataStructure::List<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    userDefineDataStructure::List<int> moved = std::move(list);
    EXPECT_EQ(moved.size(), 3);
    EXPECT_EQ(moved.front(), 1);
    EXPECT_EQ(moved.back(), 3);
    EXPECT_EQ(list.size(), 0);
}

TEST(ListTest, MoveAssignmentOperator) {
    userDefineDataStructure::List<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    userDefineDataStructure::List<int> moved;
    moved = std::move(list);
    EXPECT_EQ(moved.size(), 3);
    EXPECT_EQ(moved.front(), 1);
    EXPECT_EQ(moved.back(), 3);
    EXPECT_EQ(list.size(), 0);
}

TEST(ListTest, Iterator) {
    userDefineDataStructure::List<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    auto it = list.begin();
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 2);
    ++it;
    EXPECT_EQ(*it, 3);
    ++it;
    EXPECT_EQ(it, list.end());
}

TEST(ListTest, ConstIterator) {
    userDefineDataStructure::List<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    auto it = list.cbegin();
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 2);
    ++it;
    EXPECT_EQ(*it, 3);
    ++it;
    EXPECT_EQ(it, list.cend());
}
