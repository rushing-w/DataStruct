#pragma once

#include<iostream>
using namespace std;

enum PointerTag
{
	LINK,
	THREAD,
};

template<class T>
struct BinaryTreeThdNode
{
	typedef BinaryTreeThdNode<T> Node;

	T _data;
	Node* _left;
	Node* _right;
	Node* _parent;

	PointerTag _leftTag;
	PointerTag _rightTag;

	BinaryTreeThdNode(T& x)
		: _data(x)
		, _left(NULL)
		, _right(NULL)
		, _parent(NULL)
		, _leftTag(LINK)
		, _rightTag(LINK)
	{}	
};

template<class T, class Ref, class Ptr>
struct BinaryTreeIterator
{
	typedef BinaryTreeThdNode<T> Node;
	typedef BinaryTreeIterator<T, Ref, Ptr> Self;

	Node* _node;

	BinaryTreeIterator(Node* node)
		: _node(node)
	{}

	Ref operator*()
	{
		return _node->_data;
	}

	Ptr operator->()
	{
		return &(operator*());
	}

	Self& operator++()
	{
		_node = _Next(_node);
		return *this;
	}

	Self operator++(int)
	{
		Self tmp(*this);
		_node = _Next(_node);
		return tmp;
	}

	Self& operator--()
	{
		_node = _Prev(_node);
		return *this;
	}

	Self operator--(int)
	{
		Self tmp(*this);
		_node = _Prev(_node);
		return tmp;
	}

	bool operator!= (const Self& s) const
	{
		return _node != s._node;
	}

protected:
	Node* _Next(Node* node)
	{
		if (node->_rightTag == THREAD)
		{
			return node->_right;
		}
		else
		{
			Node* cur = node->_right;
			while (cur && cur->_leftTag == LINK)
			{
				cur = cur->_left;
			}
			return cur;
		}
	}

	Node* _Prev(Node* node)
	{
		if (node->_leftTag == THREAD)
		{
			return node->_left;
		}
		else
		{
			Node* cur = node->_left;
			while (cur->_rightTag == LINK)
			{
				cur = cur->_right;
			}
			return cur;
		}
	}
};


template<class T>
class BinaryTreeThd
{
	typedef BinaryTreeThdNode<T> Node;

public:
	typedef BinaryTreeIterator<T, T&, T*> Iterator;
	typedef BinaryTreeIterator<T, const T&, const T*> ConstIterator;

	Iterator Begin()
	{
		Node* cur = _root;
		while (cur->_leftTag == LINK)
		{
			cur = cur->_left;
		}
		return Iterator(cur);
	}

	Iterator End()
	{
		return NULL;
	}

public:
	BinaryTreeThd()
		: _root(NULL)
	{}

	BinaryTreeThd(T* a, size_t n, T invalid = T())
	{
		size_t index = 0;
		_root = _CreateTree(a, n, invalid, index);
	}

	void InOrderThreading()
	{
		Node* prev = NULL;
		_InOrderThreading(_root, prev);
	}

	void InOrderThd()
	{
		Node* cur = _root;
		while (cur)
		{
			while (cur->_leftTag == LINK)
			{
				cur = cur->_left;
			}

			cout << cur->_data << " ";

			while (cur->_rightTag == THREAD)
			{
				cur = cur->_right;
				if (cur)
				{
					cout << cur->_data << " ";
				}
			}

			cur = cur->_right;
		}

		cout << endl;
	}

	void PrevOrderThreading()
	{
		Node* prev = NULL;
		_PrevOrderThreading(_root, prev);
	}

	void PrevOrderThd()
	{
		Node* cur = _root;
		while (cur)
		{
			while (cur->_leftTag == LINK)
			{
				cout << cur->_data << " ";
				cur = cur->_left;
			}
			cout << cur->_data << " ";

			cur = cur->_right;
		}
		cout << endl;
	}

protected:
	Node* _CreateTree(T* a, size_t n, T& invalid, size_t& index)
	{
		if (index < n && a[index] != invalid)
		{
			Node* root = new Node(a[index]);
			root->_left = _CreateTree(a, n, invalid, ++index);
			root->_right = _CreateTree(a, n, invalid, ++index);
			return root;
		}
		return NULL;
	}

	void _InOrderThreading(Node* root, Node*& prev)
	{
		if (root == NULL)
			return;
		if (root->_leftTag == LINK)
		{
			_InOrderThreading(root->_left, prev);
		}
		if (root->_left == NULL)
		{
			root->_leftTag = THREAD;
			root->_left = prev;
		}
		if (prev && prev->_right == NULL)
		{
			prev->_rightTag = THREAD;
			prev->_right = root;
		}
		prev = root;
		if (root->_rightTag == LINK)
		{
			_InOrderThreading(root->_right, prev);
		}
	}

	void _PrevOrderThreading(Node* root, Node*& prev)
	{
		if (root == NULL)
			return;

		if (root->_left == NULL)
		{
			root->_left = prev;
			root->_leftTag = THREAD;
		}

		if (prev && prev->_right == NULL)
		{
			prev->_right = root;
			prev->_rightTag = THREAD;
		}

		prev = root;

		if (root->_leftTag == LINK)
			_PrevOrderThreading(root->_left, prev);

		if (root->_rightTag == LINK)
			_PrevOrderThreading(root->_right, prev);
	}

protected:
	Node* _root;
};

void TestBinaryTreeThd()
{
	int arr[10] = { 1, 2, 3, '#', '#', 4, '#', '#', 5, 6 };
	BinaryTreeThd<int> t(arr, sizeof(arr) / sizeof(arr[0]), '#');

	t.InOrderThreading();
	t.InOrderThd();

	BinaryTreeThd<int>::Iterator it = t.Begin();
	while (it != t.End())
	{
		cout << *it << " ";
		++it;
	}
	cout << endl;

//	t.PrevOrderThreading();
//	t.PrevOrderThd();
 }
