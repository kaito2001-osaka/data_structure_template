//This is template implementation file.
#include <iostream>

//==================== template class Node begin ====================

//Constructor
template <typename TYPE>
Node<TYPE>::Node():next(nullptr), previous(nullptr)  {}

//Constructor with data
template <typename TYPE>
Node<TYPE>::Node(const TYPE & source):data(source), next(nullptr), previous(nullptr)  {}

//Constructor with node
template <typename TYPE>
Node<TYPE>::Node(const Node<TYPE> & source):data(source.data), next(nullptr), previous(nullptr)  {}

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

//Returns mutable reference to previous pointer
template <typename TYPE>
Node<TYPE> * & Node<TYPE>::get_previous()
{
    return previous;
}

//Returns read-only pointer to previous node (const overload)
template <typename TYPE>
Node<TYPE> * Node<TYPE>::get_previous() const
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
bool Node<TYPE>::operator==(const TYPE & to_comp) const
{
    return data == to_comp;
}

//==================== template class Node end ====================



//==================== template class DLL begin ====================

//Default constructor
template <typename TYPE>
DLL<TYPE>::DLL():head(nullptr), tail(nullptr)  {}

//Copy constructor
template <typename TYPE>
DLL<TYPE>::DLL(const DLL<TYPE> & source):head(nullptr), tail(nullptr)
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
        tail = nullptr;
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

//Appends a new node at the tail in O(1) using the tail pointer
template <typename TYPE>
int DLL<TYPE>::insert(const TYPE & new_data)
{
    Node<TYPE> * new_node = new Node<TYPE>(new_data);

    if (!head)                              // empty list
    {
        head = new_node;
    }
    else
    {
        new_node->get_previous() = tail;    // new node <- old tail
        tail->get_next() = new_node;        // old tail -> new node
    }

    tail = new_node;                        // advance tail

    return true;
}

//Recursively deletes every node from head to tail
template <typename TYPE>
int DLL<TYPE>::clear(Node<TYPE> * head)
{
    if (!head)
    {
        return true;
    }

    clear(head->get_next());

    delete head;
    head = tail = nullptr;

    return true;
}

//Recursively displays each node then advances to the next
template <typename TYPE>
int DLL<TYPE>::display(Node<TYPE> * head) const
{
    if (!head)
    {
        return true;
    }

    return head->display() + display(head->get_next());
}

//Public remove — delegates to private recursive remove
template <typename TYPE>
int DLL<TYPE>::remove(const TYPE & target_data)
{
    return remove(head, target_data);
}

//Recursively searches for data; unlinks and deletes the matching node, fixing the neighbouring previous pointer
template <typename TYPE>
int DLL<TYPE>::remove(Node<TYPE> * & head, const TYPE & target_data)
{
    //Base case: No node
    if (!head)
    {
       return false;
    }

    //Data
    if (*head == target_data)
    {
        Node<TYPE> * temp = head;              //Hold current node
        head = head->get_next();               //Replace current node with next node

        if (head)
            head->get_previous() = temp->get_previous();  //Fix next node's previous pointer
        else
            tail = temp->get_previous();       //Removed node was the tail

        delete temp;
        temp = nullptr;

        return true;
    }

    return remove(head->get_next(), target_data);
}

//Recursively deep-copies source chain into dest and sets previous pointers; updates tail at the end
template <typename TYPE>
int DLL<TYPE>::copy(Node<TYPE> * & dest, Node<TYPE> * source, Node<TYPE> * prev)
{
    if (!source)
    {
        dest = nullptr;
        tail = prev;
        return true;
    }

    dest = new Node<TYPE>(source->get_data());
    dest->get_previous() = prev;

    return copy(dest->get_next(), source->get_next(), dest);
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
TYPE DLL<TYPE>::retrieve(Node<TYPE> * head, const TYPE & target_data) const
{
    //Base Case: Last Node
    if (head -> get_next() == nullptr)
    {
        if(*head == target_data)
        {
            return head -> get_data();
        }
        else
        {
            throw "There is no matching data...";
        }
    }

    if (*head == target_data)
    {
        return head -> get_data();
    }

    return retrieve(head -> get_next(), target_data);
}

//==================== template class DLL end ====================
