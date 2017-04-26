#pragma once

#include<iostream>
using namespace std;

template<class T>
struct ListNode
{
	ListNode<T>* _next;
	ListNode<T>* _prev;
	T _data;

	ListNode(const T& data)
		: _data(data)
		, _prev(NULL)
		, _next(NULL)
	{}
};

template<class T, class Ref, class Ptr>
struct ListIterator
{
	typedef ListNode<T> Node;
	typedef ListIterator<T, Ref, Ptr> Self;

	Node* _node;

	ListIterator()
	{}

	ListIterator(Node* node)
		:_node(node)
	{}

	bool operator==(const Self& s)
	{
		return _node == s._node;
	}

	bool operator!=(const Self& s)
	{
		return _node != s._node;
	}

	Ref operator*()
	{
		return _node->_data;
	}

	Ptr operator->()
	{
		//return &_node->_data;
		return &(operator*());
	}

	Self& operator++()
	{
		_node = _node->_next;
		return *this;
	}

	Self& operator++(int)
	{
		Self tmp(*this);
		_node = _node->_next;
		return tmp;
	}

	Self& operator--()
	{
		_node = _node->_prev;
		return *this;
	}

	Self& operator--(int)
	{
		Self tmp(*this);
		_node = _node->_prev;
		return tmp;
	}
};

template<class T>
class List
{
	typedef ListNode<T> Node;
public:

	typedef ListIterator<T, T&, T*> Iterator;
	typedef ListIterator<T, const T&, const T*> ConstIterator;

	List()
		: _head(_BuyNewNode(T()))
	{
		_head->_next = _head;
		_head->_prev = _head;
	}

	List(const List<T>& l)
	{
		Node* NewHead = l._head;
		if (NewHead == NULL)
		{
			_head = NULL;
			return;
		}

		_head = _BuyNewNode(NewHead->_data);
		Node* cur = _head;
		Node* tmp = NewHead->_next;
		while (tmp != NewHead)
		{
			Node* NewNode = _BuyNewNode(tmp->_data);
			cur->_next = NewNode;
			NewNode->_prev = cur;

			cur = cur->_next;
			tmp = tmp->_next;
		}
		cur->_next = _head;
		_head->_prev = cur;
	}

	List<T>& operator=(const List<T>& l)
	{
		if (this != &l)
		{
			Node* del = _head;
			delete del;

			Node* NewHead = l._head;
			if (NewHead == NULL)
			{
				_head = NULL;
				return *this;
			}

			_head = _BuyNewNode(NewHead->_data);
			Node* cur = _head;
			Node* tmp = NewHead->_next;
			while (tmp != NewHead)
			{
				Node* NewNode = _BuyNewNode(tmp->_data);
				cur->_next = NewNode;
				NewNode->_prev = cur;

				cur = cur->_next;
				tmp = tmp->_next;
			}
			cur->_next = _head;
			_head->_prev = cur;
		}
		return *this;
	}

	~List()
	{
		if (_head)
			delete _head;
		_head = NULL;
	}

	void PushBack(const T& data)
	{
		Node* NewNode = _BuyNewNode(data);
		Node* tail = _head->_prev;
		tail->_next = NewNode;
		NewNode->_prev = tail;
		NewNode->_next = _head;
		_head->_prev = NewNode;
	}

	void PopBack()
	{
		Node* tail = _head->_prev;
		Node* del = tail;
		tail = tail->_prev;
		tail->_next = _head;
		_head->_prev = tail;
		delete del;
		del = NULL;
	}

	void PushFront(const T& data)
	{
		Node* NewNode = _BuyNewNode(data);
		NewNode->_next = _head->_next;
		_head->_next->_prev = NewNode;
		_head->_next = NewNode;
		NewNode->_prev = _head;
	}

	void PopFront()
	{
		Node* del = _head->_next;
		_head->_next = del->_next;
		del->_next->_prev = _head;
		delete del;
		del = NULL;
	}

	Node* Find(const T& data)
	{
		Node* cur = _head->_next;
		while (cur != _head)
		{
			if (cur->_data == data)
				return cur;
			cur = cur->_next;
		}
		return NULL;

	}

	void Insert(Node* pos, const T& data)
	{
		assert(pos);
		if (pos == _head->_prev)
		{
			PushBack(data);
			return;
		}
		if (pos == _head)
		{
			PushFront(data);
			return;
		}

		Node* NewNode = _BuyNewNode(data);
		NewNode->_next = pos->_next;
		pos->_next->_prev = NewNode;
		pos->_next = NewNode;
		NewNode->_prev = pos;
	}

	template<class InputIterator>
	void Insert(Iterator pos, InputIterator first, InputIterator last)//插入一个迭代器区间
	{
		assert(pos);
		while (first != last)
		{
			Insert(pos, *first);
			first++;
		}
	}

	Iterator Insert(Iterator pos, const T& data)//插在pos前面
	{
		assert(pos._node);
		Node* NewNode = _BuyNewNode(data);
		Node* cur = pos._node->_prev;

		cur->_next = NewNode;
		NewNode->_prev = cur;

		NewNode->_next = pos._node;
		pos._node->_prev = NewNode;

		return NewNode;
	}

	void Erase(Node* pos)
	{
		assert(pos && pos != _head);
		if (pos->_next == _head)
		{
			PopBack();
			return;
		}
		if (pos->_prev == _head)
		{
			PopFront();
			return;
		}

		pos->_prev->_next = pos->_next;
		pos->_next->_prev = pos->_prev;
		delete pos;
	}

	Iterator Erase(Iterator pos)
	{
		assert(pos._node && pos._node != _head);

		Node* Prev = pos._node->_prev;
		Node* Next = pos._node->_next;

		Prev->_next = Next;
		Next->_prev = Prev;

		delete pos._node;
		pos._node = NULL;
		return Next;
	}

	void Remove(const T& data)
	{
		Node* cur = _head->_next;
		while (cur != _head)
		{
			if (cur->_data == data)
			{
				if (cur->_next == _head)
				{
					PopBack();//走到尾删这里说明已经走到链表最后了，删掉最后一个就可以直接返回了
					return;
				}
				else if (cur->_prev == _head)
				{
					PopFront();
				}
				else
				{
					Node* del = cur;
					del->_prev->_next = del->_next;
					del->_next->_prev = del->_prev;
					cur = cur->_next;
					delete del;
					continue;
				}
			}
			cur = cur->_next;
		}
	}

	void Clear()
	{
		_head->_next = _head->_prev = NULL;
	}

	size_t Size()//因为在构造时默认是将这个头节点构造成值为0的节点，所以size最小为1
	{
		Node* cur = _head;
		size_t count = 1;
		while (cur->_next != _head)
		{
			count++;
			cur = cur->_next;
		}
		return count;
	}

	bool Empty()
	{
		if (Size() == 1)
			return true;
		return false;
	}

	Iterator Begin()
	{
		return Iterator(_head->_next);
		// return _head->_next;  在这里发生了隐式类型转换，在具有单参构造函数的类的情况下，
		//_head->_next会转换成Iterator(_head->_next) 
	}

	ConstIterator CBegin() const
	{
		return ConstIterator(_head->_next);
	}

	Iterator End()
	{
		return Iterator(_head);
	}

	ConstIterator CEnd() const
	{
		return ConstIterator(_head);
	}

protected:
	Node* _BuyNewNode(const T& data)
	{
		return new Node(data);
	}

protected:
	Node* _head;
};


void TestList()
{
	List<int> l;
	l.PushBack(1);
	l.PushBack(2);
	l.PushBack(3);
	l.PushBack(4);
	l.PushBack(5);
	l.Insert(l.Find(3), 9);
	l.Insert(l.Find(4), 9);
	l.Insert(l.Find(5), 9);
	l.Remove(9);

	List<int>::Iterator it = l.Begin();
	while (it != l.End())
	{
		cout << *it << " ";
		++it;
	}
	cout << endl;

	l.Size();
	l.PopBack();
	l.PushFront(6);
	l.PopFront();
	ListNode<int>* ret = l.Find(3);
	l.Insert(l.Find(3), 9);
	l.Erase(l.Find(9));
	l.Clear();

	List<int> l2(l);
	List<int> l3;
	l3 = l;
}