//This is template of AVL tree (self-balancing binary search tree).
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
        int get_height() const;
        int set_height(int new_height);
        int display() const;

    private:
        TYPE data;
        int height;             //1 for a leaf; 0 is reserved for a null child
        Node<TYPE> * left;
        Node<TYPE> * right;
};


template <typename TYPE>
class AVL
{
    public:
        AVL();
        AVL(const AVL & source);
        ~AVL();
        AVL<TYPE> & operator=(const AVL<TYPE> & source);
        int insert(const TYPE & new_data);
        int remove(const TYPE & target_data);
        int display_in_order() const;
        TYPE retrieve(const TYPE & target_data) const;
        int get_height() const;
        int is_balanced() const;

    private:
        Node<TYPE> * root;

        int insert(Node<TYPE> * & root, const TYPE & new_data);
        int remove(Node<TYPE> * & root, const TYPE & target_data);
        int clear(Node<TYPE> * & root);
        int copy(Node<TYPE> * & dest, Node<TYPE> * source);
        int display_in_order(Node<TYPE> * root) const;
        TYPE retrieve(Node<TYPE> * root, const TYPE & target_data) const;
        Node<TYPE> * detach_ios(Node<TYPE> * & root);
        int is_balanced(Node<TYPE> * root) const;

        //Balancing helpers — everything below is what separates AVL from BST
        int get_height(Node<TYPE> * root) const;
        int get_balance_factor(Node<TYPE> * root) const;
        int update_height(Node<TYPE> * root);
        int rotate_left(Node<TYPE> * & root);
        int rotate_right(Node<TYPE> * & root);
        int balance(Node<TYPE> * & root);
};

#include "AVL.tpp"
