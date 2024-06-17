#pragma once

#include <cassert>
#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <utility>

template <typename T, std::size_t N> class array {
public:
  typedef T value_type;
  typedef T *pointer;
  typedef const T *const_pointer;
  typedef T &reference;
  typedef const T &const_reference;
  typedef T *iterator;
  typedef const T *const_iterator;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;

  // Iterator functions
  iterator begin() { return elems; }

  const_iterator begin() const { return elems; }

  iterator end() { return elems + N; }

  const_iterator end() const { return elems + N; }

  const_iterator cbegin() const { return elems; }

  const_iterator cend() const { return elems + N; }

  // Capacity
  constexpr size_type size() const { return N; }

  constexpr size_type max_size() const { return N; }

  constexpr bool empty() const { return N == 0; }

  // Element access
  reference operator[](size_type i) {
    assert(i < N && "out of range");
    return elems[i];
  }

  const_reference operator[](size_type i) const {
    assert(i < N && "out of range");
    return elems[i];
  }

  reference at(size_type i) {
    if (i >= N)
      throw std::out_of_range("array::at");
    return elems[i];
  }

  const_reference at(size_type i) const {
    if (i >= N)
      throw std::out_of_range("array::at");
    return elems[i];
  }

  reference front() { return elems[0]; }

  const_reference front() const { return elems[0]; }

  reference back() { return elems[N - 1]; }

  const_reference back() const { return elems[N - 1]; }

  pointer data() { return elems; }

  const_pointer data() const { return elems; }

  // Fill and swap
  void fill(const T &value) {
    for (size_type i = 0; i < N; ++i)
      elems[i] = value;
  }

  void swap(array &other) noexcept {
    for (size_type i = 0; i < N; ++i)
      std::swap(elems[i], other.elems[i]);
  }

private:
  T elems[N];
};

// Helper function to swap two arrays
template <typename T, std::size_t N>
void swap(array<T, N> &a, array<T, N> &b) noexcept {
  a.swap(b);
}
