//This is template implementation file.
#include <iostream>
using std::make_shared;

//==================== template class Node begin ====================

//Constructor
template <typename TYPE>
Node<TYPE>::Node() : next(nullptr) {}

//Constructor with data
template <typename TYPE>
Node<TYPE>::Node(const TYPE & source) : data(source), next(nullptr) {}

//Constructor with node
template <typename TYPE>
Node<TYPE>::Node(const Node<TYPE> & source) : data(source.data), next(nullptr) {}

//Returns reference to next shared_ptr
template <typename TYPE>
shared_ptr<Node<TYPE>> & Node<TYPE>::get_next()
{
    return next;
}

//This function returns the data.
template <typename TYPE>
TYPE Node<TYPE>::get_data() const
{
    return data;
}

//This function displays the data.
template <typename TYPE>
int Node<TYPE>::display() const
{
    std::cout << '\n' << data << '\n';
    return true;
}

//This function compares the data in the node and to_comp.
template <typename TYPE>
bool Node<TYPE>::operator==(const TYPE & to_comp)
{
    return data == to_comp;
}

//==================== template class Node end ====================



//==================== template class CLL begin ====================

//Default constructor
template <typename TYPE>
CLL<TYPE>::CLL() : head(nullptr), rear() {}

//Copy constructor
template <typename TYPE>
CLL<TYPE>::CLL(const CLL<TYPE> & source) : head(nullptr), rear()
{
    copy(source.head);
}

//Destructor
template <typename TYPE>
CLL<TYPE>::~CLL()
{
    clear(head);
    rear.reset();
}

//Assignment operator
template <typename TYPE>
CLL<TYPE> & CLL<TYPE>::operator=(const CLL<TYPE> & source)
{
    if (this != &source)
    {
        clear(head);
        rear.reset();
        copy(source.head);
    }
    return *this;
}

//O(1) insert at rear: new node becomes the new last node
template <typename TYPE>
int CLL<TYPE>::insert(const TYPE & new_data)
{
    auto new_node = make_shared<Node<TYPE>>(new_data);
    auto rear_node = rear.lock();

    if (!rear_node)
    {
        head = new_node;                   //first node — head takes ownership
    }
    else
    {
        rear_node->get_next() = new_node;  //old rear links forward to new node
    }

    rear = new_node;                       //weak_ptr updated to track the new rear
    return true;
}

//Public remove — delegates to private recursive remove
template <typename TYPE>
int CLL<TYPE>::remove(const TYPE & target_data)
{
    if (!head)
        return false;

    return remove(head, nullptr, target_data);
}

//Public display — delegates to private recursive display
template <typename TYPE>
int CLL<TYPE>::display() const
{
    return display(head);
}

//Public retrieve — delegates to private recursive retrieve
template <typename TYPE>
TYPE CLL<TYPE>::retrieve(const TYPE & target_data) const
{
    if (!head)
        throw "There is no node...";

    return retrieve(head, target_data);
}

//Recursively resets nodes from tail to head, releasing each one cleanly
template <typename TYPE>
int CLL<TYPE>::clear(shared_ptr<Node<TYPE>> & current)
{
    if (!current)
        return true;

    clear(current->get_next());
    current.reset();

    return true;
}

//Recursively inserts each node of the source chain in order
template <typename TYPE>
int CLL<TYPE>::copy(shared_ptr<Node<TYPE>> current)
{
    if (!current)
        return true;

    insert(current->get_data());
    return copy(current->get_next());
}

//Recursively displays each node then advances to the next
template <typename TYPE>
int CLL<TYPE>::display(shared_ptr<Node<TYPE>> current) const
{
    if (!current)
        return true;

    return current->display() + display(current->get_next());
}

//Recursively searches for target_data; unlinks the matching node and updates rear if needed.
//prev carries the preceding shared_ptr so that rear can be stepped back on rear removal.
template <typename TYPE>
int CLL<TYPE>::remove(shared_ptr<Node<TYPE>> & current,
                      shared_ptr<Node<TYPE>>   prev,
                      const TYPE & target_data)
{
    if (!current)
        return false;

    if (*current == target_data)
    {
        if (current == rear.lock())
        {
            if (prev)
                rear = prev;   //step rear back to the predecessor
            else
                rear.reset();  //removing the only node
        }

        current = current->get_next();
        return true;
    }

    return remove(current->get_next(), current, target_data);
}

//Recursively searches for target_data; throws if the end is reached without a match
template <typename TYPE>
TYPE CLL<TYPE>::retrieve(shared_ptr<Node<TYPE>> current,
                         const TYPE & target_data) const
{
    if (!current)
        throw "There is no matching data...";

    if (*current == target_data)
        return current->get_data();

    return retrieve(current->get_next(), target_data);
}

//==================== template class CLL end ====================
