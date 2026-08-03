//This is template implementation file.
#include <iostream>

//==================== template class Node begin ====================

//Constructor
template <typename TYPE>
Node<TYPE>::Node() : height(1), left(nullptr), right(nullptr) {}

//Constructor with data
template <typename TYPE>
Node<TYPE>::Node(const TYPE & source) : data(source), height(1), left(nullptr), right(nullptr) {}

//Constructor with node
template <typename TYPE>
Node<TYPE>::Node(const Node<TYPE> & source) : data(source.data), height(1), left(nullptr), right(nullptr) {}

//Destructor
template <typename TYPE>
Node<TYPE>::~Node()
{
    left  = nullptr;
    right = nullptr;
}

//Returns mutable reference to left child pointer
template <typename TYPE>
Node<TYPE> * & Node<TYPE>::get_left()
{
    return left;
}

//Returns mutable reference to right child pointer
template <typename TYPE>
Node<TYPE> * & Node<TYPE>::get_right()
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

//Returns the cached height of the subtree rooted at this node
template <typename TYPE>
int Node<TYPE>::get_height() const
{
    return height;
}

//Overwrites the cached height. The tree recomputes it after every structural change.
template <typename TYPE>
int Node<TYPE>::set_height(int new_height)
{
    height = new_height;
    return true;
}

//Displays the data stored in this node
template <typename TYPE>
int Node<TYPE>::display() const
{
    std::cout << '\n' << data << '\n';
    return true;
}

//==================== template class Node end ====================



//==================== template class AVL begin ====================

//Default constructor
template <typename TYPE>
AVL<TYPE>::AVL() : root(nullptr) {}

//Copy constructor
template <typename TYPE>
AVL<TYPE>::AVL(const AVL<TYPE> & source) : root(nullptr)
{
    copy(root, source.root);
}

//Destructor
template <typename TYPE>
AVL<TYPE>::~AVL()
{
    clear(root);
}

//Assignment operator
template <typename TYPE>
AVL<TYPE> & AVL<TYPE>::operator=(const AVL<TYPE> & source)
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
int AVL<TYPE>::insert(const TYPE & new_data)
{
    return insert(root, new_data);
}

//Public remove — delegates to private recursive remove
template <typename TYPE>
int AVL<TYPE>::remove(const TYPE & target_data)
{
    return remove(root, target_data);
}

//Public display_in_order — delegates to private recursive display_in_order
template <typename TYPE>
int AVL<TYPE>::display_in_order() const
{
    if (!root)
        return false;

    return display_in_order(root);
}

//Public retrieve — delegates to private recursive retrieve
template <typename TYPE>
TYPE AVL<TYPE>::retrieve(const TYPE & target_data) const
{
    if (!root)
        throw "There is no node...";

    return retrieve(root, target_data);
}

//Returns the height of the whole tree (0 when empty, 1 for a single node)
template <typename TYPE>
int AVL<TYPE>::get_height() const
{
    return get_height(root);
}

//Public is_balanced — delegates to private recursive is_balanced.
//An empty tree is balanced by definition.
template <typename TYPE>
int AVL<TYPE>::is_balanced() const
{
    return is_balanced(root);
}

//Recursively inserts new_data in BST order, then rebalances on the way back up.
//Equal values go right (duplicates allowed), exactly as in the BST template.
//The only difference from BST::insert is the balance() call: every node on the
//path back to the root gets its height refreshed and is rotated if it went out
//of balance, so the tree height stays O(log n).
template <typename TYPE>
int AVL<TYPE>::insert(Node<TYPE> * & root, const TYPE & new_data)
{
    if (!root)
    {
        root = new Node<TYPE>(new_data);
        return true;
    }

    int result;

    if (root->greater_than(new_data))
        result = insert(root->get_left(), new_data);
    else
        result = insert(root->get_right(), new_data);

    balance(root);

    return result;
}

//Recursively searches for target_data and removes the matching node.
//Three cases, unchanged from BST:
//  Leaf            — delete directly.
//  One child       — splice the node out and connect parent to its only child.
//  Two children    — the in-order successor is detached and takes over the
//                    removed node's position, inheriting both subtrees.
//What AVL adds is a balance() call after every case, so the shrunken subtree is
//rotated back into shape before the caller sees it.
template <typename TYPE>
int AVL<TYPE>::remove(Node<TYPE> * & root, const TYPE & target_data)
{
    if (!root)
        return false;

    if (root->greater_than(target_data))
    {
        int result = remove(root->get_left(), target_data);
        balance(root);

        return result;
    }

    if (!root->equal_to(target_data))
    {
        int result = remove(root->get_right(), target_data);
        balance(root);

        return result;
    }

    //Leaf node
    if (!root->get_left() && !root->get_right())
    {
        delete root;
        root = nullptr;

        return true;
    }

    //Only right child
    if (!root->get_left())
    {
        Node<TYPE> * temp = root;
        root = root->get_right();
        temp->get_right() = nullptr;  //Unlink before delete
        delete temp;
        temp = nullptr;

        balance(root);

        return true;
    }

    //Only left child
    if (!root->get_right())
    {
        Node<TYPE> * temp = root;
        root = root->get_left();
        temp->get_left() = nullptr;   //Unlink before delete
        delete temp;
        temp = nullptr;

        balance(root);

        return true;
    }

    //Two children: detach the IOS node and move it into this node's position.
    //No data copy needed — the IOS node itself takes over, inheriting both subtrees.
    Node<TYPE> * ios = detach_ios(root->get_right());
    ios->get_left() = root->get_left();   //IOS inherits deleted node's left subtree
    ios->get_right() = root->get_right();  //IOS inherits deleted node's right subtree
                                            //(already updated and rebalanced by detach_ios)

    root->get_left() = nullptr;           //Unlink before delete
    root->get_right() = nullptr;           //Unlink before delete

    delete root;
    root = ios;

    //The IOS arrives with a stale height and two inherited subtrees, so this
    //balance() both refreshes it and fixes the imbalance the removal caused.
    balance(root);

    return true;
}

//Recursively deletes every node post-order (left, right, then root)
template <typename TYPE>
int AVL<TYPE>::clear(Node<TYPE> * & root)
{
    if (!root)
        return false;

    //Leaf node
    if (!root->get_left() && !root->get_right())
    {
        delete root;
        root = nullptr;
        return true;
    }

    clear(root->get_left());
    clear(root->get_right());

    delete root;
    root = nullptr;

    return true;
}

//Recursively deep-copies source pre-order (root, left, right), preserving structure.
//No rebalancing is needed — the source is already an AVL tree, so copying its shape
//copies its balance. Heights are recomputed bottom-up rather than copied.
template <typename TYPE>
int AVL<TYPE>::copy(Node<TYPE> * & dest, Node<TYPE> * source)
{
    if (!source)
    {
        dest = nullptr;
        return true;
    }

    dest = new Node<TYPE>(source->get_data());
    copy(dest->get_left(),  source->get_left());
    copy(dest->get_right(), source->get_right());

    return update_height(dest);
}

//Recursively displays all nodes in ascending order (left → root → right)
template <typename TYPE>
int AVL<TYPE>::display_in_order(Node<TYPE> * root) const
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
TYPE AVL<TYPE>::retrieve(Node<TYPE> * root, const TYPE & target_data) const
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
//Returns the detached IOS node with both pointers nulled.
//Every node on the way back up is rebalanced, since the left side just shrank.
template <typename TYPE>
Node<TYPE> * AVL<TYPE>::detach_ios(Node<TYPE> * & root)
{
    //IOS found: this node has no left child
    if (!root->get_left())
    {
        Node<TYPE> * ios = root;
        root = root->get_right();  //parent now links to IOS's right child
        ios->get_right() = nullptr;            //unlink IOS from the subtree
        return ios;
    }

    Node<TYPE> * ios = detach_ios(root->get_left());
    balance(root);

    return ios;
}

//Recursively checks the AVL invariant: every node's balance factor is -1, 0 or 1
//and every cached height matches what its children say it should be.
//This is a diagnostic used to prove the tree really is balanced, not part of the
//insert / remove path.
template <typename TYPE>
int AVL<TYPE>::is_balanced(Node<TYPE> * root) const
{
    if (!root)
        return true;

    int factor = get_balance_factor(root);

    if (factor < -1 || factor > 1)
        return false;

    int left_height  = get_height(root->get_left());
    int right_height = get_height(root->get_right());

    if (root->get_height() != (left_height > right_height ? left_height : right_height) + 1)
        return false;

    return is_balanced(root->get_left()) && is_balanced(root->get_right());
}

//Null-safe height accessor. A null child counts as height 0, which is what makes
//a leaf's height 1 and lets the balance factor be computed without null checks.
template <typename TYPE>
int AVL<TYPE>::get_height(Node<TYPE> * root) const
{
    if (!root)
        return 0;

    return root->get_height();
}

//Balance factor = left height - right height.
//Positive means left-heavy, negative means right-heavy.
//Anything outside [-1, 1] violates the AVL invariant and triggers a rotation.
template <typename TYPE>
int AVL<TYPE>::get_balance_factor(Node<TYPE> * root) const
{
    if (!root)
        return 0;

    return get_height(root->get_left()) - get_height(root->get_right());
}

//Recomputes one node's cached height from its children. Children must already
//hold correct heights, so this is always called bottom-up.
template <typename TYPE>
int AVL<TYPE>::update_height(Node<TYPE> * root)
{
    if (!root)
        return false;

    int left_height  = get_height(root->get_left());
    int right_height = get_height(root->get_right());

    return root->set_height((left_height > right_height ? left_height : right_height) + 1);
}

//Left rotation — fixes a right-heavy node.
//
//      root                  pivot
//     ╱    ╲                ╱     ╲
//    A    pivot     =>    root     C
//         ╱   ╲          ╱    ╲
//        B     C        A      B
//
//Only B changes parent, so just root and pivot need their heights refreshed,
//and root must come first because it ends up below pivot.
template <typename TYPE>
int AVL<TYPE>::rotate_left(Node<TYPE> * & root)
{
    if (!root || !root->get_right())
        return false;

    Node<TYPE> * pivot = root->get_right();

    root->get_right() = pivot->get_left();
    pivot->get_left() = root;

    update_height(root);
    update_height(pivot);

    root = pivot;

    return true;
}

//Right rotation — fixes a left-heavy node. Mirror image of rotate_left.
//
//        root                pivot
//       ╱    ╲              ╱     ╲
//    pivot    C     =>     A      root
//    ╱   ╲                        ╱    ╲
//   A     B                      B      C
template <typename TYPE>
int AVL<TYPE>::rotate_right(Node<TYPE> * & root)
{
    if (!root || !root->get_left())
        return false;

    Node<TYPE> * pivot = root->get_left();

    root->get_left() = pivot->get_right();
    pivot->get_right() = root;

    update_height(root);
    update_height(pivot);

    root = pivot;

    return true;
}

//Refreshes this node's height and rotates it back into balance if needed.
//Four cases, decided by the node's balance factor and its heavy child's:
//  LL — left-heavy, left child left-heavy or even   → rotate right
//  LR — left-heavy, left child right-heavy          → rotate left on the child, then right
//  RR — right-heavy, right child right-heavy/even   → rotate left
//  RL — right-heavy, right child left-heavy         → rotate right on the child, then left
//Called on every node along the recursion path of insert and remove.
template <typename TYPE>
int AVL<TYPE>::balance(Node<TYPE> * & root)
{
    if (!root)
        return false;

    update_height(root);

    int factor = get_balance_factor(root);

    //Left-heavy
    if (factor > 1)
    {
        if (get_balance_factor(root->get_left()) < 0)
            rotate_left(root->get_left());  //LR case: straighten the zig-zag first

        return rotate_right(root);
    }

    //Right-heavy
    if (factor < -1)
    {
        if (get_balance_factor(root->get_right()) > 0)
            rotate_right(root->get_right());  //RL case: straighten the zig-zag first

        return rotate_left(root);
    }

    //Already balanced — the height refresh above was all that was needed
    return true;
}

//==================== template class AVL end ====================
