//This is template of Doubly linked list.
//This version uses raw pointer.

template <typename TYPE>
class Node
{
	public:
		Node<TYPE>();
		Node<TYPE>(const TYPE & source);
		Node<TYPE>(const Node<TYPE> & source);
		Node<TYPE> * & get_next();
		Node<TYPE> * get_next() const;
		Node<TYPE> * & get_previous();
		Node<TYPE> * get_previous() const;
		TYPE get_data() const;
        int display() const;
        bool operator==(const TYPE & to_comp) const;
	private:
		TYPE data;
		Node<TYPE> * next;
        Node<TYPE> * previous;
};

template <typename TYPE>
class DLL
{
	public:
		DLL();
		DLL(const DLL & source);
		~DLL();
		DLL<TYPE> & operator=(const DLL<TYPE> & source);
		int insert(const TYPE & new_data);
		int remove(const TYPE & target_data);
		int display() const;
        TYPE retrieve(const TYPE & target_data) const;
	private:
		Node<TYPE> * head;
		Node<TYPE> * tail;
		int display(Node<TYPE> * head) const;
		int clear(Node<TYPE> * head);
		int copy(Node<TYPE> * & dest, Node<TYPE> * source, Node<TYPE> * prev);
		int insert(Node<TYPE> * & head, const TYPE & new_data, Node<TYPE> * prev);
		int remove(Node<TYPE> * & head, const TYPE & target_data);
        TYPE retrieve(Node<TYPE> * head, const TYPE & target_data) const;
};

#include "DLL.tpp"
