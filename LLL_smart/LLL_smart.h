//This is template of linear linked list.
//This version uses smart pointer.

#include <memory>
using std::unique_ptr;

template <typename TYPE>
class Node
{
	public:
		Node<TYPE>();
		Node<TYPE>(const TYPE & source);
		Node<TYPE>(const Node<TYPE> & source);
		unique_ptr<Node<TYPE>> & get_next();
		TYPE get_data() const;
        int display() const;
        bool operator==(const TYPE & to_comp);
	private:
		TYPE data;
		unique_ptr<Node<TYPE>> next;
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
		unique_ptr<Node<TYPE>> head;

		int display(const unique_ptr<Node<TYPE>> & head) const;
		int clear(unique_ptr<Node<TYPE>> & head);
		int copy(unique_ptr<Node<TYPE>> & dest, const unique_ptr<Node<TYPE>> & source);
		int insert(unique_ptr<Node<TYPE>> & head, const TYPE & new_data);
		int remove(unique_ptr<Node<TYPE>> & head, const TYPE & target_data);
        TYPE retrieve(unique_ptr<Node<TYPE>> & head, const TYPE & target_data) const;
};

#include "LLL_smart.tpp"
