#include "vector.h"
#include <gtest/gtest.h>

class VectorTest : public ::testing::Test {
protected:
    userDefineDataStructure::vector<int> int_vec;
    userDefineDataStructure::vector<std::string> string_vec;
};


TEST_F(VectorTest, DefaultConstructor) {
    EXPECT_EQ(int_vec.size(), 0);
    EXPECT_TRUE(int_vec.empty());
}

TEST_F(VectorTest, SizeConstructor) {
    userDefineDataStructure::vector<int> vec(5);
    EXPECT_EQ(vec.size(), 5);
    for (int i = 0; i < 5; ++i)
        EXPECT_EQ(vec[i], 0);
}

TEST_F(VectorTest, SizeValueConstructor) {
    userDefineDataStructure::vector<int> vec(3, 42);
    EXPECT_EQ(vec.size(), 3);
    for (int i = 0; i < 3; ++i)
        EXPECT_EQ(vec[i], 42);
}

TEST_F(VectorTest, InitializerListConstructor) {
    userDefineDataStructure::vector<int> vec = {1, 2, 3, 4, 5};
    EXPECT_EQ(vec.size(), 5);
    for (int i = 0; i < 5; ++i)
        EXPECT_EQ(vec[i], i + 1);
}

TEST_F(VectorTest, InitializerListAssignment) {
    int_vec = {10, 20, 30, 40, 50};
    EXPECT_EQ(int_vec.size(), 5);
    for (int i = 0; i < 5; ++i)
        EXPECT_EQ(int_vec[i], (i + 1) * 10);
}

TEST_F(VectorTest, CopyAssignment) {
    userDefineDataStructure::vector<int> vec1(3, 42);
    userDefineDataStructure::vector<int> vec2;
    vec2 = vec1;
    EXPECT_EQ(vec2.size(), 3);
    for (int i = 0; i < 3; ++i)
        EXPECT_EQ(vec2[i], 42);
}

TEST_F(VectorTest, MoveAssignment) {
    userDefineDataStructure::vector<int> vec1(3, 42);
    userDefineDataStructure::vector<int> vec2;
    vec2 = std::move(vec1);
    EXPECT_EQ(vec2.size(), 3);
    for (int i = 0; i < 3; ++i)
        EXPECT_EQ(vec2[i], 42);
    EXPECT_TRUE(vec1.empty());
}

TEST_F(VectorTest, OperatorBracket) {
    int_vec = {1, 2, 3, 4, 5};
    EXPECT_EQ(int_vec[0], 1);
    EXPECT_EQ(int_vec[4], 5);
}

TEST_F(VectorTest, At) {
    int_vec = {1, 2, 3, 4, 5};
    EXPECT_EQ(int_vec.at(0), 1);
    EXPECT_EQ(int_vec.at(4), 5);
    EXPECT_THROW(int_vec.at(5), std::out_of_range);
}

TEST_F(VectorTest, FrontBack) {
    int_vec = {1, 2, 3, 4, 5};
    EXPECT_EQ(int_vec.front(), 1);
    EXPECT_EQ(int_vec.back(), 5);
}

TEST_F(VectorTest, Size) {
    EXPECT_EQ(int_vec.size(), 0);
    int_vec = {1, 2, 3, 4, 5};
    EXPECT_EQ(int_vec.size(), 5);
}

TEST_F(VectorTest, Empty) {
    EXPECT_TRUE(int_vec.empty());
    int_vec.push_back(1);
    EXPECT_FALSE(int_vec.empty());
}

TEST_F(VectorTest, Reserve) {
    int_vec.reserve(10);
    EXPECT_GE(int_vec.capacity(), 10);
    EXPECT_EQ(int_vec.size(), 0);
}

TEST_F(VectorTest, PushBack) {
    int_vec.push_back(1);
    EXPECT_EQ(int_vec.size(), 1);
    EXPECT_EQ(int_vec[0], 1);

    int_vec.push_back(2);
    EXPECT_EQ(int_vec.size(), 2);
    EXPECT_EQ(int_vec[1], 2);
}

TEST_F(VectorTest, PopBack) {
    int_vec = {1, 2, 3};
    int_vec.pop_back();
    EXPECT_EQ(int_vec.size(), 2);
    EXPECT_EQ(int_vec.back(), 2);
}

TEST_F(VectorTest, Clear) {
    int_vec = {1, 2, 3, 4, 5};
    int_vec.clear();
    EXPECT_TRUE(int_vec.empty());
    EXPECT_EQ(int_vec.size(), 0);
}

TEST_F(VectorTest, Resize) {
    int_vec = {1, 2, 3};
    int_vec.resize(5);
    EXPECT_EQ(int_vec.size(), 5);
    EXPECT_EQ(int_vec[2], 3);
    EXPECT_EQ(int_vec[3], 0);
    EXPECT_EQ(int_vec[4], 0);

    int_vec.resize(2);
    EXPECT_EQ(int_vec.size(), 2);
    EXPECT_EQ(int_vec[1], 2);
}

TEST_F(VectorTest, ResizeWithValue) {
    int_vec = {1, 2, 3};
    int_vec.resize(5, 42);
    EXPECT_EQ(int_vec.size(), 5);
    EXPECT_EQ(int_vec[2], 3);
    EXPECT_EQ(int_vec[3], 42);
    EXPECT_EQ(int_vec[4], 42);
}

TEST_F(VectorTest, Iterators) {
    int_vec = {1, 2, 3, 4, 5};
    int sum = 0;
    for (auto it = int_vec.begin(); it != int_vec.end(); ++it)
        sum += *it;
    EXPECT_EQ(sum, 15);
}

TEST_F(VectorTest, StringVector) {
    string_vec.push_back("Hello");
    string_vec.push_back("World");
    EXPECT_EQ(string_vec.size(), 2);
    EXPECT_EQ(string_vec[0], "Hello");
    EXPECT_EQ(string_vec[1], "World");
}

TEST_F(VectorTest, ExceptionSafety) {
    struct ThrowOnCopy {
        ThrowOnCopy() = default;
        ThrowOnCopy(const ThrowOnCopy &) { throw std::runtime_error("Copy error"); }
        ThrowOnCopy &operator=(const ThrowOnCopy &) = default;
    };

    userDefineDataStructure::vector<ThrowOnCopy> vec(5);
    EXPECT_EQ(vec.size(), 5);

    EXPECT_THROW({ vec.push_back(ThrowOnCopy()); }, std::runtime_error);

    EXPECT_EQ(vec.size(), 5);
}

TEST_F(VectorTest, LargeNumberOfElements) {
    const size_t large_size = 1000000;
    int_vec.reserve(large_size);
    for (size_t i = 0; i < large_size; ++i)
        int_vec.push_back(static_cast<int>(i));
    EXPECT_EQ(int_vec.size(), large_size);
    EXPECT_EQ(int_vec[large_size - 1], static_cast<int>(large_size - 1));
}