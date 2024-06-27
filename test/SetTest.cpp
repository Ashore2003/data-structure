#include "set.h"
#include <algorithm>
#include <gtest/gtest.h>
#include <vector>

class SetTest : public ::testing::Test {
protected:
    userDefineDataStructure::set<int> intSet;
    userDefineDataStructure::set<std::string> stringSet;
};

TEST_F(SetTest, DefaultConstructor) {
    EXPECT_TRUE(intSet.empty());
    EXPECT_EQ(intSet.size(), 0);
}

TEST_F(SetTest, InsertAndSize) {
    EXPECT_TRUE(intSet.insert(1).second);
    EXPECT_EQ(intSet.size(), 1);
    EXPECT_TRUE(intSet.insert(2).second);
    EXPECT_EQ(intSet.size(), 2);
    EXPECT_FALSE(intSet.insert(1).second);
    EXPECT_EQ(intSet.size(), 2);
}

TEST_F(SetTest, Find) {
    intSet.insert(1);
    intSet.insert(2);
    intSet.insert(3);

    EXPECT_NE(intSet.find(1), intSet.end());
    EXPECT_NE(intSet.find(2), intSet.end());
    EXPECT_NE(intSet.find(3), intSet.end());
    EXPECT_EQ(intSet.find(4), intSet.end());
}

TEST_F(SetTest, Erase) {
    intSet.insert(1);
    intSet.insert(2);
    intSet.insert(3);

    EXPECT_EQ(intSet.erase(2), 1);
    EXPECT_EQ(intSet.size(), 2);
    EXPECT_EQ(intSet.find(2), intSet.end());

    EXPECT_EQ(intSet.erase(4), 0);
    EXPECT_EQ(intSet.size(), 2);
}

TEST_F(SetTest, Clear) {
    intSet.insert(1);
    intSet.insert(2);
    intSet.insert(3);

    intSet.clear();
    EXPECT_TRUE(intSet.empty());
    EXPECT_EQ(intSet.size(), 0);
}

TEST_F(SetTest, Iterator) {
    std::vector<int> values = {1, 3, 5, 7, 9};
    for (int val: values)
        intSet.insert(val);

    std::vector<int> iteratedValues;
    for (const auto &val: intSet)
        iteratedValues.push_back(val);

    EXPECT_EQ(values, iteratedValues);
}

TEST_F(SetTest, ReverseIterator) {
    std::vector<int> values = {1, 3, 5, 7, 9};
    for (int val: values)
        intSet.insert(val);

    std::vector<int> reverseIteratedValues;
    for (auto it = intSet.rbegin(); it != intSet.rend(); ++it)
        reverseIteratedValues.push_back(*it);

    std::reverse(values.begin(), values.end());
    EXPECT_EQ(values, reverseIteratedValues);
}

TEST_F(SetTest, CopyConstructor) {
    intSet.insert(1);
    intSet.insert(2);
    intSet.insert(3);

    userDefineDataStructure::set<int> copiedSet(intSet);

    EXPECT_EQ(intSet.size(), copiedSet.size());
    for (const auto &val: intSet)
        EXPECT_NE(copiedSet.find(val), copiedSet.end());
}

TEST_F(SetTest, AssignmentOperator) {
    intSet.insert(1);
    intSet.insert(2);
    intSet.insert(3);

    userDefineDataStructure::set<int> assignedSet;
    assignedSet = intSet;

    EXPECT_EQ(intSet.size(), assignedSet.size());
    for (const auto &val: intSet)
        EXPECT_NE(assignedSet.find(val), assignedSet.end());
}

TEST_F(SetTest, EmptyOperations) {
    EXPECT_EQ(intSet.erase(1), 0);
    EXPECT_EQ(intSet.find(1), intSet.end());
    EXPECT_EQ(intSet.begin(), intSet.end());
}

TEST_F(SetTest, LargeDataset) {
    const int N = 10000;
    for (int i = 0; i < N; ++i)
        EXPECT_TRUE(intSet.insert(i).second);

    EXPECT_EQ(intSet.size(), N);

    for (int i = 0; i < N; ++i)
        EXPECT_NE(intSet.find(i), intSet.end());

    for (int i = 0; i < N; i += 2)
        EXPECT_EQ(intSet.erase(i), 1);

    EXPECT_EQ(intSet.size(), N / 2);

    for (int i = 0; i < N; ++i) {
        if (i % 2 == 0)
            EXPECT_EQ(intSet.find(i), intSet.end());
        else
            EXPECT_NE(intSet.find(i), intSet.end());
    }
}

TEST_F(SetTest, StringSet) {
    stringSet.insert("apple");
    stringSet.insert("banana");
    stringSet.insert("cherry");

    EXPECT_EQ(stringSet.size(), 3);
    EXPECT_NE(stringSet.find("banana"), stringSet.end());
    EXPECT_EQ(stringSet.find("grape"), stringSet.end());

    EXPECT_EQ(stringSet.erase("banana"), 1);
    EXPECT_EQ(stringSet.size(), 2);
    EXPECT_EQ(stringSet.find("banana"), stringSet.end());
}

TEST_F(SetTest, CustomComparator) {
    struct ReverseCompare {
        bool operator()(const int &a, const int &b) const {
            return a > b;
        }
    };

    userDefineDataStructure::set<int, ReverseCompare> reverseSet;
    reverseSet.insert(1);
    reverseSet.insert(3);
    reverseSet.insert(2);

    std::vector<int> values;
    for (const auto &val: reverseSet)
        values.push_back(val);

    std::vector<int> expected = {3, 2, 1};
    EXPECT_EQ(values, expected);
}
