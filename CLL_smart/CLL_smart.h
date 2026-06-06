//This is template of circular linked list.
//This version uses smart pointer.
//
//Ownership model:
//  head (shared_ptr) owns the linear chain front→...→rear.
//  rear (weak_ptr) is a non-owning handle to the last node, giving O(1) append.
//  weak_ptr is the right choice for rear because the chain from head already
//  owns that node; a second owning pointer would be redundant and misleading.

#include <memory>

template <typename TYPE>
class Node
{
    public:
        Node();
        Node(const TYPE & source);
        Node(const Node & source);
        std::shared_ptr<Node<TYPE>> & get_next();
        TYPE get_data() const;
        int display() const;
        bool operator==(const TYPE & to_comp);
    private:
        TYPE data;
        std::shared_ptr<Node<TYPE>> next;
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
        std::shared_ptr<Node<TYPE>> head;
        std::weak_ptr<Node<TYPE>> rear;
        int clear(std::shared_ptr<Node<TYPE>> & current);
        int copy(std::shared_ptr<Node<TYPE>> current);
        int display(std::shared_ptr<Node<TYPE>> current) const;
        int remove(std::shared_ptr<Node<TYPE>> & current, std::shared_ptr<Node<TYPE>> prev, const TYPE & target_data);
        TYPE retrieve(std::shared_ptr<Node<TYPE>> current, const TYPE & target_data) const;
};

#include "CLL_smart.tpp"
