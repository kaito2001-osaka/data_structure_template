# Data Structure Templates in C++

These are C++ template implementations of common data structures. This repository was created to compile the results of my college studies about data structures and to familiarize with git and GitHub.
Each structure has two versions. One uses raw pointers, and the other uses smart pointers to compare the memory overhead associated with smart pointers.

All public methods return `int` instead of `void`. Functions that succeed return `true` (1) and functions that fail return `false` (0). This is a rule of abstract data type. Internal logic is all recursive.

---

## Directory Structure

```
data_structure_template/
├── LLL/           Linear Linked List (raw pointer)
├── LLL_smart/     Linear Linked List (unique_ptr)
├── DLL/           Doubly Linked List (raw pointer)
├── DLL_smart/     Doubly Linked List (shared_ptr + weak_ptr)
├── CLL/           Circular Linked List (raw pointer)
├── CLL_smart/     Circular Linked List (shared_ptr + weak_ptr)
└── BST/           Binary Search Tree (raw pointer)
```

Each folder has a `.h` header, a `.tpp` implementation file.

---

## Linear Linked List — LLL / LLL_smart

A singly linked list where each node holds data and a pointer to the next node. Insertion appends to the tail (walks recursively to the end), and removal unlinks the first matching node it finds.

**LLL** uses raw `Node*` pointers. The class owns `head` and is responsible for deleting every node in the destructor via `clear()`.

**LLL_smart** uses `unique_ptr<Node>` for `head` and each `next`. Because `unique_ptr` has exclusive ownership, memory is cleaned up automatically when the pointer goes out of scope — no manual `delete` needed. The tradeoff is that you can't copy a `unique_ptr`, so copy operations have to explicitly walk the source chain and build a new one.

One thing I noticed: when clearing a long list recursively, the destructor of each node triggers the destructor of the next one, which could cause a stack overflow on a very long list. It should be kept in mind.

---

## Doubly Linked List — DLL / DLL_smart

Same as LLL but each node also has a `previous` pointer, so you can traverse in both directions. The class tracks both `head` and `tail`.

**DLL** uses raw pointers for both `next` and `previous`.

**DLL_smart** uses `shared_ptr<Node>` for `next` (forward direction) and `weak_ptr<Node>` for `previous` (backward direction). If both directions were owning pointers, every node would keep its neighbor alive and the reference counts would never reach zero — nothing would get freed. By making `previous` a `weak_ptr`, ownership only flows forward through `next`, so nodes are freed as soon as the forward chain lets go of them. `tail` in the DLL class is also a `weak_ptr` for the same reason.

One thing worth noting: `weak_ptr` can only observe objects that are already managed by a `shared_ptr`. This is why `next` had to be `shared_ptr` rather than `unique_ptr` — you can't create a `weak_ptr` to a `unique_ptr`-managed object. To read the node a `weak_ptr` is observing, you call `.lock()`, which returns a temporary `shared_ptr` if the node still exists, or null if it has already been freed.

---

## Circular Linked List — CLL / CLL_smart

A linked list where the last node's `next` points back to the first node instead of `nullptr`. This makes it possible to keep cycling through the list without hitting a null terminator.

The key design choice here is using a `rear` pointer (pointing to the last node) instead of a `head` pointer. Since `rear->next` is always the front, you get O(1) access to both ends of the list without needing two separate pointers. Inserting at the tail is just a matter of updating `rear->next` and `rear` — no traversal needed.

**CLL** stores `rear` as a raw `Node*`. Breaking the circle before deleting is important; if you try to walk a circular list without breaking it first, the traversal never terminates.

**CLL_smart** is where things get more interesting. You can't use `unique_ptr` here at all — if the last node's `next` pointed back to a node that's already owned by another `unique_ptr`, you'd have two owners, which `unique_ptr` doesn't allow. Even `shared_ptr` alone would create a reference cycle: each node keeps the next one alive, and the circle means nobody's reference count ever reaches zero, so nothing gets freed.

The solution is to use `shared_ptr` for `head` (which owns the chain linearly) and `weak_ptr` for `rear`. A `weak_ptr` doesn't participate in ownership — it's just a way to observe a node that's already owned by something else. To use it, you call `rear.lock()` which gives you a temporary `shared_ptr` if the node still exists. When the list is cleared, only the `shared_ptr` chain matters for destruction, so there's no cycle.

---

## Binary Search Tree — BST

> **🚧 Under construction 🚧**

---

## Notes

- Templates are implemented in `.tpp` files and `#include`d at the bottom of `.h` files. This is one way to handle the template instantiation problem in C++ (the compiler needs to see both the declaration and the definition when it instantiates a template).
- The `Node` class in each version overloads `operator==` so that `*node == data` works cleanly instead of needing a separate comparison method.
