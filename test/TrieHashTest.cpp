#include "trie_hash.h"
#include <gtest/gtest.h>

class TrieHashTest : public ::testing::Test {
protected:
  TrieHash trie;

  void SetUp() override {
    trie.insert("hello");
    trie.insert("hell");
    trie.insert("help");
  }
};

TEST_F(TrieHashTest, InsertAndSearch) {
  EXPECT_TRUE(trie.search("hello"));
  EXPECT_TRUE(trie.search("hell"));
  EXPECT_FALSE(trie.search("world"));
}

TEST_F(TrieHashTest, DeleteWord) {
  trie.deleteWord("hello");
  EXPECT_FALSE(trie.search("hello"));
  EXPECT_TRUE(trie.search("hell"));

  // Deleting a non-existing word should not affect the trie.
  EXPECT_FALSE(trie.deleteWord("nonexisting"));
  EXPECT_TRUE(trie.search("hell"));
}

TEST_F(TrieHashTest, PredictWords) {
  auto predictions = trie.predictWords("hel");
  EXPECT_EQ(predictions.size(), 3);
  EXPECT_TRUE(std::find(predictions.begin(), predictions.end(), "hello") !=
              predictions.end());
  EXPECT_TRUE(std::find(predictions.begin(), predictions.end(), "help") !=
              predictions.end());

  trie.deleteWord("help");
  predictions = trie.predictWords("hel");
  EXPECT_EQ(predictions.size(), 2);
}

TEST_F(TrieHashTest, StartsWith) {
  EXPECT_TRUE(trie.startWith("hel"));
  EXPECT_FALSE(trie.startWith("hex"));
}
