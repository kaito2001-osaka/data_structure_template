//This is template of circular linked list.
//This version uses raw pointer.
//rear points to the last node; rear->next is the front.

template <typename TYPE>
class Node
{
    public:
        Node();
        ~Node();
        Node(const TYPE & source);
        Node(const Node & source);
        Node * & get_next();
        Node * get_next() const;
        TYPE get_data() const;
        int display() const;
        bool operator==(const TYPE & to_comp) const;
    private:
        TYPE data;
        Node * next;
};

template <typename TYPE>
class CLL
{
    public:
        CLL();
        CLL(const CLL & source);
        ~CLL();
        CLL & operator=(const CLL & source);
        int insert(const TYPE & new_data);
        int remove(const TYPE & target_data);
        int display() const;
        TYPE retrieve(const TYPE & target_data) const;
    private:
        Node<TYPE> * rear;
        void clear();
        void copy(Node<TYPE> * source_rear);
};

#include "CLL.tpp"
