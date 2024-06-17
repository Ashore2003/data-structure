#include <iostream>
#include <iterator>

template <typename T> class List {
public:
  struct Node {
    T data;
    Node *next;
    Node(T val) : data(val), next(nullptr) {}
  };

  List() : head(nullptr), tail(nullptr) {}

  ~List() { clear(); }

  void add(T value) {
    Node *newNode = new Node(value);
    if (!head) {
      head = newNode;
      tail = newNode;
    } else {
      tail->next = newNode;
      tail = newNode;
    }
  }

  bool remove(T value) {
    Node *current = head, *prev = nullptr;
    while (current != nullptr) {
      if (current->data == value) {
        if (prev)
          prev->next = current->next;
        else
          head = current->next;
        if (current == tail)
          tail = prev;
        delete current;
        return true;
      }
      prev = current;
      current = current->next;
    }
    return false;
  }

  Node *find(T value) {
    Node *current = head;
    while (current != nullptr) {
      if (current->data == value)
        return current;
      current = current->next;
    }
    return nullptr;
  }

  void clear() {
    Node *current = head;
    while (current != nullptr) {
      Node *next = current->next;
      delete current;
      current = next;
    }
    head = nullptr;
    tail = nullptr;
  }

  void print() {
    Node *current = head;
    while (current != nullptr) {
      std::cout << current->data << " ";
      current = current->next;
    }
    std::cout << std::endl;
  }

  class iterator {
  public:
    iterator(Node *ptr) : nodePtr(ptr) {}

    iterator operator++() {
      nodePtr = nodePtr->next;
      return *this;
    }

    T &operator*() { return nodePtr->data; }

    bool operator!=(const iterator &other) { return nodePtr != other.nodePtr; }

  private:
    Node *nodePtr;
  };

  iterator begin() { return iterator(head); }

  iterator end() { return iterator(nullptr); }

private:
  Node *head;
  Node *tail;
};
