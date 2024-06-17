#pragma once

#include "list.h"
#include <iostream>
#include <stdexcept>

template <typename T> class Queue {
public:
  Queue(size_t max_size) : max_size(max_size) {}

  void enqueue(T value) {
    if (data.size() >= max_size)
      throw std::runtime_error("Queue capacity exceeded");
    data.add(value);
  }

  void dequeue() {
    if (data.empty())
      throw std::runtime_error("Attempting to dequeue from an empty queue");
    data.pop_front();
  }

  T &front() {
    if (data.empty())
      throw std::runtime_error(
          "Attempting to access the front of an empty queue");
    return data.front();
  }

  bool empty() const { return data.empty(); }

  size_t size() const { return data.size(); }

  using iterator = typename List<T>::iterator;
  
  using const_iterator = typename List<T>::const_iterator;

  iterator begin() { return data.begin(); }

  iterator end() { return data.end(); }

  const_iterator begin() const { return data.cbegin(); }

  const_iterator end() const { return data.cend(); }

private:
  List<T> data;
  size_t max_size;
};
