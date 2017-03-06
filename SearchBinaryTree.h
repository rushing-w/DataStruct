#pragma once

#include<iostream>
using namespace std;
#include<assert.h>

template<class T>
struct SearchBinaryTreeNode
{
	SearchBinaryTreeNode<T>* _left;
	SearchBinaryTreeNode<T>* _right;

	T _key;

	SearchBinaryTreeNode(const T& k)
		: _left(NULL)
		, _right(NULL)
		, _key(k)
	{}
};

//BSTree
template<class T>
class SearchBinaryTree
{
	typedef SearchBinaryTreeNode<T> Node;

public:
	SearchBinaryTree()
		: _root(NULL)
	{}

	~SearchBinaryTree()
	{
		_Destroy(_root);
		_root = NULL;
	}

	bool Insert(const T& k)//非递归方式
	{
		if (_root == NULL)
		{
			_root = new Node(k);
			return true;
		}

		Node* cur = _root;
		Node* parent = cur;

		while (cur)
		{
			parent = cur;

			if (cur->_key < k)
			{
				cur = cur->_right;
			}
			else if (cur->_key>k)
			{
				cur = cur->_left;
			}
			else
			{
				return false;
			}
		}

		if (parent->_key > k)
		{
			parent->_left = new Node(k);
		}
		else if (parent->_key < k)
		{
			parent->_right = new Node(k);
		}

		return true;
	}

	bool InsertR(const T& k)//递归方式
	{
		return _InsertR(_root, k);
	}


	Node* Find(const T& k)//非递归方式
	{
		Node* cur = _root;

		while (cur)
		{
			if (cur->_key > k)
			{
				cur = cur->_left;
			}
			else if (cur->_key > k)
			{
				cur = cur->_right;
			}
			else
			{
				return cur;
			}
		}

		return NULL;
	}

	Node* FindR(const T& k)//递归方式
	{
		return _FindR(_root, k);
	}

	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}

	bool Remove(const T& k)//非递归方式
	{
		Node* cur = _root;
		Node* parent = NULL;
		Node* del = cur;

		//先遍历找到需要删除的结点
		while (cur)
		{
			if (cur->_key < k)
			{
				parent = cur;
				cur = cur->_right;
			}
			else if (cur->_key>k)
			{
				parent = cur;
				cur = cur->_left;
			}
			else
			{
				break;
			}
		}

		if (cur == NULL)
			return false;

		if (cur->_left == NULL)
		{
			del = cur;

			if (cur == _root)
				_root = cur->_right;
			else if (cur == parent->_left)
				parent->_left = cur->_right;
			else
				parent->_right = cur->_right;
		}
		else if (cur->_right == NULL)
		{
			del = cur;

			if (cur == _root)
				_root = cur->_right;
			else if (cur == parent->_left)
				parent->_left = cur->_right;
			else
				parent->_right = cur->_right;
		}
		else
		{
			Node* subleft = NULL;

			subleft = cur->_right;
			parent = cur;
			while (subleft->_left)
			{
				parent = subleft;
				subleft = subleft->_left;
			}

			cur->_key = subleft->_key;

			if (parent->_left == subleft)
				parent->_left = subleft->_right;
			else
				parent->_right = subleft->_right;

			del = subleft;
		}
		
		delete del;
		del = NULL;

		return true;
	}

	bool RemoveR(const T& k)
	{
		return _RemoveR(_root, k);
	}

protected:
	void _Destroy(Node* root)
	{
		if (root == NULL)
			return;
		_Destroy(root->_left);
		_Destroy(root->_right);
	}

	void _InOrder(Node* root)
	{
		if (root == NULL)
			return;

		_InOrder(root->_left);
		cout << root->_key << " ";
		_InOrder(root->_right);
	}

	bool _InsertR(Node*& root, const T& k)
	{
		if (root == NULL)
		{
			root = new Node(k);
			return true;
		}

		if (root->_key > k)
		{
			return _InsertR(root->_left, k);
		}
		else if (root->_key < k)
		{
			return _InsertR(root->_right, k);
		}
		else
		{
			return false;
		}
	}

	Node* _FindR(Node* root, const T& k)
	{
		if (root == NULL)
			return NULL;

		if (root->_key < k)
		{
			return _FindR(root->_right, k);
		}
		else if (root->_key >k)
		{
			return _FindR(root->_left, k);
		}
		else
		{
			return root;
		}
	}

	bool _RemoveR(Node*& root, const T& k)
	{
		if (root == NULL)
			return false;

		Node* del = NULL;

		if (root->_key < k)
			return _RemoveR(root->_right, k);
		else if (root->_key>k)
			return _RemoveR(root->_left, k);
		else
		{
			if (root->_left == NULL)
			{
				del = root;
				root = root->_right;
			}
			else if (root->_right == NULL)
			{
				del = root;
				root = root->_left;
			}
			else
			{
				Node* subleft = root->_right;
				Node* parent = root;
				while (subleft->_left)
				{
					subleft = subleft->_left;
					parent = subleft;
				}
				
				if (parent->_left == subleft)
					parent->_left = subleft->_right;
				else
					parent->_right = subleft->_right;
			}
			delete del;
			del = NULL;

			return true;
		}
	}
	

protected:
	Node* _root;
};

void TestBSTree()
{
	int a[] = { 5, 3, 4, 1, 7, 8, 2, 6, 0, 9 };
	SearchBinaryTree<int> s;

	for (size_t i = 0; i < sizeof(a) / sizeof(a[0]); i++)
	{
		s.InsertR(a[i]);
	}
	s.InOrder();

	s.RemoveR(2);
	s.RemoveR(1);
	s.RemoveR(8);
	s.RemoveR(5);

	s.InOrder();

	s.RemoveR(0);
	s.RemoveR(1);
	s.RemoveR(2);
	s.RemoveR(3);
	s.RemoveR(4);
	s.RemoveR(5);
	s.RemoveR(6);
	s.RemoveR(7);
	s.RemoveR(8);
	s.RemoveR(9);

	s.InOrder();
}
