//This is template implementation file.
#include <iostream>
using std::make_unique;

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

//Returns reference to left child unique_ptr
template <typename TYPE>
unique_ptr<Node<TYPE>> & Node<TYPE>::get_left()
{
    return left;
}

//Returns reference to right child unique_ptr
template <typename TYPE>
unique_ptr<Node<TYPE>> & Node<TYPE>::get_right()
{
    return right;
}

//Returns true if this node's data is greater than the argument
template <typename TYPE>
bool Node<TYPE>::greater_than(const TYPE & data) const
{
    return this->data > data;
}

//Returns true if this node's data equals the argument
template <typename TYPE>
bool Node<TYPE>::equal_to(const TYPE & data) const
{
    return this->data == data;
}

//Returns the data stored in this node
template <typename TYPE>
TYPE Node<TYPE>::get_data() const
{
    return data;
}

//Displays the data stored in this node
template <typename TYPE>
int Node<TYPE>::display() const
{
    std::cout << '\n' << data << '\n';
    return true;
}

//==================== template class Node end ====================



//==================== template class BST begin ====================

//Default constructor
template <typename TYPE>
BST<TYPE>::BST() : root(nullptr) {}

//Copy constructor
template <typename TYPE>
BST<TYPE>::BST(const BST<TYPE> & source) : root(nullptr)
{
    copy(root, source.root);
}

//Destructor
template <typename TYPE>
BST<TYPE>::~BST()
{
    clear(root);
}

//Assignment operator
template <typename TYPE>
BST<TYPE> & BST<TYPE>::operator=(const BST<TYPE> & source)
{
    if (this != &source)
    {
        clear(root);
        root = nullptr;
        copy(root, source.root);
    }

    return *this;
}

//Public insert — delegates to private recursive insert
template <typename TYPE>
int BST<TYPE>::insert(const TYPE & new_data)
{
    return insert(root, new_data);
}

//Public remove — delegates to private recursive remove
template <typename TYPE>
int BST<TYPE>::remove(const TYPE & target_data)
{
    return remove(root, target_data);
}

//Public display_in_order — delegates to private recursive display_in_order
template <typename TYPE>
int BST<TYPE>::display_in_order() const
{
    if (!root)
        return false;

    return display_in_order(root);
}

//Public retrieve — delegates to private recursive retrieve
template <typename TYPE>
TYPE BST<TYPE>::retrieve(const TYPE & target_data) const
{
    if (!root)
        throw "There is no node...";

    return retrieve(root, target_data);
}

//Recursively inserts new_data in BST order.
//Equal values go right (duplicates allowed).
template <typename TYPE>
int BST<TYPE>::insert(unique_ptr<Node<TYPE>> & root, const TYPE & new_data)
{
    if (!root)
    {
        root = make_unique<Node<TYPE>>(new_data);
        return true;
    }

    if (root->greater_than(new_data))
        return insert(root->get_left(), new_data);

    return insert(root->get_right(), new_data);
}

//Recursively searches for target_data and removes the matching node.
//Three cases:
//  Leaf            — reset the owning pointer.
//  One child       — replace the node with its only child (move the subtree up).
//  Two children    — detach the in-order successor and move it into this node's
//                    position, inheriting both subtrees.
template <typename TYPE>
int BST<TYPE>::remove(unique_ptr<Node<TYPE>> & root, const TYPE & target_data)
{
    if (!root)
        return false;

    if (root->greater_than(target_data))
        return remove(root->get_left(), target_data);

    if (!root->equal_to(target_data))
        return remove(root->get_right(), target_data);

    //Leaf node
    if (!root->get_left() && !root->get_right())
    {
        root.reset();
        return true;
    }

    //Only right child — move it up; the old node is freed on assignment
    if (!root->get_left())
    {
        root = std::move(root->get_right());
        return true;
    }

    //Only left child — move it up; the old node is freed on assignment
    if (!root->get_right())
    {
        root = std::move(root->get_left());
        return true;
    }

    //Two children: detach the IOS node and move it into this node's position.
    //No data copy needed — the IOS node itself takes over, inheriting both subtrees.
    unique_ptr<Node<TYPE>> ios = detach_ios(root->get_right());
    ios->get_left()  = std::move(root->get_left());   //IOS inherits deleted node's left subtree
    ios->get_right() = std::move(root->get_right());  //IOS inherits remaining right subtree
    root = std::move(ios);                            //old node (now child-less) is freed

    return true;
}

//Recursively releases every node post-order (left, right, then root)
template <typename TYPE>
int BST<TYPE>::clear(unique_ptr<Node<TYPE>> & root)
{
    if (!root)
        return false;

    //Leaf node
    if (!root->get_left() && !root->get_right())
    {
        root.reset();
        return true;
    }

    clear(root->get_left());
    clear(root->get_right());

    root.reset();

    return true;
}

//Recursively deep-copies source pre-order (root, left, right), preserving BST structure
template <typename TYPE>
int BST<TYPE>::copy(unique_ptr<Node<TYPE>> & dest, const unique_ptr<Node<TYPE>> & source)
{
    if (!source)
    {
        dest = nullptr;
        return true;
    }

    dest = make_unique<Node<TYPE>>(source->get_data());
    copy(dest->get_left(),  source->get_left());
    return copy(dest->get_right(), source->get_right());
}

//Recursively displays all nodes in ascending order (left → root → right)
template <typename TYPE>
int BST<TYPE>::display_in_order(const unique_ptr<Node<TYPE>> & root) const
{
    if (!root)
        return false;

    //Leaf node
    if (!root->get_left() && !root->get_right())
    {
        root->display();
        return true;
    }

    return display_in_order(root->get_left()) + root->display() + display_in_order(root->get_right());
}

//Recursively searches for target_data using BST ordering; throws if not found
template <typename TYPE>
TYPE BST<TYPE>::retrieve(const unique_ptr<Node<TYPE>> & root, const TYPE & target_data) const
{
    if (!root)
        throw "There is no matching data...";

    if (root->equal_to(target_data))
        return root->get_data();

    if (root->greater_than(target_data))
        return retrieve(root->get_left(), target_data);

    return retrieve(root->get_right(), target_data);
}

//Recursively finds and detaches the IOS (leftmost node of a subtree).
//The IOS's right child is connected to its parent so the subtree stays valid.
//Returns the detached IOS node (its right pointer nulled, left already null).
template <typename TYPE>
unique_ptr<Node<TYPE>> BST<TYPE>::detach_ios(unique_ptr<Node<TYPE>> & root)
{
    //IOS found: this node has no left child
    if (!root->get_left())
    {
        unique_ptr<Node<TYPE>> ios = std::move(root);   //take ownership of the IOS node
        root = std::move(ios->get_right());             //parent now links to IOS's right child
        return ios;                                     //ios->get_right() is now null
    }

    return detach_ios(root->get_left());
}

//==================== template class BST end ====================
