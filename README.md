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
├── BST/           Binary Search Tree (raw pointer)
└── BST_smart/     Binary Search Tree (unique_ptr)
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

## Binary Search Tree — BST / BST_smart

A tree where each node has up to two children, ordered so that everything in the left subtree is smaller than the node and everything in the right subtree is greater-or-equal (equal values go right, so duplicates are allowed). This ordering is what makes `insert`, `remove`, and `retrieve` recurse down a single path instead of scanning every node.

Removal is the tricky operation and splits into three cases:
- **Leaf** — just delete it.
- **One child** — splice the node out and connect the parent straight to its only child.
- **Two children** — the node is replaced by its *in-order successor* (the leftmost node of its right subtree). `detach_ios()` walks left to find and unlink that successor, which then takes over the removed node's position and inherits both subtrees. No data is copied — the successor node itself is moved into place.

**BST** uses raw `Node*` pointers for `left` and `right`. The class owns `root` and deletes every node post-order in the destructor via `clear()`.

**BST_smart** uses `unique_ptr<Node>` for `left` and `right`. A tree is a pure ownership hierarchy with no back-links, so `unique_ptr` is the natural fit — each subtree is freed automatically when its owning pointer is reset, and no node can ever have two owners. The raw-pointer removal logic translates cleanly to moves: the one-child case becomes `node = std::move(node->child)` (the old node is freed on assignment once its child has moved up), and `detach_ios()` returns ownership of the successor as a `unique_ptr` that the caller moves into place. Because `make_unique` is used, **BST_smart requires C++14** (`-std=c++14`), whereas the other versions does not matter.

---

## Computational Complexity

The smart-pointer version of each structure has the same complexity as its raw-pointer counterpart — smart pointers change *how memory is managed*, not *how many nodes an operation touches* — so they share a row below. Each cell shows **best case / worst case**.

| Structure          | Insert          | Remove          | Retrieve        | Space |
|--------------------|-----------------|-----------------|-----------------|-------|
| LLL / LLL_smart    | O(n) / O(n)     | O(1) / O(n)     | O(1) / O(n)     | O(n)  |
| DLL / DLL_smart    | O(1) / O(1)     | O(1) / O(n)     | O(1) / O(n)     | O(n)  |
| CLL / CLL_smart    | O(1) / O(1)     | O(1) / O(n)     | O(1) / O(n)     | O(n)  |
| BST / BST_smart    | O(log n) / O(n) | O(log n) / O(n) | O(log n) / O(n) | O(n)  |

What the best / worst split means depends on the structure — for the lists it is *where the target sits*, for the BST it is *the shape of the tree*:

- **LLL** appends by recursing to the tail, so insertion always walks the whole list — O(n) in both cases. Remove and retrieve are linear searches: O(1) when the target is the head, O(n) when it is the last node or absent.
- **DLL** inserts in O(1) in every case by appending through the `tail` pointer (the smart version keeps `tail` as a `weak_ptr` for the same purpose), so no traversal is needed. Remove and retrieve scan from `head`, so they range from O(1) (head) to O(n) (tail / absent).
- **CLL** inserts in O(1) in every case: `rear->next` is always the front, so appending is just relinking `rear` with no traversal. Remove and retrieve walk the circle, from O(1) (front) to O(n) (rear / absent).
- **BST** complexity depends on tree height, so here best / worst tracks the **tree shape**: a balanced tree gives O(log n), but inserting already-sorted data degenerates the tree into a list and every operation becomes O(n). These templates do not self-balance.
- **Space** is O(n) for every structure — one node per stored element. The recursive operations also consume call-stack space proportional to the recursion depth: O(n) for the lists (and a worst-case BST), O(log n) for a balanced BST — the same depth that drives the stack-overflow risk noted above.

---

## Notes

- Templates are implemented in `.tpp` files and `#include`d at the bottom of `.h` files. This is one way to handle the template instantiation problem in C++ (the compiler needs to see both the declaration and the definition when it instantiates a template).
- In the linked-list versions the `Node` class overloads `operator==` so that `*node == data` works cleanly instead of needing a separate comparison method. The BST `Node` instead exposes `equal_to()` and `greater_than()`, since ordering a tree needs an inequality test, not just equality.
