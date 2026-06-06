//This is template implementation file.
#include <iostream>

//==================== template class Node begin ====================

//Constructor
template <typename TYPE>
Node<TYPE>::Node() : next(nullptr) {}

//Destructor
template <typename TYPE>
Node<TYPE>::~Node()
{
    next = nullptr;
}

//Constructor with data
template <typename TYPE>
Node<TYPE>::Node(const TYPE & source) : data(source), next(nullptr) {}

//Constructor with node
template <typename TYPE>
Node<TYPE>::Node(const Node<TYPE> & source) : data(source.data), next(nullptr) {}

//Returns mutable reference to next pointer
template <typename TYPE>
Node<TYPE> * & Node<TYPE>::get_next()
{
    return next;
}

//Returns read-only pointer to next node (const overload)
template <typename TYPE>
Node<TYPE> * Node<TYPE>::get_next() const
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
bool Node<TYPE>::operator==(const TYPE & to_comp) const
{
    return data == to_comp;
}

//==================== template class Node end ====================



//==================== template class CLL begin ====================

//Default constructor
template <typename TYPE>
CLL<TYPE>::CLL() : rear(nullptr) {}

//Copy constructor
template <typename TYPE>
CLL<TYPE>::CLL(const CLL<TYPE> & source) : rear(nullptr)
{
    copy(source.rear);
}

//Destructor
template <typename TYPE>
CLL<TYPE>::~CLL()
{
    clear();
}

//Assignment operator
template <typename TYPE>
CLL<TYPE> & CLL<TYPE>::operator=(const CLL<TYPE> & source)
{
    if (this != &source)
    {
        clear();
        copy(source.rear);
    }
    return *this;
}

//O(1) insert at rear: new node becomes the new last node
template <typename TYPE>
int CLL<TYPE>::insert(const TYPE & new_data)
{
    Node<TYPE> * new_node = new Node<TYPE>(new_data);

    if (!rear)
    {
        rear = new_node;
        rear->get_next() = rear;  // single node points to itself
    }
    else
    {
        new_node->get_next() = rear->get_next();  // new node -> front
        rear->get_next() = new_node;              // old rear -> new node
        rear = new_node;                           // advance rear
    }

    return true;
}

//Finds and removes the first node matching target_data
template <typename TYPE>
int CLL<TYPE>::remove(const TYPE & target_data)
{
    if (!rear)
        return false;

    Node<TYPE> * front = rear->get_next();

    //Single node
    if (rear == front)
    {
        if (*rear == target_data)
        {
            rear->get_next() = nullptr;
            delete rear;
            rear = nullptr;
            return true;
        }
        return false;
    }

    //Traverse: prev trails one step behind current
    Node<TYPE> * prev    = rear;
    Node<TYPE> * current = front;

    do
    {
        if (*current == target_data)
        {
            prev->get_next() = current->get_next();

            if (current == rear)
                rear = prev;

            current->get_next() = nullptr;
            delete current;
            return true;
        }
        prev    = current;
        current = current->get_next();
    }
    while (current != front);

    return false;
}

//Displays every node in order from front to rear
template <typename TYPE>
int CLL<TYPE>::display() const
{
    if (!rear)
        return true;

    Node<TYPE> * front   = rear->get_next();
    Node<TYPE> * current = front;

    do
    {
        current->display();
        current = current->get_next();
    }
    while (current != front);

    return true;
}

//Returns data matching target_data; throws if not found or list is empty
template <typename TYPE>
TYPE CLL<TYPE>::retrieve(const TYPE & target_data) const
{
    if (!rear)
        throw "There is no node...";

    Node<TYPE> * front   = rear->get_next();
    Node<TYPE> * current = front;

    do
    {
        if (*current == target_data)
            return current->get_data();
        current = current->get_next();
    }
    while (current != front);

    throw "There is no matching data...";
}

//Breaks the circle and deletes every node
template <typename TYPE>
void CLL<TYPE>::clear()
{
    if (!rear)
        return;

    Node<TYPE> * head = rear->get_next();
    rear->get_next() = nullptr;  // break circle so linear traversal terminates

    while (head)
    {
        Node<TYPE> * temp = head;
        head = head->get_next();
        delete temp;
    }

    rear = nullptr;
}

//Deep-copies a circular list rooted at source_rear by re-inserting each element
template <typename TYPE>
void CLL<TYPE>::copy(Node<TYPE> * source_rear)
{
    if (!source_rear)
        return;

    Node<TYPE> * source_front = source_rear->get_next();
    Node<TYPE> * current      = source_front;

    do
    {
        insert(current->get_data());
        current = current->get_next();
    }
    while (current != source_front);
}

//==================== template class CLL end ====================
