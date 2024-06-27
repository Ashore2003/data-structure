#pragma once

#include "list.h"
#include "vector.h"
#include <cmath>
#include <functional>
#include <stdexcept>
#include <utility>

/**
 * @class HashMap
 * @brief A hash table implementation with an STL-like interface.
 *
 * This class provides a generic hash table implementation with support for
 * standard map operations such as insertion, deletion, and lookup. It uses
 * separate chaining for collision resolution and provides iteration capabilities.
 *
 * @tparam Key The type of keys stored in the hash map.
 * @tparam Value The type of mapped values.
 * @tparam Hash The hash function type, defaults to std::hash<Key>.
 *
 * Key features:
 * - Amortized constant time complexity for insert, delete, and search operations
 * - Automatic resizing to maintain load factor
 * - Custom hash function support
 * - Iterator support for traversing all elements
 *
 * Usage example:
 * @code
 * userDefineDataStructure::HashMap<std::string, int> myMap;
 * myMap.insert_or_assign("one", 1);
 * myMap.insert_or_assign("two", 2);
 *
 * for(const auto& pair : myMap) {
 *     std::cout << pair.first << ": " << pair.second << std::endl;
 * }
 * @endcode
 *
 * @warning This class is not thread-safe. External synchronization is required
 *          for concurrent access.
 */

namespace userDefineDataStructure {
    template<typename Key, typename Value, typename Hash = std::hash<Key>>
    class HashMap {
    private:
        using Bucket = List<std::pair<const Key, Value>>;///< Type alias for a bucket (linked list of key-value pairs)
        using BucketVector = vector<Bucket>;             ///< Type alias for the vector of buckets

        BucketVector buckets;  ///< Vector of buckets for separate chaining
        size_t size_;          ///< Current number of elements in the hash map
        float max_load_factor_;///< Maximum load factor before rehashing
        Hash hasher;           ///< Hash function object

        /**
        * @brief Finds an element with the specified key in a bucket.
        *
        * @param bucket The bucket to search in.
        * @param key The key to search for.
        * @return typename Bucket::iterator Iterator to the found element, or end iterator if not found.
        */
        typename Bucket::iterator find_in_bucket(Bucket &bucket, const Key &key) {
            return std::find_if(bucket.begin(), bucket.end(),
                                [&key](const auto &pair) { return pair.first == key; });
        }

        /**
        * @brief Finds an element with the specified key in a bucket (const version).
        *
        * @param bucket The bucket to search in.
        * @param key The key to search for.
        * @return typename Bucket::const_iterator Const iterator to the found element, or end iterator if not found.
        */
        typename Bucket::const_iterator find_in_bucket(const Bucket &bucket, const Key &key) const {
            return std::find_if(bucket.begin(), bucket.end(),
                                [&key](const auto &pair) { return pair.first == key; });
        }

        /**
        * @brief Calculates the bucket index for a given key.
        *
        * @param key The key to hash.
        * @return size_t The calculated bucket index.
        */
        size_t bucket_index(const Key &key) const {
            return hasher(key) % buckets.size();
        }

        /**
        * @brief Checks if rehashing is needed and performs it if necessary.
        */
        void check_for_rehash() {
            if (load_factor() > max_load_factor_)
                rehash(buckets.size() * 2);
        }

    public:
        /**
        * @brief Constructs a new HashMap object.
        *
        * @param initial_bucket_count Initial number of buckets (default is 16).
        * @param hash Hash function object (default is Hash()).
        */
        explicit HashMap(size_t initial_bucket_count = 16, const Hash &hash = Hash())
            : buckets(initial_bucket_count), size_(0), max_load_factor_(0.75f), hasher(hash) {}

        /**
        * @brief Inserts a new element or assigns to an existing element.
        *
        * If the key does not exist, a new element is inserted. If the key exists,
        * the corresponding value is updated.
        *
        * @param key The key of the element to insert or assign.
        * @param value The value to be inserted or assigned.
        *
        * Time Complexity: Amortized O(1) on average, O(n) worst case when rehashing.
        */
        void insert_or_assign(const Key &key, const Value &value) {
            check_for_rehash();
            size_t index = bucket_index(key);
            auto it = find_in_bucket(buckets[index], key);
            if (it == buckets[index].end()) {
                buckets[index].push_back(std::make_pair(key, value));
                ++size_;
            } else {
                auto &pair = const_cast<std::pair<const Key, Value> &>(*it);
                pair.second = value;
            }
        }

        /**
        * @brief Accesses or inserts an element.
        *
        * Returns a reference to the value that is mapped to the key.
        * If the key does not exist, a new element with that key is inserted.
        *
        * @param key The key of the element to access or insert.
        * @return Value& Reference to the mapped value.
        *
        * Time Complexity: Amortized O(1) on average.
        */
        Value &operator[](const Key &key) {
            check_for_rehash();
            size_t index = bucket_index(key);
            auto it = find_in_bucket(buckets[index], key);
            if (it == buckets[index].end()) {
                auto [it, inserted] = buckets[index].push_back(std::make_pair(key, Value()));
                ++size_;
                return it->second;
            }
            return it->second;
        }

        /**
        * @brief Accesses an element (const version).
        *
        * Returns a const reference to the value that is mapped to the key.
        * If the key does not exist, throws std::out_of_range.
        *
        * @param key The key of the element to access.
        * @return const Value& Const reference to the mapped value.
        * @throw std::out_of_range if the key is not found.
        *
        * Time Complexity: O(1) on average.
        */
        const Value &at(const Key &key) const {
            size_t index = bucket_index(key);
            auto it = find_in_bucket(buckets[index], key);
            if (it == buckets[index].end())
                throw std::out_of_range("Key not found in HashMap");
            return it->second;
        }

        /**
        * @brief Accesses an element.
        *
        * Returns a reference to the value that is mapped to the key.
        * If the key does not exist, throws std::out_of_range.
        *
        * @param key The key of the element to access.
        * @return Value& Reference to the mapped value.
        * @throw std::out_of_range if the key is not found.
        *
        * Time Complexity: O(1) on average.
        */
        Value &at(const Key &key) {
            return const_cast<Value &>(static_cast<const HashMap *>(this)->at(key));
        }

        /**
        * @brief Checks if the container contains an element with the specified key.
        *
        * @param key The key to search for.
        * @return true If an element with the key exists.
        * @return false If no element with the key exists.
        *
        * Time Complexity: O(1) on average.
        */
        bool contains(const Key &key) const {
            size_t index = bucket_index(key);
            return find_in_bucket(buckets[index], key) != buckets[index].end();
        }

        /**
        * @brief Removes an element with the specified key.
        *
        * @param key The key of the element to remove.
        * @return true If an element was found and removed.
        * @return false If no element with the specified key existed.
        *
        * Time Complexity: O(1) on average.
        */
        bool erase(const Key &key) {
            size_t index = bucket_index(key);
            auto &bucket = buckets[index];
            auto it = find_in_bucket(bucket, key);
            if (it != bucket.end()) {
                bucket.remove(*it);
                --size_;
                return true;
            }
            return false;
        }

        /**
        * @brief Removes all elements from the container.
        *
        * Time Complexity: O(n), where n is the number of elements.
        */
        void clear() {
            for (auto &bucket: buckets)
                bucket.clear();
            size_ = 0;
        }

        /**
        * @brief Returns the number of elements in the container.
        *
        * @return size_t The number of elements.
        *
        * Time Complexity: O(1)
        */
        size_t size() const { return size_; }

        /**
        * @brief Checks if the container is empty.
        *
        * @return true If the container is empty.
        * @return false If the container is not empty.
        *
        * Time Complexity: O(1)
        */
        bool empty() const { return size_ == 0; }

        /**
        * @brief Returns the number of buckets in the container.
        *
        * @return size_t The number of buckets.
        *
        * Time Complexity: O(1)
        */
        size_t bucket_count() const { return buckets.size(); }

        /**
        * @brief Returns the current load factor of the container.
        *
        * The load factor is the ratio between the number of elements and the number of buckets.
        *
        * @return float The current load factor.
        *
        * Time Complexity: O(1)
        */
        float load_factor() const {
            return static_cast<float>(size_) / bucket_count();
        }

        /**
        * @brief Sets the maximum load factor for the container.
        *
        * If the new load factor is smaller than the current load factor,
        * a rehash is automatically performed.
        *
        * @param mlf The new maximum load factor.
        * @throw std::invalid_argument if mlf is not positive.
        *
        * Time Complexity: O(n) if rehashing occurs, O(1) otherwise.
        */
        void max_load_factor(float mlf) {
            if (mlf <= 0.f) throw std::invalid_argument("Load factor must be positive");
            max_load_factor_ = mlf;
            check_for_rehash();
        }

        /**
        * @brief Returns the current maximum load factor.
        *
        * @return float The current maximum load factor.
        *
        * Time Complexity: O(1)
        */
        float max_load_factor() const { return max_load_factor_; }

        /**
        * @brief Changes the number of buckets and rehashes all elements.
        *
        * @param new_bucket_count The new number of buckets.
        *
        * Time Complexity: O(n), where n is the number of elements.
        */
        void rehash(size_t new_bucket_count) {
            if (new_bucket_count < size_ / max_load_factor_)
                new_bucket_count = static_cast<size_t>(std::ceil(size_ / max_load_factor_));

            BucketVector new_buckets(new_bucket_count);

            for (const auto &bucket: buckets) {
                for (const auto &pair: bucket) {
                    size_t new_index = hasher(pair.first) % new_bucket_count;
                    new_buckets[new_index].push_back(pair);
                }
            }

            buckets = std::move(new_buckets);
        }

        /**
        * @brief Iterator class for HashMap.
        *
        * Provides a way to iterate over all elements in the HashMap.
        */
        class iterator {
        private:
            HashMap *map;
            size_t bucket_index;
            typename Bucket::iterator bucket_it;

            /**
            * @brief Finds the next valid element in the HashMap.
            */
            void find_next_valid() {
                while (bucket_index < map->bucket_count() &&
                       bucket_it == map->buckets[bucket_index].end()) {
                    ++bucket_index;
                    if (bucket_index < map->bucket_count())
                        bucket_it = map->buckets[bucket_index].begin();
                }
            }

        public:
            /**
            * @brief Constructs an iterator.
            *
            * @param m Pointer to the HashMap.
            * @param bi Current bucket index.
            * @param it Iterator within the current bucket.
            */
            iterator(HashMap *m, size_t bi, typename Bucket::iterator it)
                : map(m), bucket_index(bi), bucket_it(it) {
                find_next_valid();
            }

            /**
            * @brief Dereference operator.
            *
            * @return std::pair<const Key, Value>& Reference to the current key-value pair.
            */
            std::pair<const Key, Value> &operator*() { return *bucket_it; }

            /**
            * @brief Arrow operator.
            *
            * @return std::pair<const Key, Value>* Pointer to the current key-value pair.
            */
            std::pair<const Key, Value> *operator->() { return &(*bucket_it); }

            /**
            * @brief Prefix increment operator.
            *
            * @return iterator& Reference to the incremented iterator.
            */
            iterator &operator++() {
                ++bucket_it;
                find_next_valid();
                return *this;
            }

            /**
            * @brief Equality comparison operator.
            *
            * @param other The iterator to compare with.
            * @return true If the iterators are equal.
            * @return false If the iterators are not equal.
            */
            bool operator==(const iterator &other) const {
                return map == other.map && bucket_index == other.bucket_index &&
                       (bucket_index == map->bucket_count() || bucket_it == other.bucket_it);
            }

            /**
            * @brief Inequality comparison operator.
            *
            * @param other The iterator to compare with.
            * @return true If the iterators are not equal.
            * @return false If the iterators are equal.
            */
            bool operator!=(const iterator &other) const {
                return !(*this == other);
            }
        };

        /**
        * @brief Returns an iterator to the beginning of the HashMap.
        *
        * @return iterator Iterator pointing to the first element.
        *
        * Time Complexity: O(n) in the worst case, where n is the number of buckets.
        */
        iterator begin() {
            for (size_t i = 0; i < bucket_count(); ++i) {
                if (!buckets[i].empty())
                    return iterator(this, i, buckets[i].begin());
            }
            return end();
        }

        /**
        * @brief Returns an iterator to the end of the HashMap.
        *
        * @return iterator Iterator pointing to the position one past the last element.
        *
        * Time Complexity: O(1)
        */
        iterator end() {
            return iterator(this, bucket_count(), typename Bucket::iterator(nullptr));
        }

        /**
        * @brief Returns the bucket index for a key.
        *
        * @param key The key to get the bucket index for.
        * @return size_t The index of the bucket that would contain the key.
        *
        * Time Complexity: O(1)
        */
        size_t bucket(const Key &key) const {
            return bucket_index(key);
        }

        /**
        * @brief Returns the number of elements in a specific bucket.
        *
        * @param n The index of the bucket.
        * @return size_t The number of elements in the bucket.
        * @throw std::out_of_range if n is not a valid bucket index.
        *
        * Time Complexity: O(1)
        */
        size_t bucket_size(size_t n) const {
            if (n >= bucket_count())
                throw std::out_of_range("Invalid bucket index");
            return buckets[n].size();
        }

        /**
        * @brief Returns the hash function object used by the container.
        *
        * @return Hash The hash function object.
        *
        * Time Complexity: O(1)
        */
        Hash hash_function() const {
            return hasher;
        }

        /**
        * @brief Reserves space for at least the specified number of elements.
        *
        * @param count Minimum capacity to reserve.
        *
        * Time Complexity: Average case O(n), where n is the number of elements in the container.
        */
        void reserve(size_t count) {
            rehash(std::ceil(count / max_load_factor()));
        }

        // You might want to add const_iterator support as well, similar to the iterator class
    };

}// namespace userDefineDataStructure