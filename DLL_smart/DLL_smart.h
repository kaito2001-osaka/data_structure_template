//This is template of Doubly linked list.
//This version uses smart pointer.
//
//Ownership model:
//  next     (shared_ptr) owns the forward chain head→...→tail.
//  previous (weak_ptr)   is a non-owning back-link; it observes the
//                        preceding node that is already kept alive by
//                        the forward chain — no co-ownership needed.
//  tail     (weak_ptr)   is a non-owning handle to the last node for
//                        O(1) tail updates during remove.

#include <memory>

using std::shared_ptr;
using std::weak_ptr;

template <typename TYPE>
class Node
{
    public:
        Node<TYPE>();
        Node<TYPE>(const TYPE & source);
        Node<TYPE>(const Node<TYPE> & source);
        shared_ptr<Node<TYPE>> & get_next();
        weak_ptr<Node<TYPE>>  & get_previous();
        TYPE get_data() const;
        int  display() const;
        bool operator==(const TYPE & to_comp);
    private:
        TYPE data;
        shared_ptr<Node<TYPE>> next;
        weak_ptr<Node<TYPE>>   previous;
};

template <typename TYPE>
class DLL
{
    public:
        DLL();
        DLL(const DLL & source);
        ~DLL();
        DLL<TYPE> & operator=(const DLL<TYPE> & source);
        int  insert(const TYPE & new_data);
        int  remove(const TYPE & target_data);
        int  display() const;
        TYPE retrieve(const TYPE & target_data) const;

    private:
        shared_ptr<Node<TYPE>> head;
        weak_ptr<Node<TYPE>>   tail;

        int  display(const shared_ptr<Node<TYPE>> & node) const;
        int  clear(shared_ptr<Node<TYPE>> & node);
        int  copy(shared_ptr<Node<TYPE>> & dest, const shared_ptr<Node<TYPE>> & source, shared_ptr<Node<TYPE>> prev);
        int  insert(shared_ptr<Node<TYPE>> & node, const TYPE & new_data, shared_ptr<Node<TYPE>> prev);
        int  remove(shared_ptr<Node<TYPE>> & node, const TYPE & target_data);
        TYPE retrieve(const shared_ptr<Node<TYPE>> & node, const TYPE & target_data) const;
};

#include "DLL_smart.tpp"
