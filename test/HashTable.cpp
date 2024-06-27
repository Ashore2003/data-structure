#include "hash_table.h"
#include <gtest/gtest.h>
#include <random>
#include <string>
#include <vector>

namespace {

    class HashMapTest : public ::testing::Test {
    protected:
        userDefineDataStructure::HashMap<std::string, int> map;

        void SetUp() override {
        }

        void TearDown() override {
        }
    };

    TEST_F(HashMapTest, InsertAndRetrieve) {
        map.insert_or_assign("key1", 100);
        EXPECT_EQ(map.at("key1"), 100);
        EXPECT_TRUE(map.contains("key1"));
    }

    TEST_F(HashMapTest, UpdateExistingKey) {
        map.insert_or_assign("key1", 100);
        map.insert_or_assign("key1", 200);
        EXPECT_EQ(map.at("key1"), 200);
    }

    TEST_F(HashMapTest, EraseElement) {
        map.insert_or_assign("key1", 100);
        EXPECT_TRUE(map.erase("key1"));
        EXPECT_FALSE(map.contains("key1"));
        EXPECT_FALSE(map.erase("key1"));
    }

    TEST_F(HashMapTest, EmptyMapOperations) {
        EXPECT_TRUE(map.empty());
        EXPECT_EQ(map.size(), 0);
        EXPECT_FALSE(map.contains("anykey"));
        EXPECT_FALSE(map.erase("anykey"));
    }

    TEST_F(HashMapTest, LargeNumberOfInsertions) {
        const int NUM_INSERTS = 10000;
        for (int i = 0; i < NUM_INSERTS; ++i) {
            map.insert_or_assign("key" + std::to_string(i), i);
        }
        EXPECT_EQ(map.size(), NUM_INSERTS);
        for (int i = 0; i < NUM_INSERTS; ++i) {
            EXPECT_TRUE(map.contains("key" + std::to_string(i)));
        }
    }

    TEST_F(HashMapTest, IteratorTest) {
        std::vector<std::pair<std::string, int>> expected = {
                {"key1", 100},
                {"key2", 200},
                {"key3", 300}};
        for (const auto &pair: expected) {
            map.insert_or_assign(pair.first, pair.second);
        }

        std::vector<std::pair<std::string, int>> actual;
        for (const auto &pair: map) {
            actual.push_back(pair);
        }

        EXPECT_EQ(actual.size(), expected.size());
        for (const auto &pair: expected) {
            EXPECT_TRUE(std::find(actual.begin(), actual.end(), pair) != actual.end());
        }
    }

    TEST_F(HashMapTest, RehashAndLoadFactor) {
        const int INITIAL_BUCKET_COUNT = 16;
        userDefineDataStructure::HashMap<int, int> intMap(INITIAL_BUCKET_COUNT);

        float initial_load_factor = intMap.load_factor();
        EXPECT_FLOAT_EQ(initial_load_factor, 0.0f);

        const int NUM_INSERTS = 100;
        for (int i = 0; i < NUM_INSERTS; ++i) {
            intMap.insert_or_assign(i, i);
        }

        EXPECT_GT(intMap.bucket_count(), INITIAL_BUCKET_COUNT);
        EXPECT_LE(intMap.load_factor(), intMap.max_load_factor());
    }

    TEST_F(HashMapTest, ExceptionHandling) {
        EXPECT_THROW(map.at("nonexistent"), std::out_of_range);

        EXPECT_THROW(map.max_load_factor(0.0f), std::invalid_argument);
        EXPECT_THROW(map.max_load_factor(-1.0f), std::invalid_argument);
    }

    TEST_F(HashMapTest, PerformanceTest) {
        const int NUM_OPERATIONS = 100000;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, NUM_OPERATIONS - 1);

        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < NUM_OPERATIONS; ++i) {
            map.insert_or_assign(std::to_string(i), i);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto insert_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < NUM_OPERATIONS; ++i) {
            int key = dis(gen);
            map.contains(std::to_string(key));
        }
        end = std::chrono::high_resolution_clock::now();
        auto lookup_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::cout << "Insert " << NUM_OPERATIONS << " elements: " << insert_duration.count() << "ms" << std::endl;
        std::cout << "Lookup " << NUM_OPERATIONS << " times: " << lookup_duration.count() << "ms" << std::endl;

        EXPECT_LT(insert_duration.count(), 5000);
        EXPECT_LT(lookup_duration.count(), 5000);
    }

    TEST_F(HashMapTest, ClearTest) {
        for (int i = 0; i < 100; ++i) {
            map.insert_or_assign(std::to_string(i), i);
        }
        EXPECT_FALSE(map.empty());
        map.clear();
        EXPECT_TRUE(map.empty());
        EXPECT_EQ(map.size(), 0);
    }

    struct ComplexKey {
        int a;
        std::string b;

        bool operator==(const ComplexKey &other) const {
            return a == other.a && b == other.b;
        }
    };

    struct ComplexKeyHash {
        std::size_t operator()(const ComplexKey &key) const {
            return std::hash<int>()(key.a) ^ std::hash<std::string>()(key.b);
        }
    };

    TEST_F(HashMapTest, ComplexKeyTest) {
        userDefineDataStructure::HashMap<ComplexKey, int, ComplexKeyHash> complexMap;

        ComplexKey key1{1, "one"};
        ComplexKey key2{2, "two"};

        complexMap.insert_or_assign(key1, 100);
        complexMap.insert_or_assign(key2, 200);

        EXPECT_EQ(complexMap.at(key1), 100);
        EXPECT_EQ(complexMap.at(key2), 200);
        EXPECT_TRUE(complexMap.contains(key1));
        EXPECT_TRUE(complexMap.contains(key2));
    }

}// namespace