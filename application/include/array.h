#pragma once

#include <cassert>
#include <cstddef>
#include <stdexcept>
#include <utility>

/**
* @class userDefineDataStructure::Array
* 
* @brief A fixed-size array class template with STL-like interface.
* 
* This class provides a generic array implementation with
* support for standard container operations such as begin, end, size, and element access
* with bounds checking. It encapsulates a native array for fast access and simple use cases.
* 
* @tparam T The type of elements stored in the array.
* @tparam N The number of elements in the array.
* 
* Key features:
* - Constant time access to elements via direct indexing.
* - Support for range-based for loops via iterators.
* - Bounds-checked element access with the at() method.
* - Compile-time fixed size with no dynamic memory allocation.
* - Support for aggregate initialization.
* 
* Usage example:
* @code
* userDefineDataStructure::Array<int, 3> myArray{{1, 2, 3}};
* for(const auto& item : myArray) {
*     std::cout << item << " ";
* }
* // Output: 1 2 3
* @endcode
* 
* @warning This class does not manage resources beyond its static allocation; thus,
*          it is not suitable for managing dynamic resources without proper wrappers.
*/
namespace userDefineDataStructure {
    template<typename T, std::size_t N>
    class Array {
    public:
        /// @brief The type of elements stored in the array.
        typedef T value_type;
        /// @brief Pointer to the element type.
        typedef T *pointer;
        /// @brief Const pointer to the element type.
        typedef const T *const_pointer;
        /// @brief Reference to the element type.
        typedef T &reference;
        /// @brief Const reference to the element type.
        typedef const T &const_reference;
        /// @brief Iterator type (non-const).
        typedef T *iterator;
        /// @brief Const iterator type.
        typedef const T *const_iterator;
        /// @brief Type used for array indices and sizes.
        typedef std::size_t size_type;
        /// @brief Type used for pointer arithmetic.
        typedef std::ptrdiff_t difference_type;

        /**
        * @brief Get an iterator to the beginning of the array.
        * @return An iterator pointing to the first element.
        */
        iterator begin() { return elems; }

        /**
        * @brief Get a const iterator to the beginning of the array.
        * @return A const iterator pointing to the first element.
        */
        const_iterator begin() const { return elems; }

        /**
        * @brief Get an iterator to the end of the array.
        * @return An iterator pointing one past the last element.
        */
        iterator end() { return elems + N; }

        /**
        * @brief Get a const iterator to the end of the array.
        * @return A const iterator pointing one past the last element.
        */
        const_iterator end() const { return elems + N; }

        /**
        * @brief Get a const iterator to the beginning of the array.
        * @return A const iterator pointing to the first element.
        */
        const_iterator cbegin() const { return elems; }

        /**
        * @brief Get a const iterator to the end of the array.
        * @return A const iterator pointing one past the last element.
        */
        const_iterator cend() const { return elems + N; }

        /**
        * @brief Get the number of elements in the array.
        * @return The number of elements in the array.
        */
        [[nodiscard]] constexpr size_type size() const { return N; }

        /**
        * @brief Get the maximum possible number of elements.
        * @return The maximum number of elements the array can hold.
        */
        [[nodiscard]] constexpr size_type max_size() const { return N; }

        /**
        * @brief Check if the array is empty.
        * @return true if the array is empty, false otherwise.
        */
        [[nodiscard]] constexpr bool empty() const { return N == 0; }

        /**
        * @brief Access an element of the array.
        * @param i The index of the element to access.
        * @return A reference to the element at position i.
        * @note This function uses assert and will terminate the program if i is out of range.
        */
        reference operator[](size_type i) {
            assert(i < N && "out of range");
            return elems[i];
        }

        /**
        * @brief Access an element of the array (const version).
        * @param i The index of the element to access.
        * @return A const reference to the element at position i.
        * @note This function uses assert and will terminate the program if i is out of range.
        */
        const_reference operator[](size_type i) const {
            assert(i < N && "out of range");
            return elems[i];
        }

        /**
        * @brief Access an element of the array with bounds checking.
        * @param i The index of the element to access.
        * @return A reference to the element at position i.
        * @throw std::out_of_range if i is not within the range of the array.
        */
        reference at(size_type i) {
            if (i >= N)
                throw std::out_of_range("array::at");
            return elems[i];
        }

        /**
        * @brief Access an element of the array with bounds checking (const version).
        * @param i The index of the element to access.
        * @return A const reference to the element at position i.
        * @throw std::out_of_range if i is not within the range of the array.
        */
        const_reference at(size_type i) const {
            if (i >= N)
                throw std::out_of_range("array::at");
            return elems[i];
        }

        /**
        * @brief Access the first element of the array.
        * @return A reference to the first element.
        */
        reference front() { return elems[0]; }

        /**
        * @brief Access the first element of the array (const version).
        * @return A const reference to the first element.
        */
        const_reference front() const { return elems[0]; }

        /**
        * @brief Access the last element of the array.
        * @return A reference to the last element.
        */
        reference back() { return elems[N - 1]; }

        /**
        * @brief Access the last element of the array (const version).
        * @return A const reference to the last element.
        */
        const_reference back() const { return elems[N - 1]; }

        /**
        * @brief Get a pointer to the underlying array.
        * @return A pointer to the first element of the array.
        */
        pointer data() { return elems; }

        /**
        * @brief Get a const pointer to the underlying array.
        * @return A const pointer to the first element of the array.
        */
        const_pointer data() const { return elems; }

        /**
        * @brief Fill the array with a specific value.
        * @param value The value to fill the array with.
        */
        void fill(const T &value) {
            for (size_type i = 0; i < N; ++i)
                elems[i] = value;
        }

        /**
        * @brief Swap the contents of this array with another.
        * @param other The array to swap contents with.
        */
        void swap(Array &other) noexcept {
            for (size_type i = 0; i < N; ++i)
                std::swap(elems[i], other.elems[i]);
        }

    private:
        T elems[N];///< The actual array of elements.
    };

    /**
    * @brief Swap the contents of two Array objects.
    * @tparam T The type of elements in the arrays.
    * @tparam N The size of the arrays.
    * @param a The first array.
    * @param b The second array.
    */
    template<typename T, std::size_t N>
    void swap(Array<T, N> &a, Array<T, N> &b) noexcept {
        a.swap(b);
    }

}// namespace userDefineDataStructure