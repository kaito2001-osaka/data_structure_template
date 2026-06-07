//This is template of circular linked list.
//This version uses smart pointer.
//
//Ownership model:
//  head (shared_ptr) owns the linear chain front→...→rear.
//  rear (weak_ptr) is a non-owning handle to the last node, giving O(1) append.
//  weak_ptr is the right choice for rear because the chain from head already
//  owns that node; a second owning pointer would be redundant and misleading.

#include <memory>

using std::shared_ptr;
using std::weak_ptr;

template <typename TYPE>
class Node
{
    public:
        Node();
        Node(const TYPE & source);
        Node(const Node & source);
        shared_ptr<Node<TYPE>> & get_next();
        TYPE get_data() const;
        int display() const;
        bool operator==(const TYPE & to_comp);
    private:
        TYPE data;
        shared_ptr<Node<TYPE>> next;
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
        shared_ptr<Node<TYPE>> head;
        weak_ptr<Node<TYPE>> rear;
        int clear(shared_ptr<Node<TYPE>> & current);
        int copy(shared_ptr<Node<TYPE>> current);
        int display(shared_ptr<Node<TYPE>> current) const;
        int remove(shared_ptr<Node<TYPE>> & current, shared_ptr<Node<TYPE>> prev, const TYPE & target_data);
        TYPE retrieve(shared_ptr<Node<TYPE>> current, const TYPE & target_data) const;
};

#include "CLL_smart.tpp"
