/**
 * @file vector.h
 * @brief Custom vector implementation similar to std::vector.
 */

#pragma once

#include <algorithm>
#include <limits>
#include <memory>
#include <stdexcept>

namespace userDefineDataStructure {

    /**
     * @class vector
     * @brief A dynamic array implementation with similar interface to std::vector.
     * 
     * @tparam T The type of elements stored in the vector.
     * @tparam Allocator The allocator used for all memory management.
     */
    template<typename T, typename Allocator = std::allocator<T>>
    class vector {
    public:
        using value_type = T;                                                          ///< The type of elements.
        using allocator_type = Allocator;                                              ///< The allocator type.
        using size_type = std::size_t;                                                 ///< An unsigned integral type.
        using difference_type = std::ptrdiff_t;                                        ///< A signed integral type.
        using reference = value_type &;                                                ///< Reference to an element.
        using const_reference = const value_type &;                                    ///< Const reference to an element.
        using pointer = typename std::allocator_traits<Allocator>::pointer;            ///< Pointer to an element.
        using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;///< Const pointer to an element.
        using iterator = pointer;                                                      ///< Random access iterator.
        using const_iterator = const_pointer;                                          ///< Const random access iterator.

        /**
         * @brief Default constructor.
         * Constructs an empty vector with the given allocator.
         */
        vector() noexcept(noexcept(Allocator()))
            : vector(Allocator()) {}

        /**
         * @brief Constructs an empty vector with the given allocator.
         * @param alloc Allocator to use for memory management.
         */
        explicit vector(const Allocator &alloc) noexcept
            : alloc_(alloc) {}

        /**
         * @brief Constructs a vector with count copies of value.
         * @param count Number of elements to initialize.
         * @param value The value to initialize elements with.
         * @param alloc Allocator to use for memory management.
         */
        vector(size_type count, const T &value, const Allocator &alloc = Allocator())
            : alloc_(alloc) {
            assign(count, value);
        }

        /**
         * @brief Constructs a vector with count default-initialized elements.
         * @param count Number of elements to initialize.
         * @param alloc Allocator to use for memory management.
         */
        explicit vector(size_type count, const Allocator &alloc = Allocator())
            : alloc_(alloc) {
            resize(count);
        }

        /**
         * @brief Copy constructor.
         * @param other The vector to copy from.
         */
        vector(const vector &other)
            : alloc_(other.alloc_) {
            assign(other.begin(), other.end());
        }

        /**
         * @brief Move constructor.
         * @param other The vector to move from.
         */
        vector(vector &&other) noexcept
            : begin_(other.begin_),
              end_(other.end_),
              cap_(other.cap_),
              alloc_(std::move(other.alloc_)) {
            other.begin_ = other.end_ = other.cap_ = nullptr;
        }

        /**
         * @brief Destructor.
         * Destroys the vector and frees the allocated memory.
         */
        ~vector() {
            clear();
            deallocate();
        }

        // Assignment operators
        /**
         * @brief Copy assignment operator.
         * @param other The vector to copy from.
         * @return Reference to this vector.
         */
        vector &operator=(const vector &other) {
            if (this != &other)
                assign(other.begin(), other.end());
            return *this;
        }

        /**
         * @brief Move assignment operator.
         * @param other The vector to move from.
         * @return Reference to this vector.
         */
        vector &operator=(vector &&other) noexcept {
            swap(other);
            return *this;
        }

        /**
         * @brief Constructs a vector with elements from an initializer list.
         * @param init Initializer list with elements to initialize the vector.
         * @param alloc Allocator to use for memory management.
         */
        vector(std::initializer_list<T> init, const Allocator &alloc = Allocator())
            : alloc_(alloc) {
            assign(init.begin(), init.end());
        }

        /**
         * @brief Assigns elements from an initializer list to the vector.
         * @param ilist Initializer list with elements to assign.
         * @return Reference to this vector.
         */
        vector &operator=(std::initializer_list<T> ilist) {
            assign(ilist.begin(), ilist.end());
            return *this;
        }

        /**
         * @brief Assigns elements from an initializer list to the vector.
         * @param ilist Initializer list with elements to assign.
         */
        void assign(std::initializer_list<T> ilist) {
            assign(ilist.begin(), ilist.end());
        }

        /**
         * @brief Assigns count copies of value to the vector.
         * @param count Number of elements to assign.
         * @param value The value to assign to each element.
         */
        void assign(size_type count, const T &value) {
            clear();
            if (count > capacity()) {
                deallocate();
                begin_ = allocate(count);
                cap_ = begin_ + count;
            }
            end_ = begin_ + count;
            std::fill_n(begin_, count, value);
        }

        /**
         * @brief Assigns elements from a range [first, last) to the vector.
         * @tparam InputIt Input iterator type.
         * @param first Iterator to the first element in the range.
         * @param last Iterator to the last element in the range.
         */
        template<typename InputIt>
        void assign(InputIt first, InputIt last) {
            clear();
            for (; first != last; ++first)
                push_back(*first);
        }

        /**
         * @brief Accesses the element at specified position with bounds checking.
         * @param pos Position of the element to return.
         * @return Reference to the element at specified position.
         * @throw std::out_of_range if pos >= size().
         */
        reference at(size_type pos) {
            if (pos >= size())
                throw std::out_of_range("vector::at");
            return begin_[pos];
        }

        /**
         * @brief Accesses the element at specified position with bounds checking.
         * @param pos Position of the element to return.
         * @return Const reference to the element at specified position.
         * @throw std::out_of_range if pos >= size().
         */
        const_reference at(size_type pos) const {
            if (pos >= size())
                throw std::out_of_range("vector::at");
            return begin_[pos];
        }

        /**
         * @brief Accesses the element at specified position.
         * @param pos Position of the element to return.
         * @return Reference to the element at specified position.
         */
        reference operator[](size_type pos) { return begin_[pos]; }

        /**
         * @brief Accesses the element at specified position.
         * @param pos Position of the element to return.
         * @return Const reference to the element at specified position.
         */
        const_reference operator[](size_type pos) const { return begin_[pos]; }

        /**
         * @brief Accesses the first element.
         * @return Reference to the first element.
         */
        reference front() { return *begin_; }

        /**
         * @brief Accesses the first element.
         * @return Const reference to the first element.
         */
        const_reference front() const { return *begin_; }

        /**
         * @brief Accesses the last element.
         * @return Reference to the last element.
         */
        reference back() { return *(end_ - 1); }

        /**
         * @brief Accesses the last element.
         * @return Const reference to the last element.
         */
        const_reference back() const { return *(end_ - 1); }

        /**
         * @brief Returns a pointer to the underlying array serving as element storage.
         * @return Pointer to the underlying array.
         */
        T *data() noexcept { return begin_; }

        /**
         * @brief Returns a pointer to the underlying array serving as element storage.
         * @return Const pointer to the underlying array.
         */
        const T *data() const noexcept { return begin_; }

        /**
         * @brief Returns an iterator to the first element.
         * @return Iterator to the first element.
         */
        iterator begin() noexcept { return begin_; }

        /**
         * @brief Returns a const iterator to the first element.
         * @return Const iterator to the first element.
         */
        const_iterator begin() const noexcept { return begin_; }

        /**
         * @brief Returns a const iterator to the first element.
         * @return Const iterator to the first element.
         */
        const_iterator cbegin() const noexcept { return begin_; }

        /**
         * @brief Returns an iterator to the element following the last element.
         * @return Iterator to the element following the last element.
         */
        iterator end() noexcept { return end_; }

        /**
         * @brief Returns a const iterator to the element following the last element.
         * @return Const iterator to the element following the last element.
         */
        const_iterator end() const noexcept { return end_; }

        /**
         * @brief Returns a const iterator to the element following the last element.
         * @return Const iterator to the element following the last element.
         */
        const_iterator cend() const noexcept { return end_; }

        /**
         * @brief Checks if the vector is empty.
         * @return True if the vector is empty, false otherwise.
         */
        bool empty() const noexcept { return begin_ == end_; }

        /**
         * @brief Returns the number of elements in the vector.
         * @return The number of elements in the vector.
         */
        size_type size() const noexcept { return end_ - begin_; }

        /**
         * @brief Returns the number of elements that the vector can hold before needing to reallocate.
         * @return The capacity of the vector.
         */
        size_type capacity() const noexcept { return cap_ - begin_; }

        /**
         * @brief Increases the capacity of the vector to at least new_cap.
         * @param new_cap The new capacity.
         * @throw std::length_error if new_cap > max_size().
         */
        void reserve(size_type new_cap) {
            if (new_cap > capacity())
                reallocate(new_cap);
        }

        // Modifiers
        /**
         * @brief Clears the contents of the vector.
         * Leaves the capacity unchanged.
         */
        void clear() noexcept {
            destroy_range(begin_, end_);
            end_ = begin_;
        }

        /**
         * @brief Appends the given value to the end of the vector.
         * @param value The value to append.
         */
        void push_back(const T &value) {
            if (end_ == cap_)
                reallocate(recommend(size() + 1));
            std::allocator_traits<Allocator>::construct(alloc_, end_, value);
            ++end_;
        }

        /**
         * @brief Appends the given value to the end of the vector.
         * @param value The value to append.
         */
        void push_back(T &&value) {
            if (end_ == cap_)
                reallocate(recommend(size() + 1));
            std::allocator_traits<Allocator>::construct(alloc_, end_, std::move(value));
            ++end_;
        }

        /**
         * @brief Removes the last element of the vector.
         * Reduces the size by one.
         */
        void pop_back() {
            --end_;
            std::allocator_traits<Allocator>::destroy(alloc_, end_);
        }

        /**
         * @brief Resizes the vector to contain count elements.
         * @param count The new size.
         * If count < size(), elements are erased.
         * If count > size(), default-inserted elements are appended.
         */
        void resize(size_type count) {
            if (count > size()) {
                if (count > capacity())
                    reallocate(recommend(count));
                std::uninitialized_value_construct(end_, begin_ + count);
            } else if (count < size())
                destroy_range(begin_ + count, end_);
            end_ = begin_ + count;
        }

        /**
         * @brief Resizes the vector to contain count elements.
         * @param count The new size.
         * @param value The value to initialize the new elements with.
         * If count < size(), elements are erased.
         * If count > size(), copies of value are appended.
         */
        void resize(size_type count, const value_type &value) {
            if (count > size()) {
                if (count > capacity())
                    reallocate(recommend(count));
                std::uninitialized_fill(end_, begin_ + count, value);
            } else if (count < size())
                destroy_range(begin_ + count, end_);
            end_ = begin_ + count;
        }

        /**
         * @brief Swaps the contents of this vector with another vector.
         * @param other The vector to swap with.
         */
        void swap(vector &other) noexcept {
            std::swap(begin_, other.begin_);
            std::swap(end_, other.end_);
            std::swap(cap_, other.cap_);
            std::swap(alloc_, other.alloc_);
        }

    private:
        pointer begin_ = nullptr;              ///< Pointer to the first element.
        pointer end_ = nullptr;                ///< Pointer to the one-past-last element.
        pointer cap_ = nullptr;                ///< Pointer to the end of allocated storage.
        [[no_unique_address]] Allocator alloc_;///< Allocator used for all memory management.

        // Helper functions
        /**
         * @brief Allocates memory for n elements.
         * @param n The number of elements to allocate.
         * @return Pointer to the allocated memory.
         */
        pointer allocate(size_type n) {
            return n != 0 ? std::allocator_traits<Allocator>::allocate(alloc_, n) : nullptr;
        }

        /**
         * @brief Deallocates the previously allocated memory.
         */
        void deallocate() {
            if (begin_)
                std::allocator_traits<Allocator>::deallocate(alloc_, begin_, capacity());
        }

        /**
         * @brief Recommends a new capacity based on the desired size.
         * @param new_size The desired size.
         * @return The recommended capacity.
         * @throw std::length_error if new_size > max_size().
         */
        size_type recommend(size_type new_size) const {
            const size_type ms = max_size();
            if (new_size > ms)
                throw std::length_error("vector::reserve");
            const size_type cap = capacity();
            if (cap >= ms / 2)
                return ms;
            return std::max(2 * cap, new_size);
        }

        /**
         * @brief Reallocates the vector to have the specified new capacity.
         * @param new_cap The new capacity.
         */
        void reallocate(size_type new_cap) {
            pointer new_begin = allocate(new_cap);
            pointer new_end = new_begin;
            try {
                new_end = std::uninitialized_move(begin_, end_, new_begin);
            } catch (...) {
                std::allocator_traits<Allocator>::deallocate(alloc_, new_begin, new_cap);
                throw;
            }
            destroy_range(begin_, end_);
            deallocate();
            begin_ = new_begin;
            end_ = new_end;
            cap_ = new_begin + new_cap;
        }

        /**
         * @brief Destroys the range of elements [first, last).
         * @param first Iterator to the first element in the range.
         * @param last Iterator to the element past the end of the range.
         */
        void destroy_range(pointer first, pointer last) {
            for (; first != last; ++first)
                std::allocator_traits<Allocator>::destroy(alloc_, first);
        }

        /**
         * @brief Returns the maximum number of elements the vector can hold.
         * @return The maximum size.
         */
        size_type max_size() const noexcept {
            return std::min<size_type>(
                    std::allocator_traits<Allocator>::max_size(alloc_),
                    std::numeric_limits<difference_type>::max());
        }
    };

}// namespace userDefineDataStructure
