#pragma once

#include "list.h"
#include <iostream>
#include <stdexcept>

template <typename T> class Deque {
public:
  Deque() {}

  void push_front(const T &value) {
    if (list.empty())
      list.add(value);
    else
      list.insert_front(value);
  }

  void push_back(const T &value) { list.add(value); }

  void pop_front() {
    if (list.empty()) {
      throw std::runtime_error("Cannot pop from an empty deque");
    }
    list.pop_front();
  }

  void pop_back() {
    if (list.empty()) {
      throw std::runtime_error("Cannot pop from an empty deque");
    }
    list.pop_back();
  }

  T &front() { return list.front(); }

  T &back() {
    if (list.empty()) {
      throw std::runtime_error("Deque is empty");
    }
    return list.back();
  }

  bool empty() const { return list.empty(); }

  size_t size() const { return list.size(); }

private:
  List<T> list;
};