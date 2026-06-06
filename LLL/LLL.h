//This is template of linear linked list.
//This version uses raw pointer.

template <typename TYPE>
class Node
{
	public:
		Node<TYPE>();
        ~Node();
		Node<TYPE>(const TYPE & source);
		Node<TYPE>(const Node<TYPE> & source);
		Node<TYPE> * & get_next();
		Node<TYPE> * get_next() const;
		TYPE get_data() const;
        int display() const;
        bool operator==(const TYPE & to_comp) const;
	private:
		TYPE data;
		Node<TYPE> * next;
};

template <typename TYPE>
class LLL
{
	public:
		LLL();
		LLL(const LLL & source);
		~LLL();
		LLL<TYPE> & operator=(const LLL<TYPE> & source);
		int insert(const TYPE & new_data);
		int remove(const TYPE & target_data);
		int display() const;
        TYPE retrieve(const TYPE & target_data) const;
	private:
		Node<TYPE> * head;
		int display(Node<TYPE> * head) const;
		int clear(Node<TYPE> * & head);
		int copy(Node<TYPE> * & dest, Node<TYPE> * source);
		int insert(Node<TYPE> * & head, const TYPE & new_data);
		int remove(Node<TYPE> * & head, const TYPE & target_data);
        TYPE retrieve(Node<TYPE> * head, const TYPE & target_data) const;
};

#include "LLL.tpp"
