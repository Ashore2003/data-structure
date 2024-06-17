#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class TrieHash {
private:
  struct Node {
    // Store pointers to child nodes, each character corresponds to a node
    std::unordered_map<char, std::unique_ptr<Node>> children_;
    // Mark if this node is the end of a word
    bool word_end_ = false;
  };

  // Root node, does not contain a character but points to nodes of all starting
  // characters
  std::unique_ptr<Node> root_node_ = std::make_unique<Node>();

  // Helper function to collect all words from a given node
  std::vector<std::string> getAllWords(std::vector<std::string> &results,
                                       const Node *element,
                                       std::string &prefix) const {
    if (element->word_end_) {
      results.push_back(prefix);
    }
    for (const auto &child : element->children_) {
      prefix.push_back(child.first);
      getAllWords(results, child.second.get(), prefix);
      prefix.pop_back();
    }
    return results;
  }

  // Helper function to delete a word recursively
  bool deleteWordHelper(const std::string &word, Node *node, size_t depth) {
    if (depth == word.size()) {
      if (!node->word_end_)
        return false;
      node->word_end_ = false;
      return node->children_.empty();
    }
    char ch = word[depth];
    auto it = node->children_.find(ch);
    if (it == node->children_.end())
      return false;
    bool should_delete_child =
        deleteWordHelper(word, it->second.get(), depth + 1);
    if (should_delete_child) {
      node->children_.erase(ch);
      return node->children_.empty() && !node->word_end_;
    }
    return false;
  }

public:
  TrieHash() = default;
  ~TrieHash() = default;
  TrieHash(const TrieHash &) = delete;
  TrieHash &operator=(const TrieHash &) = delete;

  // Insert a word into the trie
  void insert(const std::string &word) {
    Node *curr = root_node_.get();
    for (char ch : word) {
      if (curr->children_.find(ch) == curr->children_.end()) {
        curr->children_[ch] = std::make_unique<Node>();
      }
      curr = curr->children_[ch].get();
    }
    curr->word_end_ = true;
  }

  // Check if a word exists in the trie
  [[nodiscard]] bool search(const std::string &word) const {
    const Node *curr = root_node_.get();
    for (char ch : word) {
      auto it = curr->children_.find(ch);
      if (it == curr->children_.end())
        return false;
      curr = it->second.get();
    }
    return curr->word_end_;
  }

  // Check if any word starts with a given prefix
  [[nodiscard]] bool startWith(const std::string &prefix) const {
    const Node *curr = root_node_.get();
    for (char ch : prefix) {
      auto it = curr->children_.find(ch);
      if (it == curr->children_.end())
        return false;
      curr = it->second.get();
    }
    return true;
  }

  // Delete a word from the trie
  bool deleteWord(const std::string &word) {
    return deleteWordHelper(word, root_node_.get(), 0);
  }

  // Predict words based on a given prefix
  [[nodiscard]] std::vector<std::string>
  predictWords(const std::string &prefix) const {
    const Node *curr = root_node_.get();
    for (char ch : prefix) {
      auto it = curr->children_.find(ch);
      if (it == curr->children_.end())
        return {};
      curr = it->second.get();
    }
    std::vector<std::string> result;
    std::string new_prefix = prefix;
    getAllWords(result, curr, new_prefix);
    return result;
  }

  // Print all words in the trie
  void printAllWords() const {
    std::vector<std::string> words;
    std::string prefix;
    getAllWords(words, root_node_.get(), prefix);
    for (const std::string &word : words) {
      std::cout << word << std::endl;
    }
  }
};
