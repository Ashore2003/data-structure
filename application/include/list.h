#pragma once

#include <iostream>
#include <iterator>
#include <memory>

template <typename T> class List {
public:
  // Node structure for the linked list
  struct Node {
    T data;
    std::unique_ptr<Node> next;
    Node(T val) : data(val), next(nullptr) {}
  };

  List() : head(nullptr), tail(nullptr), count(0) {}

  ~List() { clear(); }

  // Add a value to the end of the list
  void add(T value) {
    auto newNode = std::make_unique<Node>(value);
    Node *newRawNode = newNode.get();
    if (!head) {
      head = std::move(newNode);
      tail = newRawNode;
    } else {
      tail->next = std::move(newNode);
      tail = newRawNode;
    }
    count++;
  }

  // Remove the first occurrence of the value
  bool remove(T value) {
    Node *current = head.get(), *prev = nullptr;
    while (current != nullptr) {
      if (current->data == value) {
        std::unique_ptr<Node> tempNext = std::move(current->next);
        if (prev)
          prev->next = std::move(tempNext);
        else
          head = std::move(tempNext);
        if (current == tail)
          tail = prev;
        count--;
        return true;
      }
      prev = current;
      current = current->next.get();
    }
    return false;
  }

  // Remove the first element
  void pop_front() {
    if (head == nullptr)
      return;
    head = std::move(head->next);
    if (head == nullptr)
      tail = nullptr;
    count--;
  }

  // Access the first element
  T &front() {
    if (!head)
      throw std::runtime_error("List is empty");
    return head->data;
  }

  // Access the last element
  T &back() {
    if (!tail)
      throw std::runtime_error("List is empty");
    return tail->data;
  }

  // Insert a value at the front of the list
  void insert_front(T value) {
    auto newNode = std::make_unique<Node>(value);
    newNode->next = std::move(head);
    head = std::move(newNode);
    if (!tail)
      tail = head.get();
    count++;
  }

  // Remove the last element
  void pop_back() {
    if (!tail)
      return;
    if (head.get() == tail) {
      head.reset();
      tail = nullptr;
    } else {
      Node *current = head.get();
      while (current->next.get() != tail)
        current = current->next.get();
      current->next.reset();
      tail = current;
    }
    count--;
  }

  // Check if the list is empty
  bool empty() const { return count == 0; }

  // Get the number of elements in the list
  size_t size() const { return count; }

  // Find the first node containing the specified value
  Node *find(T value) {
    Node *current = head.get();
    while (current != nullptr) {
      if (current->data == value)
        return current;
      current = current->next.get();
    }
    return nullptr;
  }

  // Clear all elements from the list
  void clear() {
    head.reset();
    tail = nullptr;
    count = 0;
  }

  // Iterator class for list traversal
  class iterator {
  public:
    explicit iterator(Node *ptr) : nodePtr(ptr) {}

    // Prefix increment
    iterator &operator++() {
      nodePtr = nodePtr->next.get();
      return *this;
    }

    // Dereference
    T &operator*() { return nodePtr->data; }

    // Inequality check
    bool operator!=(const iterator &other) const {
      return nodePtr != other.nodePtr;
    }

  private:
    Node *nodePtr;
  };

  // Const iterator class for read-only list traversal
  class const_iterator {
  public:
    explicit const_iterator(const Node *ptr) : nodePtr(ptr) {}

    // Prefix increment
    const_iterator &operator++() {
      nodePtr = nodePtr->next.get();
      return *this;
    }

    // Dereference
    const T &operator*() const { return nodePtr->data; }

    // Inequality check
    bool operator!=(const const_iterator &other) const {
      return nodePtr != other.nodePtr;
    }

  private:
    const Node *nodePtr;
  };

  // Get iterator to the beginning
  iterator begin() { return iterator(head.get()); }

  // Get iterator to the end (past-the-end)
  iterator end() { return iterator(nullptr); }

  // Get const iterator to the beginning
  const_iterator begin() const { return const_iterator(head.get()); }

  // Get const iterator to the end (past-the-end)
  const_iterator end() const { return const_iterator(nullptr); }

private:
  std::unique_ptr<Node> head;
  Node *tail;
  size_t count;
};
