//This is template implementation file.
#include <iostream>
using std::move;
using std::make_shared;

//==================== template class Node begin ====================

//Constructor
template <typename TYPE>
Node<TYPE>::Node() {}

//Constructor with data
template <typename TYPE>
Node<TYPE>::Node(const TYPE & source) : data(source) {}

//Constructor with node
template <typename TYPE>
Node<TYPE>::Node(const Node<TYPE> & source) : data(source.data) {}

//Returns reference to next shared_ptr
template <typename TYPE>
shared_ptr<Node<TYPE>> & Node<TYPE>::get_next()
{
    return next;
}

//Returns reference to previous weak_ptr
template <typename TYPE>
weak_ptr<Node<TYPE>> & Node<TYPE>::get_previous()
{
    return previous;
}

//This function returns the data.
template <typename TYPE>
TYPE Node<TYPE>::get_data() const
{
    return data;
}

//This function displays the data. It returns true if it succeed to display data, otherwise it returns false.
template <typename TYPE>
int Node<TYPE>::display() const
{
    std::cout << '\n' << data << '\n';

    return true;
}

//This function compares the data in the node and to_comp. It returns true if data and to_comp is same, otherwise returns false.
template <typename TYPE>
bool Node<TYPE>::operator==(const TYPE & to_comp)
{
    return data == to_comp;
}

//==================== template class Node end ====================



//==================== template class DLL begin ====================

//Default constructor
template <typename TYPE>
DLL<TYPE>::DLL() : head(nullptr) {}

//Copy constructor
template <typename TYPE>
DLL<TYPE>::DLL(const DLL<TYPE> & source) : head(nullptr)
{
    copy(head, source.head, nullptr);
}

//Destructor
template <typename TYPE>
DLL<TYPE>::~DLL()
{
    clear(head);
}

//Assignment operator
template <typename TYPE>
DLL<TYPE> & DLL<TYPE>::operator=(const DLL<TYPE> & source)
{
    if (this != &source)
    {
        clear(head);
        head = nullptr;
        tail.reset();
        copy(head, source.head, nullptr);
    }

    return *this;
}

//Public display — delegates to private recursive display
template <typename TYPE>
int DLL<TYPE>::display() const
{
    return display(head);
}

//Public insert — delegates to private recursive insert
template <typename TYPE>
int DLL<TYPE>::insert(const TYPE & new_data)
{
    return insert(head, new_data, nullptr);
}

//Recursively deletes every node from head to tail
template <typename TYPE>
int DLL<TYPE>::clear(shared_ptr<Node<TYPE>> & node)
{
    if (!node)
        return true;

    clear(node->get_next());
    node.reset();

    return true;
}

//Recursively displays each node then advances to the next
template <typename TYPE>
int DLL<TYPE>::display(const shared_ptr<Node<TYPE>> & node) const
{
    if (!node)
        return true;

    return node->display() + display(node->get_next());
}

//Public remove — delegates to private recursive remove
template <typename TYPE>
int DLL<TYPE>::remove(const TYPE & target_data)
{
    return remove(head, target_data);
}

//Recursively searches for data; unlinks and deletes the matching node, fixing the neighbouring previous pointer
template <typename TYPE>
int DLL<TYPE>::remove(shared_ptr<Node<TYPE>> & node, const TYPE & target_data)
{
    if (!node)
        return false;

    if (*node == target_data)
    {
        weak_ptr<Node<TYPE>> saved_prev = node->get_previous();  //Save back-link of deleted node

        node = move(node->get_next());                           //Destroy current node; node now points to successor

        if (node)
            node->get_previous() = saved_prev;                        //Fix successor's back-link
        else
            tail = saved_prev;                                        //Removed node was the tail

        return true;
    }

    return remove(node->get_next(), target_data);
}

//Recursively deep-copies source chain into dest and sets previous pointers; updates tail at the end
template <typename TYPE>
int DLL<TYPE>::copy(shared_ptr<Node<TYPE>> & dest, const shared_ptr<Node<TYPE>> & source, shared_ptr<Node<TYPE>> prev)
{
    if (!source)
    {
        dest = nullptr;
        tail = prev;
        return true;
    }

    dest = make_shared<Node<TYPE>>(source->get_data());
    dest->get_previous() = prev;

    return copy(dest->get_next(), source->get_next(), dest);
}

//Recursively walks to the end of the list and appends a new node, setting its previous pointer
template <typename TYPE>
int DLL<TYPE>::insert(shared_ptr<Node<TYPE>> & node, const TYPE & new_data, shared_ptr<Node<TYPE>> prev)
{
    if (!node)
    {
        node = make_shared<Node<TYPE>>(new_data);
        node->get_previous() = prev;
        tail = node;
        return true;
    }

    return insert(node->get_next(), new_data, node);
}

//Retrieve the data that is same as target_data in argument.
//If there is no matching data in DLL, it throws exceptions.
template <typename TYPE>
TYPE DLL<TYPE>::retrieve(const TYPE & target_data) const
{
    if (!head)
        throw "There is no node...";

    return retrieve(head, target_data);
}

//Recursive function for retrieve()
template <typename TYPE>
TYPE DLL<TYPE>::retrieve(const shared_ptr<Node<TYPE>> & node, const TYPE & target_data) const
{
    if (!node->get_next())
    {
        if (*node == target_data)
            return node->get_data();
        else
            throw "There is no matching data...";
    }

    if (*node == target_data)
        return node->get_data();

    return retrieve(node->get_next(), target_data);
}

//==================== template class DLL end ====================
