#pragma once

#include "list.h"

namespace userDefineDataStructure {

    /**
    * @brief A template class implementing a queue data structure.
    *
    * This queue is implemented using a list as the underlying container.
    * It provides standard queue operations such as push, pop, front, and back.
    *
    * @tparam T The type of elements stored in the queue.
    */
    template<typename T>
    class Queue {
    private:
        userDefineDataStructure::List<T> data; /**< The underlying list used to store queue elements. */

    public:
        /**
        * @brief Default constructor.
        *
        * Constructs an empty queue.
        */
        Queue() = default;

        /**
        * @brief Copy constructor.
        *
        * Constructs a queue with a copy of the contents of another queue.
        *
        * @param other The queue to copy from.
        */
        Queue(const Queue &other) = default;

        /**
        * @brief Move constructor.
        *
        * Constructs a queue by moving the contents of another queue.
        *
        * @param other The queue to move from.
        */
        Queue(Queue &&other) noexcept = default;

        /**
        * @brief Copy assignment operator.
        *
        * Replaces the contents of the queue with a copy of the contents of another queue.
        *
        * @param other The queue to copy from.
        * @return A reference to this queue.
        */
        Queue &operator=(const Queue &other) = default;

        /**
        * @brief Move assignment operator.
        *
        * Replaces the contents of the queue with the contents of another queue using move semantics.
        *
        * @param other The queue to move from.
        * @return A reference to this queue.
        */
        Queue &operator=(Queue &&other) noexcept = default;

        /**
        * @brief Destructor.
        *
        * Destroys the queue and frees all allocated memory.
        */
        ~Queue() = default;

        /**
        * @brief Adds an element to the back of the queue.
        *
        * @param value The value to be added to the queue.
        */
        void push(const T &value) {
            data.push_back(value);
        }

        /**
        * @brief Removes the front element from the queue.
        *
        * @throws std::runtime_error if the queue is empty.
        */
        void pop() {
            if (empty()) {
                throw std::runtime_error("Queue is empty");
            }
            data.pop_front();
        }

        /**
        * @brief Gets a reference to the front element of the queue.
        *
        * @return A reference to the front element.
        * @throws std::runtime_error if the queue is empty.
        */
        T &front() {
            if (empty()) {
                throw std::runtime_error("Queue is empty");
            }
            return data.front();
        }

        /**
        * @brief Gets a const reference to the front element of the queue.
        *
        * @return A const reference to the front element.
        * @throws std::runtime_error if the queue is empty.
        */
        const T &front() const {
            if (empty()) {
                throw std::runtime_error("Queue is empty");
            }
            return data.front();
        }

        /**
        * @brief Gets a reference to the back element of the queue.
        *
        * @return A reference to the back element.
        * @throws std::runtime_error if the queue is empty.
        */
        T &back() {
            if (empty()) {
                throw std::runtime_error("Queue is empty");
            }
            return data.back();
        }

        /**
        * @brief Gets a const reference to the back element of the queue.
        *
        * @return A const reference to the back element.
        * @throws std::runtime_error if the queue is empty.
        */
        const T &back() const {
            if (empty()) {
                throw std::runtime_error("Queue is empty");
            }
            return data.back();
        }

        /**
        * @brief Checks if the queue is empty.
        *
        * @return true if the queue is empty, false otherwise.
        */
        [[nodiscard]] bool empty() const {
            return data.empty();
        }

        /**
        * @brief Gets the current size of the queue.
        *
        * @return The number of elements in the queue.
        */
        [[nodiscard]] size_t size() const {
            return data.size();
        }

        /**
        * @brief Swaps the contents of this queue with another queue.
        *
        * @param other The queue to swap contents with.
        */
        void swap(Queue &other) noexcept {
            data.swap(other.data);
        }
    };

    /**
    * @brief Non-member swap function for Queue.
    *
    * Swaps the contents of two queues.
    *
    * @tparam T The type of elements stored in the queues.
    * @param lhs The first queue to swap.
    * @param rhs The second queue to swap.
    */
    template<typename T>
    void swap(Queue<T> &lhs, Queue<T> &rhs) noexcept {
        lhs.swap(rhs);
    }

}// namespace userDefineDataStructure