//This is template implementation file.
#include <iostream>
using std::make_unique;

//==================== template class Node begin ====================

//Constructor
template <typename TYPE>
Node<TYPE>::Node():next(nullptr)  {}

//Constructor with data
template <typename TYPE>
Node<TYPE>::Node(const TYPE & source):data(source), next(nullptr)  {}

//Constructor with node
template <typename TYPE>
Node<TYPE>::Node(const Node<TYPE> & source):data(source.data), next(nullptr)  {}

//Returns reference to next unique_ptr
template <typename TYPE>
unique_ptr<Node<TYPE>> & Node<TYPE>::get_next()
{
    return next;
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
    std::cout << '\n' << data << std::endl;

    return true;
}
        
//This function compares the data in the node and to_comp. It returns true if data and to_comp is same, otherwise returns false.
template <typename TYPE>
bool Node<TYPE>::operator==(const TYPE & to_comp)
{
    return data == to_comp;
}

//==================== template class Node end ====================



//==================== template class LLL begin ====================

//Default constructor
template <typename TYPE>
LLL<TYPE>::LLL():head(nullptr)  {}

//Copy constructor
template <typename TYPE>
LLL<TYPE>::LLL(const LLL<TYPE> & source):head(nullptr)
{
    copy(head, source.head);
}

//Destructor
template <typename TYPE>
LLL<TYPE>::~LLL()
{
    clear(head);
}

//Assignment operator
template <typename TYPE>
LLL<TYPE> & LLL<TYPE>::operator=(const LLL<TYPE> & source)
{
    if (this != &source)
    {
        clear(head);
        head = nullptr;
        copy(head, source.head);
    }

    return *this;
}

//Public display — delegates to private recursive display
template <typename TYPE>
int LLL<TYPE>::display() const
{
    return display(head);
}

//Public insert — delegates to private recursive insert
template <typename TYPE>
int LLL<TYPE>::insert(const TYPE & new_data)
{
    return insert(head, new_data);
}

//Recursively deletes every node from head to tail
template <typename TYPE>
int LLL<TYPE>::clear(unique_ptr<Node<TYPE>> & head)
{
    if (!head)
    {
        return true;
    }

    clear(head->get_next());

    head.reset();

    return true;
}

//Recursively displays each node then advances to the next
template <typename TYPE>
int LLL<TYPE>::display(const unique_ptr<Node<TYPE>> & node) const
{
    if (!node)
    {
        return true;
    }

    return node->display() + display(node->get_next());
}

//Public remove — delegates to private recursive remove
template <typename TYPE>
int LLL<TYPE>::remove(const TYPE & target_data)
{
    return remove(head, target_data);
}

//Recursively searches for data; unlinks and deletes the matching node
template <typename TYPE>
int LLL<TYPE>::remove(unique_ptr<Node<TYPE>> & head, const TYPE & target_data)
{
    //Base case: No node
    if (!head)
    {
        return false;
    }

    //Data
    if (*head == target_data)
    {
        head = std::move(head->get_next()); //Replace current node to next node

        return true;
    }

    return remove(head->get_next(), target_data);
}

//Recursively deep-copies source chain into dest
template <typename TYPE>
int LLL<TYPE>::copy(unique_ptr<Node<TYPE>> & dest, const unique_ptr<Node<TYPE>> & source)
{
    if (!source)
    {
        dest = nullptr;
        return true;
    }

    dest = make_unique<Node<TYPE>>(source->get_data());

    return copy(dest->get_next(), source->get_next());
}

//Recursively walks to the end of the list and appends a new node
template <typename TYPE>
int LLL<TYPE>::insert(unique_ptr<Node<TYPE>> & head, const TYPE & new_data)
{
    if (!head)
    {
        head = make_unique<Node<TYPE>>(new_data);
        return true;
    }
    return insert(head->get_next(), new_data);
}

//Retrieve the data that is same as target_data in argument.
//If there is no matching data in LLL, it throws exceptions.
template <typename TYPE>
TYPE LLL<TYPE>::retrieve(const TYPE & target_data) const
{
    if (!head)
        throw "There is no node...";

    return retrieve(const_cast<unique_ptr<Node<TYPE>>&>(head), target_data);
}
//Recursive function for retrieve()
template <typename TYPE>
TYPE LLL<TYPE>::retrieve(unique_ptr<Node<TYPE>> & head, const TYPE & target_data) const
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

//==================== template class LLL end ====================
