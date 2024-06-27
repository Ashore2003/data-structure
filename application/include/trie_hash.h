
#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @class userDefineDataStructure::TrieHash
 * 
 * @brief A Trie (prefix tree) implementation using hash maps for efficient string operations.
 * 
 * This class provides a Trie data structure implementation using hash maps for its internal
 * representation. It offers efficient operations for inserting, searching, and deleting strings,
 * as well as prefix-based word prediction. The use of hash maps allows for faster child node
 * lookups compared to traditional array-based Trie implementations.
 * 
 * Key features:
 * - Efficient insertion and search operations, typically O(k) where k is the length of the string.
 * - Prefix-based word prediction functionality.
 * - Memory-efficient storage of strings with common prefixes.
 * - Supports deletion of words while maintaining the integrity of the Trie.
 * - Provides methods to print all stored words and check for words with a given prefix.
 * 
 * Usage example:
 * @code
 * userDefineDataStructure::TrieHash trie;
 * trie.insert("apple");
 * trie.insert("app");
 * trie.insert("application");
 * 
 * std::cout << trie.search("apple") << std::endl;  // Output: 1 (true)
 * std::cout << trie.startWith("app") << std::endl;  // Output: 1 (true)
 * 
 * auto predictions = trie.predictWords("app");
 * for (const auto& word : predictions) {
 *     std::cout << word << " ";
 * }
 * // Output: app apple application
 * 
 * trie.deleteWord("apple");
 * std::cout << trie.search("apple") << std::endl;  // Output: 0 (false)
 * @endcode
 * 
 * @note This implementation uses smart pointers (std::unique_ptr) for automatic memory management.
 * 
 * @warning This class is not thread-safe. External synchronization is required for concurrent access.
 */
namespace userDefineDataStructure {
    /**
    * @class TrieHash
    * @brief A Trie data structure that uses hash maps for its implementation.
    */
    class TrieHash {
    private:
        /**
        * @struct Node
        * @brief A node in the Trie.
        */
        struct Node {
            /// Hash map storing pointers to child nodes, each character corresponds to a node
            std::unordered_map<char, std::unique_ptr<Node>> children_;
            /// Boolean flag indicating if this node marks the end of a word
            bool word_end_ = false;
        };

        /// Root node of the Trie, does not contain a character but points to nodes of all starting characters
        std::unique_ptr<Node> root_node_ = std::make_unique<Node>();

        /**
        * @brief Helper function to collect all words from a given node.
        * @param results Vector to store the collected words.
        * @param element The current node being traversed.
        * @param prefix The current prefix formed during traversal.
        * @return Vector of all words collected.
        */
        std::vector<std::string> getAllWords(std::vector<std::string> &results,
                                             const Node *element,
                                             std::string &prefix) const {
            if (element->word_end_)
                results.push_back(prefix);
            for (const auto &child: element->children_) {
                prefix.push_back(child.first);
                getAllWords(results, child.second.get(), prefix);
                prefix.pop_back();
            }
            return results;
        }

        /**
        * @brief Helper function to delete a word recursively.
        * @param word The word to be deleted.
        * @param node The current node being traversed.
        * @param depth The current depth in the Trie.
        * @return True if the child node should be deleted, false otherwise.
        */
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
        /**
        * @brief Default constructor for TrieHash.
        */
        TrieHash() = default;

        /**
        * @brief Default destructor for TrieHash.
        */
        ~TrieHash() = default;

        /**
        * @brief Deleted copy constructor to prevent copying.
        */
        TrieHash(const TrieHash &) = delete;

        /**
        * @brief Deleted copy assignment operator to prevent copying.
        * @return Reference to the assigned object.
        */
        TrieHash &operator=(const TrieHash &) = delete;

        /**
        * @brief Insert a word into the Trie.
        * @param word The word to be inserted.
        */
        void insert(const std::string &word) {
            Node *curr = root_node_.get();
            for (char ch: word) {
                if (curr->children_.find(ch) == curr->children_.end())
                    curr->children_[ch] = std::make_unique<Node>();
                curr = curr->children_[ch].get();
            }
            curr->word_end_ = true;
        }

        /**
        * @brief Check if a word exists in the Trie.
        * @param word The word to be searched.
        * @return True if the word exists, false otherwise.
        */
        [[nodiscard]] bool search(const std::string &word) const {
            const Node *curr = root_node_.get();
            for (char ch: word) {
                auto it = curr->children_.find(ch);
                if (it == curr->children_.end())
                    return false;
                curr = it->second.get();
            }
            return curr->word_end_;
        }

        /**
        * @brief Check if any word starts with a given prefix.
        * @param prefix The prefix to be checked.
        * @return True if there is any word with the given prefix, false otherwise.
        */
        [[nodiscard]] bool startWith(const std::string &prefix) const {
            const Node *curr = root_node_.get();
            for (char ch: prefix) {
                auto it = curr->children_.find(ch);
                if (it == curr->children_.end())
                    return false;
                curr = it->second.get();
            }
            return true;
        }

        /**
        * @brief Delete a word from the Trie.
        * @param word The word to be deleted.
        * @return True if the word was successfully deleted, false otherwise.
        */
        bool deleteWord(const std::string &word) {
            return deleteWordHelper(word, root_node_.get(), 0);
        }

        /**
        * @brief Predict words based on a given prefix.
        * @param prefix The prefix used for prediction.
        * @return Vector of words that match the given prefix.
        */
        [[nodiscard]] std::vector<std::string> predictWords(const std::string &prefix) const {
            const Node *curr = root_node_.get();
            for (char ch: prefix) {
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

        /**
        * @brief Print all words stored in the Trie.
        */
        void printAllWords() const {
            std::vector<std::string> words;
            std::string prefix;
            getAllWords(words, root_node_.get(), prefix);
            for (const std::string &word: words)
                std::cout << word << std::endl;
        }
    };

}// namespace userDefineDataStructure
