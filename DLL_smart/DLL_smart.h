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

template <typename TYPE>
class Node
{
    public:
        Node<TYPE>();
        Node<TYPE>(const TYPE & source);
        Node<TYPE>(const Node<TYPE> & source);
        std::shared_ptr<Node<TYPE>> & get_next();
        std::weak_ptr<Node<TYPE>>  & get_previous();
        TYPE get_data() const;
        int  display() const;
        bool operator==(const TYPE & to_comp);
    private:
        TYPE data;
        std::shared_ptr<Node<TYPE>> next;
        std::weak_ptr<Node<TYPE>>   previous;
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
        std::shared_ptr<Node<TYPE>> head;
        std::weak_ptr<Node<TYPE>>   tail;

        int  display(const std::shared_ptr<Node<TYPE>> & node) const;
        int  clear(std::shared_ptr<Node<TYPE>> & node);
        int  copy(std::shared_ptr<Node<TYPE>> & dest, const std::shared_ptr<Node<TYPE>> & source, std::shared_ptr<Node<TYPE>> prev);
        int  insert(std::shared_ptr<Node<TYPE>> & node, const TYPE & new_data, std::shared_ptr<Node<TYPE>> prev);
        int  remove(std::shared_ptr<Node<TYPE>> & node, const TYPE & target_data);
        TYPE retrieve(const std::shared_ptr<Node<TYPE>> & node, const TYPE & target_data) const;
};

#include "DLL_smart.tpp"
