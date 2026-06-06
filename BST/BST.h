//This is template of binary search tree.
//This version uses raw pointer.

template <typename TYPE>
class Node
{
    public:
        Node();
        Node(const TYPE & source);
        Node(const Node & source);
        ~Node();
        Node<TYPE> * & get_left();
        Node<TYPE> * & get_right();
        bool greater_than(const TYPE & data) const;
        bool equal_to(const TYPE & data) const;
        TYPE get_data() const;
        int set_data(const TYPE & new_data);
        int display() const;

    private:
        TYPE data;
        Node<TYPE> * left;
        Node<TYPE> * right;
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
        Node<TYPE> * root;

        int insert(Node<TYPE> * & root, const TYPE & new_data);
        int remove(Node<TYPE> * & root, const TYPE & target_data);
        int clear(Node<TYPE> * & root);
        int copy(Node<TYPE> * & dest, Node<TYPE> * source);
        int display_in_order(Node<TYPE> * root) const;
        TYPE retrieve(Node<TYPE> * root, const TYPE & target_data) const;
        Node<TYPE> * detach_ios(Node<TYPE> * & root);
};

#include "BST.tpp"
