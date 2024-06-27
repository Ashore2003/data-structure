#pragma once

#include <initializer_list>
#include <memory>
#include <stdexcept>

/**
 * @class userDefineDataStructure::List
 * 
 * @brief A doubly linked list implementation with STL-like interface.
 * 
 * This class provides a generic doubly linked list implementation with
 * support for standard container operations such as push_back, push_front,
 * pop_back, pop_front, and iteration. It uses smart pointers for automatic
 * memory management and provides both const and non-const iterators.
 * 
 * @tparam T The type of elements stored in the list.
 * 
 * Key features:
 * - Constant time insertion and removal of elements at both ends
 * - Linear time insertion and removal of elements at arbitrary positions
 * - Bidirectional iteration
 * - Exception safety (basic guarantee for most operations)
 * - Move semantics support
 * 
 * Usage example:
 * @code
 * userDefineDataStructure::List<int> myList;
 * myList.push_back(1);
 * myList.push_back(2);
 * myList.push_front(0);
 * 
 * for(const auto& item : myList) {
 *     std::cout << item << " ";
 * }
 * // Output: 0 1 2
 * @endcode
 * 
 * @warning This class is not thread-safe. External synchronization is required
 *          for concurrent access.
 */

namespace userDefineDataStructure {

    /**
    * @brief A doubly linked list implementation.
    * 
    * @tparam T The type of elements stored in the list.
    */
    template<typename T>
    class List {
    private:
        /**
        * @brief Node structure for the linked list.
        */
        struct Node {
            T data;                    ///< Data stored in the node
            std::unique_ptr<Node> next;///< Smart pointer to the next node
            Node *prev;                ///< Raw pointer to the previous node

            /**
            * @brief Construct a new Node object with a copy of the value.
            * 
             * @param value The value to be stored in the node.
            */
            explicit Node(const T &value)
                : data(value), next(nullptr), prev(nullptr) {}

            /**
            * @brief Construct a new Node object with perfect forwarding.
             * 
            * @tparam Args Variadic template for perfect forwarding.
            * @param args Arguments to be forwarded to the T constructor.
            */
            template<typename... Args>
            explicit Node(Args &&...args)
                : data(std::forward<Args>(args)...), next(nullptr), prev(nullptr) {}
        };

        std::unique_ptr<Node> head;///< Smart pointer to the first node
        Node *tail;                ///< Raw pointer to the last node
        size_t list_size;          ///< Current size of the list

    public:
        /**
        * @brief Construct a new empty List object.
        */
        List()
            : head(nullptr), tail(nullptr), list_size(0) {}

        /**
        * @brief Destroy the List object and free all nodes.
        */
        ~List() {
            clear();
        }

        /**
        * @brief Copy construct a new List object.
        * 
        * @param other The List to be copied.
        */
        List(const List &other)
            : List() {
            for (const auto &value: other)
                push_back(value);
        }

        /**
        * @brief Copy assignment operator.
        * 
        * @param other The List to be copied.
        * @return List& Reference to this List.
        */
        List &operator=(const List &other) {
            if (this != &other) {
                List temp(other);
                swap(temp);
            }
            return *this;
        }

        /**
        * @brief Move construct a new List object.
         * 
        * @param other The List to be moved from.
        */
        List(List &&other) noexcept
            : head(std::move(other.head)), tail(other.tail), list_size(other.list_size) {
            other.tail = nullptr;
            other.list_size = 0;
        }

        /**
        * @brief Move assignment operator.
         * 
        * @param other The List to be moved from.
        * @return List& Reference to this List.
        */
        List &operator=(List &&other) noexcept {
            if (this != &other) {
                clear();
                head = std::move(other.head);
                tail = other.tail;
                list_size = other.list_size;

                other.tail = nullptr;
                other.list_size = 0;
            }
            return *this;
        }

        /**
         * @brief Construct a new List object from an initializer list.
        * 
        * @param init The initializer list to construct from.
        */
        List(std::initializer_list<T> init)
            : List() {
            for (const auto &item: init)
                push_back(item);
        }

        /**
        * @brief Add an element to the end of the list.
        * 
         * @param value The value to be added.
        */
        void push_back(const T &value) {
            auto new_node = std::make_unique<Node>(value);
            Node *new_node_ptr = new_node.get();

            if (!tail) {
                head = std::move(new_node);
                tail = head.get();
            } else {
                new_node_ptr->prev = tail;
                tail->next = std::move(new_node);
                tail = new_node_ptr;
            }
            ++list_size;
        }

        /**
         * @brief Add an element to the beginning of the list.
        * 
        * @param value The value to be added.
        */
        void push_front(const T &value) {
            auto new_node = std::make_unique<Node>(value);
            Node *new_node_ptr = new_node.get();

            if (!head) {
                head = std::move(new_node);
                tail = head.get();
            } else {
                new_node_ptr->next = std::move(head);
                new_node_ptr->next->prev = new_node_ptr;
                head = std::move(new_node);
            }
            ++list_size;
        }

        /**
        * @brief Remove the last element from the list.
        */
        void pop_back() {
            if (!tail) return;
            if (tail->prev) {
                tail = tail->prev;
                tail->next.reset();
            } else {
                head.reset();
                tail = nullptr;
            }
            --list_size;
        }

        /**
        * @brief Remove the first element from the list.
        */
        void pop_front() {
            if (!head) return;
            if (head->next) {
                head = std::move(head->next);
                head->prev = nullptr;
            } else {
                head.reset();
                tail = nullptr;
            }
            --list_size;
        }

        /**
        * @brief Remove all elements from the list.
        */
        void clear() {
            head.reset();
            tail = nullptr;
            list_size = 0;
        }

        /**
        * @brief Remove all elements with the specified value from the list.
        *
        * This function traverses the list and removes all nodes that contain
        * the specified value. It properly handles cases where the removed node
        * is at the beginning or end of the list.
        *
        * @param value The value to be removed from the list.
        * @return size_t The number of elements removed.
        *
        * Time Complexity: O(n), where n is the number of elements in the list.
        * Space Complexity: O(1)
        */
        size_t remove(const T &value) {
            size_t removed_count = 0;
            Node *current = head.get();
            Node *prev = nullptr;

            while (current != nullptr) {
                if (current->data == value) {
                    ++removed_count;

                    if (prev == nullptr) {
                        // Removing the head node
                        head = std::move(current->next);
                        if (head)
                            head->prev = nullptr;
                        else
                            // List is now empty
                            tail = nullptr;
                        current = head.get();
                    } else {
                        // Removing a non-head node
                        prev->next = std::move(current->next);
                        if (prev->next)
                            prev->next->prev = prev;
                        else
                            // Removed the last node
                            tail = prev;
                        current = prev->next.get();
                    }
                } else {
                    prev = current;
                    current = current->next.get();
                }
            }

            list_size -= removed_count;
            return removed_count;
        }

        /**
        * @brief Get the current size of the list.
        * 
        * @return size_t The number of elements in the list.
        */
        [[nodiscard]] size_t size() const {
            return list_size;
        }

        /**
        * @brief Check if the list is empty.
        * 
        * @return true If the list is empty.
        * @return false If the list is not empty.
        */
        [[nodiscard]] bool empty() const {
            return list_size == 0;
        }

        /**
        * @brief Get a reference to the first element.
        * 
        * @return T& Reference to the first element.
        * @throw std::runtime_error If the list is empty.
        */
        T &front() {
            if (!head) throw std::runtime_error("List is empty");
            return head->data;
        }

        /**
        * @brief Get a const reference to the first element.
        * 
        * @return const T& Const reference to the first element.
        * @throw std::runtime_error If the list is empty.
        */
        const T &front() const {
            if (!head) throw std::runtime_error("List is empty");
            return head->data;
        }

        /**
        * @brief Get a reference to the last element.
        * 
        * @return T& Reference to the last element.
        * @throw std::runtime_error If the list is empty.
        */
        T &back() {
            if (!tail) throw std::runtime_error("List is empty");
            return tail->data;
        }

        /**
        * @brief Get a const reference to the last element.
        * 
        * @return const T& Const reference to the last element.
        * @throw std::runtime_error If the list is empty.
        */
        const T &back() const {
            if (!tail) throw std::runtime_error("List is empty");
            return tail->data;
        }

        /**
        * @brief Swap the contents of this list with another.
        * 
        * @param other The list to swap with.
        */
        void swap(List &other) noexcept {
            using std::swap;
            swap(head, other.head);
            swap(tail, other.tail);
            swap(list_size, other.list_size);
        }

        /**
        * @brief Iterator class for traversing the list.
        */
        class iterator {
        private:
            Node *current;

        public:
            // Iterator traits
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = T *;
            using reference = T &;

            /**
            * @brief Construct a new iterator object.
            * 
            * @param node Pointer to the current node.
            */
            explicit iterator(Node *node)
                : current(node) {}

            /**
            * @brief Prefix increment operator.
            * 
            * @return iterator& Reference to this iterator after increment.
            */
            iterator &operator++() {
                if (current) current = current->next.get();
                return *this;
            }

            /**
            * @brief Postfix increment operator.
            * 
            * @return iterator Copy of the iterator before increment.
            */
            iterator operator++(int) {
                iterator temp = *this;
                ++(*this);
                return temp;
            }

            /**
            * @brief Prefix decrement operator.
            * 
            * @return iterator& Reference to this iterator after decrement.
            */
            iterator &operator--() {
                if (current) current = current->prev;
                return *this;
            }

            /**
            * @brief Postfix decrement operator.
            * 
            * @return iterator Copy of the iterator before decrement.
            */
            iterator operator--(int) {
                iterator temp = *this;
                --(*this);
                return temp;
            }

            /**
            * @brief Dereference operator.
            * 
            * @return reference Reference to the data in the current node.
            */
            reference operator*() const { return current->data; }

            /**
            * @brief Arrow operator.
            * 
            * @return pointer Pointer to the data in the current node.
            */
            pointer operator->() const { return &current->data; }

            /**
            * @brief Equality comparison operator.
            * 
            * @param other The iterator to compare with.
            * @return true If both iterators point to the same node.
            * @return false If iterators point to different nodes.
            */
            bool operator==(const iterator &other) const { return current == other.current; }

            /**
            * @brief Inequality comparison operator.
            * 
            * @param other The iterator to compare with.
            * @return true If iterators point to different nodes.
            * @return false If both iterators point to the same node.
            */
            bool operator!=(const iterator &other) const { return !(*this == other); }

            friend class List;
        };

        /**
        * @brief Const iterator class for traversing the list.
        */
        class const_iterator {
        private:
            const Node *current;

        public:
            // Iterator traits
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = const T;
            using difference_type = std::ptrdiff_t;
            using pointer = const T *;
            using reference = const T &;

            /**
            * @brief Construct a new const_iterator object.
            * 
            * @param node Pointer to the current node.
            */
            explicit const_iterator(const Node *node)
                : current(node) {}

            /**
            * @brief Construct a new const_iterator object from a non-const iterator.
            * 
            * @param it The non-const iterator to convert from.
            */
            const_iterator(const iterator &it)
                : current(it.current) {}

            /**
            * @brief Prefix increment operator.
            * 
            * @return const_iterator& Reference to this const_iterator after increment.
            */
            const_iterator &operator++() {
                if (current) current = current->next.get();
                return *this;
            }

            /**
            * @brief Postfix increment operator.
            * 
            * @return const_iterator Copy of the const_iterator before increment.
            */
            const_iterator operator++(int) {
                const_iterator temp = *this;
                ++(*this);
                return temp;
            }

            /**
            * @brief Prefix decrement operator.
            * 
            * @return const_iterator& Reference to this const_iterator after decrement.
            */
            const_iterator &operator--() {
                if (current) current = current->prev;
                return *this;
            }

            /**
            * @brief Postfix decrement operator.
            * 
            * @return const_iterator Copy of the const_iterator before decrement.
            */
            const_iterator operator--(int) {
                const_iterator temp = *this;
                --(*this);
                return temp;
            }

            /**
            * @brief Dereference operator.
            * 
            * @return reference Const reference to the data in the current node.
            */
            reference operator*() const { return current->data; }

            /**
            * @brief Arrow operator.
            * 
            * @return pointer Const pointer to the data in the current node.
            */
            pointer operator->() const { return &current->data; }

            /**
            * @brief Equality comparison operator.
            * 
            * @param other The const_iterator to compare with.
            * @return true If both const_iterators point to the same node.
            * @return false If const_iterators point to different nodes.
            */
            bool operator==(const const_iterator &other) const { return current == other.current; }

            /**
            * @brief Inequality comparison operator.
            * 
            * @param other The const_iterator to compare with.
            * @return true If const_iterators point to different nodes.
            * @return false If both const_iterators point to the same node.
            */
            bool operator!=(const const_iterator &other) const { return !(*this == other); }

            friend class List;
        };

        /**
        * @brief Get an iterator to the beginning of the list.
        * 
        * @return iterator Iterator pointing to the first element.
        */
        iterator begin() { return iterator(head.get()); }

        /**
        * @brief Get an iterator to the end of the list.
        * 
        * @return iterator Iterator pointing one past the last element.
        */
        iterator end() { return iterator(nullptr); }

        /**
        * @brief Get a const iterator to the beginning of the list.
        * 
        * @return const_iterator Const iterator pointing to the first element.
        */
        const_iterator begin() const { return const_iterator(head.get()); }

        /**
        * @brief Get a const iterator to the end of the list.
        * 
        * @return const_iterator Const iterator pointing one past the last element.
        */
        const_iterator end() const { return const_iterator(nullptr); }

        /**
        * @brief Get a const iterator to the beginning of the list.
        * 
        * @return const_iterator Const iterator pointing to the first element.
        */
        const_iterator cbegin() const { return const_iterator(head.get()); }

        /**
        * @brief Get a const iterator to the end of the list.
        * 
        * @return const_iterator Const iterator pointing one past the last element.
        */
        const_iterator cend() const { return const_iterator(nullptr); }
    };

    /**
    * @brief Non-member swap function for List.
    * 
    * This function allows for ADL (Argument Dependent Lookup) and
    * provides a convenient way to swap two List objects.
    * 
    * @tparam T The type of elements stored in the list.
    * @param lhs The first List to swap.
    * @param rhs The second List to swap.
    */
    template<typename T>
    void swap(List<T> &lhs, List<T> &rhs) noexcept {
        lhs.swap(rhs);
    }

}// namespace userDefineDataStructure