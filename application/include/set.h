#include <functional>
#include <iterator>
#include <memory>


/**
 * @class userDefineDataStructure::set
 * 
 * @brief A Red-Black Tree based set implementation with an STL-like interface.
 * 
 * This class provides a generic set implementation using a Red-Black Tree for balanced
 * tree properties, ensuring that operations such as insertions, deletions, and lookups
 * can be done in logarithmic time. It supports unique keys and provides an iterator interface
 * that offers in-order traversal of the elements.
 * 
 * @tparam Key The type of elements stored in the set.
 * @tparam Compare A comparison function object type, std::less<Key> by default, that determines the key ordering.
 * @tparam Allocator Allocator type to use for all memory allocations of this container.
 * 
 * Key features:
 * - Logarithmic time complexity for insert, find, and erase operations.
 * - Provides bidirectional iterators that iterate over the set in sorted order.
 * - Customizable memory management through Allocators.
 * - Exception safety guarantees similar to those of standard associative containers.
 * - Supports copy construction and assignment.
 * 
 * Usage example:
 * @code
 * userDefineDataStructure::set<int> mySet;
 * mySet.insert(10);
 * mySet.insert(20);
 * mySet.insert(5);
 * 
 * for(const auto& key : mySet) {
 *     std::cout << key << " ";
 * }
 * // Output: 5 10 20
 * @endcode
 * 
 * @note This class does not support multi-key insertion as it is a set of unique elements.
 * 
 * @warning This class is not thread-safe. External synchronization is required for concurrent access.
 */
namespace userDefineDataStructure {
    template<class Key, class Compare = std::less<Key>, class Allocator = std::allocator<Key>>
    class set {
    private:
        /**
        * @enum Color
        * @brief Represents the color of a node in the Red-Black tree.
        *
        * In a Red-Black tree, each node is colored either red or black.
        * This coloring is used to ensure the tree remains balanced after insertions and deletions.
        */
        enum class Color { Red,
                           Black };

        /**
        * @struct Node
        * @brief Represents a node in the Red-Black tree.
        *
        * Each node contains the stored value, pointers to its parent and children,
        * and its color (red or black).
        */
        struct Node {
            Key value;   ///< The value stored in the node
            Node *parent;///< Pointer to the parent node
            Node *left;  ///< Pointer to the left child
            Node *right; ///< Pointer to the right child
            Color color; ///< Color of the node (Red or Black)

            /**
            * @brief Constructs a new Node with the given value.
            * @param val The value to be stored in the node.
             *
            * Initializes a new node with the given value, setting all pointers to nullptr
            * and the color to red. New nodes are always inserted as red in a Red-Black tree.
            */
            Node(const Key &val)
                : value(val), parent(nullptr), left(nullptr), right(nullptr), color(Color::Red) {}
        };

        Node *root;     ///< Pointer to the root node of the tree
        Node *end_node; ///< Sentinel node representing the end of the tree
        size_t count;   ///< Number of elements in the set
        Compare comp;   ///< Comparison function object
        Allocator alloc;///< Allocator object

        /**
        * @typedef AllocTraits
        * @brief Type alias for the allocator traits
        *
        * This typedef provides a convenient way to access the properties and functions
        * of the allocator through the allocator_traits template.
        */
        using AllocTraits = std::allocator_traits<Allocator>;

        /**
        * @typedef NodeAlloc
        * @brief Type alias for the node allocator
        *
        * This typedef rebinds the original allocator to work with Node objects instead of Key objects.
        */
        using NodeAlloc = typename AllocTraits::template rebind_alloc<Node>;

        /**
        * @typedef NodeAllocTraits
        * @brief Type alias for the node allocator traits
        *
        * This typedef provides a convenient way to access the properties and functions
        * of the node allocator through the allocator_traits template.
        */
        using NodeAllocTraits = std::allocator_traits<NodeAlloc>;

        NodeAlloc node_alloc;///< Node allocator

        /**
        * @brief Performs a left rotation on the given node.
        * @param x The node to rotate.
        *
        * This function performs a left rotation around the node x,
        * which is a fundamental operation in maintaining the Red-Black tree properties.
        * Time Complexity: O(1)
        *
        * @pre x is not null and has a right child.
        * @post The tree structure is changed but still maintains binary search tree properties.
        */
        void rotateLeft(Node *x) {
            Node *y = x->right;
            x->right = y->left;
            if (y->left) y->left->parent = x;
            y->parent = x->parent;
            if (!x->parent) root = y;
            else if (x == x->parent->left)
                x->parent->left = y;
            else
                x->parent->right = y;
            y->left = x;
            x->parent = y;
        }

        /**
        * @brief Performs a right rotation on the given node.
        * @param y The node to rotate.
        *
        * This function performs a right rotation around the node y,
        * which is a fundamental operation in maintaining the Red-Black tree properties.
        * Time Complexity: O(1)
        *
        * @pre y is not null and has a left child.
        * @post The tree structure is changed but still maintains binary search tree properties.
        */
        void rotateRight(Node *y) {
            Node *x = y->left;
            y->left = x->right;
            if (x->right) x->right->parent = y;
            x->parent = y->parent;
            if (!y->parent) root = x;
            else if (y == y->parent->right)
                y->parent->right = x;
            else
                y->parent->left = x;
            x->right = y;
            y->parent = x;
        }

        /**
        * @brief Fixes the Red-Black tree properties after insertion.
        * @param z The newly inserted node.
        *
        * This function is called after a new node is inserted to ensure
        * that the Red-Black tree properties are maintained.
        * Time Complexity: O(log n), where n is the number of nodes in the tree.
        *
        * @pre z is a valid node in the tree and is colored red.
        * @post The tree satisfies all Red-Black tree properties.
        */
        void insertFixup(Node *z) {
            while (z->parent && z->parent->color == Color::Red) {
                if (z->parent == z->parent->parent->left) {
                    Node *y = z->parent->parent->right;
                    if (y && y->color == Color::Red) {
                        // Case 1: Uncle is red
                        z->parent->color = Color::Black;
                        y->color = Color::Black;
                        z->parent->parent->color = Color::Red;
                        z = z->parent->parent;
                    } else {
                        if (z == z->parent->right) {
                            // Case 2: Uncle is black and node is a right child
                            z = z->parent;
                            rotateLeft(z);
                        }
                        // Case 3: Uncle is black and node is a left child
                        z->parent->color = Color::Black;
                        z->parent->parent->color = Color::Red;
                        rotateRight(z->parent->parent);
                    }
                } else {
                    // Mirror image of above cases
                    Node *y = z->parent->parent->left;
                    if (y && y->color == Color::Red) {
                        z->parent->color = Color::Black;
                        y->color = Color::Black;
                        z->parent->parent->color = Color::Red;
                        z = z->parent->parent;
                    } else {
                        if (z == z->parent->left) {
                            z = z->parent;
                            rotateRight(z);
                        }
                        z->parent->color = Color::Black;
                        z->parent->parent->color = Color::Red;
                        rotateLeft(z->parent->parent);
                    }
                }
            }
            root->color = Color::Black;
        }

        /**
        * @brief Fixes the Red-Black tree properties after deletion.
        * @param x The node that replaced the deleted node.
        * @param xParent The parent of x.
        *
        * This function is called after a node is deleted to ensure
        * that the Red-Black tree properties are maintained.
        * Time Complexity: O(log n), where n is the number of nodes in the tree.
        *
        * @pre The tree satisfies Red-Black properties except possibly at node x and its path to the root.
        * @post The tree satisfies all Red-Black tree properties.
        */
        void eraseFixup(Node *x, Node *xParent) {
            while (x != root && (!x || x->color == Color::Black)) {
                if (x == xParent->left) {
                    Node *w = xParent->right;
                    if (w && w->color == Color::Red) {
                        w->color = Color::Black;
                        xParent->color = Color::Red;
                        rotateLeft(xParent);
                        w = xParent->right;
                    }
                    if (w && (!w->left || w->left->color == Color::Black) &&
                        (!w->right || w->right->color == Color::Black)) {
                        w->color = Color::Red;
                        x = xParent;
                        xParent = x->parent;
                    } else {
                        if (w && (!w->right || w->right->color == Color::Black)) {
                            if (w->left) w->left->color = Color::Black;
                            w->color = Color::Red;
                            rotateRight(w);
                            w = xParent->right;
                        }
                        if (w) {
                            w->color = xParent->color;
                            if (w->right) w->right->color = Color::Black;
                        }
                        xParent->color = Color::Black;
                        rotateLeft(xParent);
                        x = root;
                    }
                } else {
                    // Mirror image of the above case
                    Node *w = xParent->left;
                    if (w && w->color == Color::Red) {
                        w->color = Color::Black;
                        xParent->color = Color::Red;
                        rotateRight(xParent);
                        w = xParent->left;
                    }
                    if (w && (!w->right || w->right->color == Color::Black) &&
                        (!w->left || w->left->color == Color::Black)) {
                        w->color = Color::Red;
                        x = xParent;
                        xParent = x->parent;
                    } else {
                        if (w && (!w->left || w->left->color == Color::Black)) {
                            if (w->right) w->right->color = Color::Black;
                            w->color = Color::Red;
                            rotateLeft(w);
                            w = xParent->left;
                        }
                        if (w) {
                            w->color = xParent->color;
                            if (w->left) w->left->color = Color::Black;
                        }
                        xParent->color = Color::Black;
                        rotateRight(xParent);
                        x = root;
                    }
                }
            }
            if (x) x->color = Color::Black;
        }

        /**
        * @brief Finds the minimum node in the subtree rooted at the given node.
        * @param node The root of the subtree to search.
        * @return Pointer to the node with the minimum key in the subtree.
        *
        * Time Complexity: O(log n) in the worst case, where n is the number of nodes in the subtree.
        *
        * @pre node is not null.
        * @post The returned node has no left child.
        */
        Node *minimum(Node *node) const {
            while (node->left) node = node->left;
            return node;
        }

        /**
        * @brief Finds the maximum node in the subtree rooted at the given node.
        * @param node The root of the subtree to search.
        * @return Pointer to the node with the maximum key in the subtree.
        *
        * Time Complexity: O(log n) in the worst case, where n is the number of nodes in the subtree.
        *
        * @pre node is not null.
        * @post The returned node has no right child.
        */
        Node *maximum(Node *node) const {
            while (node->right) node = node->right;
            return node;
        }

        /**
        * @brief Recursively deletes all nodes in the tree.
        * @param node The root of the subtree to delete.
        *
        * This function is used by the destructor and clear() to delete all nodes.
        * Time Complexity: O(n), where n is the number of nodes in the subtree.
        *
        * @pre None.
        * @post All nodes in the subtree rooted at node are deleted and their memory is freed.
        */
        void deleteTree(Node *node) {
            if (node && node != end_node) {
                deleteTree(node->left);
                deleteTree(node->right);
                NodeAllocTraits::destroy(node_alloc, node);
                NodeAllocTraits::deallocate(node_alloc, node, 1);
            }
        }

        /**
        * @brief Recursively copies a subtree.
        * @param node The root of the subtree to copy.
        * @param parent The parent of the new subtree root.
        * @return Pointer to the root of the new copied subtree.
        *
        * This function is used by the copy constructor and assignment operator.
        * Time Complexity: O(n), where n is the number of nodes in the subtree.
        *
        * @pre None.
        * @post A new subtree is created that is a deep copy of the original subtree.
        */
        Node *copyTree(Node *node, Node *parent) {
            if (!node) return nullptr;
            Node *newNode = NodeAllocTraits::allocate(node_alloc, 1);
            NodeAllocTraits::construct(node_alloc, newNode, node->value);
            newNode->color = node->color;
            newNode->parent = parent;
            newNode->left = copyTree(node->left, newNode);
            newNode->right = copyTree(node->right, newNode);
            return newNode;
        }

    public:
        /**
        * @class iterator
        * @brief Bidirectional iterator for the set.
        *
        * This class provides a bidirectional iterator interface for traversing the set.
        * It allows for both forward and backward traversal of the set elements.
        */
        class iterator {
        private:
            Node *node;     ///< Pointer to the current node
            const set *tree;///< Pointer to the set container

            /**
            * @brief Moves to the next node in the in-order traversal.
            * @param n The current node.
            * @return Pointer to the next node.
            *
            * Time Complexity: O(log n) in the worst case, but O(1) amortized over a complete traversal.
            *
             @pre n is a valid node in the tree.
            * @post Returns the next node in the in-order traversal, or end_node if n is the last node.
            */
            Node *increment(Node *n) const {
                if (n == tree->end_node) return n;
                if (n->right) {
                    n = n->right;
                    while (n->left) n = n->left;
                } else {
                    Node *p = n->parent;
                    while (p && n == p->right) {
                        n = p;
                        p = p->parent;
                    }
                    n = p ? p : tree->end_node;
                }
                return n;
            }

            /**
            * @brief Moves to the previous node in the in-order traversal.
            * @param n The current node.
            * @return Pointer to the previous node.
            *
            * Time Complexity: O(log n) in the worst case, but O(1) amortized over a complete traversal.
            *
            * @pre n is a valid node in the tree.
            * @post Returns the previous node in the in-order traversal, or the maximum node if n is end_node.
            */
            Node *decrement(Node *n) const {
                if (n == tree->end_node)
                    return tree->root ? tree->maximum(tree->root) : n;
                if (n->left) {
                    n = n->left;
                    while (n->right) n = n->right;
                } else {
                    Node *p = n->parent;
                    while (p && n == p->left) {
                        n = p;
                        p = p->parent;
                    }
                    n = p ? p : tree->end_node;
                }
                return n;
            }

        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = const Key;
            using difference_type = std::ptrdiff_t;
            using pointer = const Key *;
            using reference = const Key &;

            /**
            * @brief Constructs an iterator.
            * @param n Pointer to the node.
            * @param t Pointer to the set.
            *
            * Time Complexity: O(1)
            */
            iterator(Node *n, const set *t)
                : node(n), tree(t) {}

            /**
            * @brief Dereference operator.
            * @return A const reference to the value of the current node.
            *
            * Time Complexity: O(1)
            *
            * @pre The iterator is not pointing to end_node.
            */
            const Key &operator*() const { return node->value; }

            /**
            * @brief Arrow operator.
            * @return A const pointer to the value of the current node.
            *
            * Time Complexity: O(1)
            *
            * @pre The iterator is not pointing to end_node.
            */
            const Key *operator->() const { return &node->value; }

            /**
            * @brief Pre-increment operator.
            * @return Reference to the incremented iterator.
            *
            * Time Complexity: O(log n) in the worst case, but O(1) amortized.
            */
            iterator &operator++() {
                node = increment(node);
                return *this;
            }

            /**
            * @brief Post-increment operator.
            * @return The iterator before incrementing.
            *
            * Time Complexity: O(log n) in the worst case, but O(1) amortized.
            */
            iterator operator++(int) {
                iterator tmp = *this;
                node = increment(node);
                return tmp;
            }

            /**
            * @brief Pre-decrement operator.
            * @return Reference to the decremented iterator.
            *
            * Time Complexity: O(log n) in the worst case, but O(1) amortized.
            */
            iterator &operator--() {
                node = decrement(node);
                return *this;
            }

            /**
            * @brief Post-decrement operator.
            * @return The iterator before decrementing.
            *
            * Time Complexity: O(log n) in the worst case, but O(1) amortized.
            */
            iterator operator--(int) {
                iterator tmp = *this;
                node = decrement(node);
                return tmp;
            }

            /**
            * @brief Equality comparison operator.
            * @param other The iterator to compare with.
            * @return True if the iterators point to the same node, false otherwise.
            *
            * Time Complexity: O(1)
            */
            bool operator==(const iterator &other) const { return node == other.node; }

            /**
            * @brief Inequality comparison operator.
            * @param other The iterator to compare with.
            * @return True if the iterators point to different nodes, false otherwise.
            *
            * Time Complexity: O(1)
            */
            bool operator!=(const iterator &other) const { return node != other.node; }

            friend class set;
        };

        /**
        * @brief Default constructor.
        *
        * Constructs an empty set.
        * Time Complexity: O(1)
        *
        * @post An empty set is created with a sentinel end_node.
        */
        set()
            : root(nullptr), count(0) {
            end_node = NodeAllocTraits::allocate(node_alloc, 1);
            NodeAllocTraits::construct(node_alloc, end_node, Key());
            end_node->color = Color::Black;
        }

        /**
        * @brief Copy constructor.
        * @param other The set to copy from.
        *
        * Constructs a new set with a copy of the contents of other.
        * Time Complexity: O(n), where n is the number of elements in other.
        *
        * @post A new set is created that is a deep copy of other.
        */
        set(const set &other)
            : count(other.count), comp(other.comp), alloc(other.alloc), node_alloc(other.node_alloc) {
            end_node = NodeAllocTraits::allocate(node_alloc, 1);
            NodeAllocTraits::construct(node_alloc, end_node, Key());
            end_node->color = Color::Black;
            if (other.root)
                root = copyTree(other.root, nullptr);
            else
                root = nullptr;
        }

        /**
        * @brief Assignment operator.
        * @param other The set to assign from.
        * @return Reference to *this.
        *
        * Replaces the contents with a copy of the contents of other.
        * Time Complexity: O(n), where n is the number of elements in other.
        *
        * @post The set becomes a deep copy of other.
        */
        set &operator=(const set &other) {
            if (this != &other) {
                clear();
                comp = other.comp;
                alloc = other.alloc;
                node_alloc = other.node_alloc;
                if (other.root)
                    root = copyTree(other.root, nullptr);
                count = other.count;
            }
            return *this;
        }

        /**
        * @brief Destructor.
        *
        * Destroys the set object and frees all allocated memory.
        * Time Complexity: O(n), where n is the number of elements in the set.
        *
        * @post All nodes are deallocated and the set becomes empty.
        */
        ~set() {
            clear();
            NodeAllocTraits::destroy(node_alloc, end_node);
            NodeAllocTraits::deallocate(node_alloc, end_node, 1);
        }

        /**
        * @brief Removes all elements from the set.
        *
        * Erases all elements from the container. After this call, size() returns zero.
        * Time Complexity: O(n), where n is the number of elements in the set.
        *
        * @post The set becomes empty.
        */
        void clear() {
            deleteTree(root);
            root = nullptr;
            count = 0;
        }

        /**
        * @brief Returns the number of elements in the set.
        * @return The number of elements in the container.
        *
        * Time Complexity: O(1)
        */
        size_t size() const {
            return count;
        }

        /**
        * @brief Checks if the container has no elements.
        * @return true if the container is empty, false otherwise.
        *
        * Time Complexity: O(1)
        */
        bool empty() const {
            return count == 0;
        }

        /**
        * @brief Inserts an element into the set.
        * @param value The value of the element to insert.
        * @return A pair consisting of an iterator to the inserted element (or to the element that prevented the insertion) and 
                  a bool denoting whether the insertion took place.
        *
        * Time Complexity: O(log n), where n is the number of elements in the set.
        *
        * @post If insertion occurred, the set contains the new element and is still a valid Red-Black tree.
        */
        std::pair<iterator, bool> insert(const Key &value) {
            Node **current = &root;
            Node *parent = nullptr;

            while (*current && *current != end_node) {
                parent = *current;
                if (comp(value, (*current)->value))
                    current = &((*current)->left);
                else if (comp((*current)->value, value))
                    current = &((*current)->right);
                else
                    return {iterator(*current, this), false};// Element already exists
            }

            Node *newNode = NodeAllocTraits::allocate(node_alloc, 1);
            NodeAllocTraits::construct(node_alloc, newNode, value);
            newNode->parent = parent;
            newNode->left = newNode->right = nullptr;
            newNode->color = Color::Red;

            if (!parent)
                root = newNode;
            else
                *current = newNode;

            insertFixup(newNode);
            ++count;
            return {iterator(newNode, this), true};
        }

        /**
        * @brief Finds an element with specific key.
        * @param value The value to search for.
        * @return Iterator to an element with key equivalent to value. If no such element is found, past-the-end (end()) iterator is returned.
        *
        * Time Complexity: O(log n), where n is the number of elements in the set.
        */
        iterator find(const Key &value) const {
            Node *current = root;
            while (current && current != end_node) {
                if (comp(value, current->value))
                    current = current->left;
                else if (comp(current->value, value))
                    current = current->right;
                else
                    return iterator(current, this);// Found
            }
            return end();// Not found
        }

        /**
        * @brief Removes an element from the set.
        * @param value The value of the element to remove.
        * @return The number of elements removed (0 or 1).
        *
        * Time Complexity: O(log n), where n is the number of elements in the set.
        *
        * @post If the element was found and erased, the set size is reduced by 1 and the set is still a valid Red-Black tree.
        */
        size_t erase(const Key &value) {
            Node *nodeToDelete = root;
            while (nodeToDelete && nodeToDelete != end_node) {
                if (comp(value, nodeToDelete->value))
                    nodeToDelete = nodeToDelete->left;
                else if (comp(nodeToDelete->value, value))
                    nodeToDelete = nodeToDelete->right;
                else
                    break;// Found the node to delete
            }

            if (!nodeToDelete || nodeToDelete == end_node)
                return 0;// Element not found

            Node *y = nodeToDelete;
            Node *x = nullptr;
            Color originalColor = y->color;

            if (!nodeToDelete->left) {
                x = nodeToDelete->right;
                transplant(nodeToDelete, nodeToDelete->right);
            } else if (!nodeToDelete->right) {
                x = nodeToDelete->left;
                transplant(nodeToDelete, nodeToDelete->left);
            } else {
                y = minimum(nodeToDelete->right);
                originalColor = y->color;
                x = y->right;
                if (y->parent == nodeToDelete) {
                    if (x) x->parent = y;
                } else {
                    transplant(y, y->right);
                    y->right = nodeToDelete->right;
                    if (y->right) y->right->parent = y;
                }
                transplant(nodeToDelete, y);
                y->left = nodeToDelete->left;
                y->left->parent = y;
                y->color = nodeToDelete->color;
            }

            Node *xParent = x ? x->parent : (y->parent == nodeToDelete ? y : y->parent);
            NodeAllocTraits::destroy(node_alloc, nodeToDelete);
            NodeAllocTraits::deallocate(node_alloc, nodeToDelete, 1);

            if (originalColor == Color::Black)
                eraseFixup(x, xParent);

            --count;
            return 1;
        }

        /**
        * @brief Returns an iterator to the beginning of the set.
        * @return Iterator to the first element.
        *
        * Time Complexity: O(log n), where n is the number of elements in the set.
        */
        iterator begin() const {
            return iterator(root ? minimum(root) : end_node, this);
        }

        /**
        * @brief Returns an iterator to the end of the set.
        * @return Iterator to the element following the last element.
        *
        * Time Complexity: O(1)
        */
        iterator end() const {
            return iterator(end_node, this);
        }

        /**
        * @typedef reverse_iterator
        * @brief Type alias for the reverse iterator of the set.
        */
        using reverse_iterator = std::reverse_iterator<iterator>;

        /**
        * @brief Returns a reverse iterator to the beginning of the reversed set.
        * @return Reverse iterator to the last element.
        *
        * Time Complexity: O(1)
        */
        reverse_iterator rbegin() const {
            return reverse_iterator(end());
        }

        /**
        * @brief Returns a reverse iterator to the end of the reversed set.
        * @return Reverse iterator to the element before the first element.
        *
        * Time Complexity: O(log n), where n is the number of elements in the set.
        */
        reverse_iterator rend() const {
            return reverse_iterator(begin());
        }

    private:
        /**
        * @brief Replaces one subtree as a child of its parent with another subtree.
        * @param u The node to be replaced.
        * @param v The node that will replace u.
        *
        * This function is a helper for deletion operation. It replaces the subtree rooted at node u
        * with the subtree rooted at node v.
        *
        * Time Complexity: O(1)
        *
        * @pre u is not null and is a node in the tree.
        * @post The parent of u now points to v instead of u. If v is not null, its parent is updated to u's parent.
        */
        void transplant(Node *u, Node *v) {
            if (!u->parent)
                root = v;
            else if (u == u->parent->left)
                u->parent->left = v;
            else
                u->parent->right = v;
            if (v) v->parent = u->parent;
        }
    };

}// namespace userDefineDataStructure