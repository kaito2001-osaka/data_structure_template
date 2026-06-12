//This is template of binary search tree.
//This version uses smart pointer.
//
//Ownership model:
//  left / right (unique_ptr) own the child subtrees exclusively. A binary
//  search tree is a pure ownership hierarchy with no back-links, so unique_ptr
//  is the natural fit — each subtree is freed automatically when its owning
//  pointer is reset, and no node can have two owners.

#include <memory>

using std::unique_ptr;

template <typename TYPE>
class Node
{
    public:
        Node();
        Node(const TYPE & source);
        Node(const Node & source);
        unique_ptr<Node<TYPE>> & get_left();
        unique_ptr<Node<TYPE>> & get_right();
        bool greater_than(const TYPE & data) const;
        bool equal_to(const TYPE & data) const;
        TYPE get_data() const;
        int set_data(const TYPE & new_data);
        int display() const;

    private:
        TYPE data;
        unique_ptr<Node<TYPE>> left;
        unique_ptr<Node<TYPE>> right;
};


template <typename TYPE>
class BST
{
    public:
        BST();
        BST(const BST & source);
        ~BST();
        BST<TYPE> & operator=(const BST<TYPE> & source);
        int insert(const TYPE & new_data);
        int remove(const TYPE & target_data);
        int display_in_order() const;
        TYPE retrieve(const TYPE & target_data) const;

    private:
        unique_ptr<Node<TYPE>> root;

        int insert(unique_ptr<Node<TYPE>> & root, const TYPE & new_data);
        int remove(unique_ptr<Node<TYPE>> & root, const TYPE & target_data);
        int clear(unique_ptr<Node<TYPE>> & root);
        int copy(unique_ptr<Node<TYPE>> & dest, const unique_ptr<Node<TYPE>> & source);
        int display_in_order(const unique_ptr<Node<TYPE>> & root) const;
        TYPE retrieve(const unique_ptr<Node<TYPE>> & root, const TYPE & target_data) const;
        unique_ptr<Node<TYPE>> detach_ios(unique_ptr<Node<TYPE>> & root);
};

#include "BST_smart.tpp"
