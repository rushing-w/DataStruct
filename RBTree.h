#include<iostream>
using namespace std;

#pragma once

enum Color
{
	RED,
	BLACK
};

template<class K, class V>
struct RBTreeNode
{
	K _key;
	V _value;
	Color _col;
	RBTreeNode<K, V>* _left;
	RBTreeNode<K, V>* _right;
	RBTreeNode<K, V>* _parent;
	RBTreeNode(const  K& key, const V& value)
		:_left(NULL)
		, _right(NULL)
		, _key(key)
		, _value(value)
		, _parent(NULL)
		, _col(RED)
	{}
};

template<class K, class V>
class RBTree
{
	typedef RBTreeNode<K, V> Node;
public:
	RBTree()
		:_root(NULL)
	{}

	~RBTree()
	{
		Destroy(_root);
		_root = NULL;
	}

	bool Insert(const K& key, const V& value)
	{
		if (_root == NULL)
		{
			_root = new Node(key, value);
			_root->_col = BLACK;
			return true;
		}
		Node* cur;
		Node* parent = NULL;
		cur = _root;
		while (cur)
		{
			if (cur->_key < key)
			{
				parent = cur;
				cur = cur->_right;
			}
			else if (cur->_key>key)
			{
				parent = cur;
				cur = cur->_left;
			}
			else
			{
				return false;
			}
		}
		
		cur = new Node(key, value);
		if (parent->_key > key)
		{
			parent->_left = cur;
			cur->_parent = parent;
		}
		else
		{
			parent->_right = cur;
			cur->_parent = parent;
		}

		while (parent && parent->_col == RED)
		{
			Node* grandfather = parent->_parent;
			if (grandfather->_left == parent)
			{
				Node* uncle = grandfather->_right;
				if (uncle && uncle->_col == RED)
				{
					uncle->_col = parent->_col = BLACK;
					grandfather->_col = RED;
					cur = grandfather;
					parent = cur->_parent;
				}
				else
				{
					if (cur == parent->_right)
					{
						RotateL(parent);
						RotateR(grandfather);
						grandfather->_col = RED;
						cur->_col = BLACK;
						
					}
					else
					{
						RotateR(grandfather);
						parent->_col = BLACK;
						grandfather->_col = RED;
					}
					
					break;
				}
			}
			else
			{
				Node* uncle = grandfather->_left;
				if (uncle && uncle->_col == RED)
				{
					uncle->_col = parent->_col = BLACK;  
					grandfather->_col = RED;
					cur = grandfather;
					parent = cur->_parent;
				}
				else
				{
					if (cur == parent->_left)
					{
						RotateR(parent);
						RotateL(grandfather);
						grandfather->_col = RED;
						cur->_col = BLACK;
					}
					else
					{
						RotateL(grandfather);
						parent->_col = BLACK;
						grandfather->_col = RED;
					}
					break;
				}
			}
				
		}

		_root->_col = BLACK;
		return true;
	}

	bool IsBalance()
	{
		if (_root && _root->_col == RED)
		{
			return false;
		}
		size_t K = 0;
		Node* cur = _root;
		while (cur)
		{
			if (cur->_col == BLACK)
			{
				++K;
			}
			cur = cur->_left;
		}

		size_t num = 0;
		return _CheckColor(_root) && _CheckBlackNum(_root, K, num);
	}

private:
	bool _CheckColor(Node* root)
	{
		if (root == NULL)
			return true;
		if (root->_col == RED && root->_parent->_col == RED)
			return false;
		return _CheckColor(root->_left) && _CheckColor(root->_right);
	}

	bool _CheckBlackNum(Node* root, size_t K, size_t num)
	{
		if (root == NULL)
			return true;
		if (root->_col == BLACK)
			++num;
		if (root->_left == NULL && root->_right == NULL && K != num)
			return false;
		return _CheckBlackNum(root->_left, K, num) && _CheckBlackNum(root->_right, K, num);
	}

	void Destroy(Node* root)
	{
		if (root == NULL)
		{
			return;
		}
		Destroy(root->_left);
		Destroy(root->_right);
	}

	void RotateL(Node* parent)
	{
		Node* SubR = parent->_right;
		Node* SubRL = SubR->_left;
		Node* ppNode = parent->_parent;

		parent->_right = SubRL;
		if (SubRL)
		{
			SubRL->_parent = parent;
		}
		SubR->_left = parent;
		parent->_parent = SubR;
		
		if (ppNode == NULL)
		{
			_root = SubR;
			SubR->_parent = NULL;
		}
		else
		{
			if (parent == ppNode->_left)
			{
				ppNode->_left = SubR;
			}
			else
			{
				ppNode->_right = SubR;
			}
			SubR->_parent = ppNode;
		}
	}

	void RotateR(Node* parent)
	{
		Node* SubL = parent->_left;
		Node* SubLR = SubL->_right;
		Node* ppNode = parent->_parent;

		parent->_left = SubLR;
		if (SubLR)
		{
			SubLR->_parent = parent;
		}
		SubL->_right = parent;
		parent->_parent = SubL;

		SubL->_parent = ppNode;
		if (ppNode == NULL)
		{
			_root = SubL;
		}
		else
		{
			if (parent == ppNode->_left)
			{
				ppNode->_left = SubL;
			}
			else
			{
				ppNode->_right = SubL;
			}
			SubL->_parent = ppNode;
		}
	}

private:
	Node* _root;
};

void TestRBTree()
{
	int a[] = { 5, 3, 4, 1, 7, 8, 2, 6, 0, 9 };
	RBTree<int, int> r;
	for (size_t i = 0; i < sizeof(a) / sizeof(a[0]); i++)
	{
		r.Insert(a[i], i);
		cout << a[i] << " " << r.IsBalance() << endl;
	}
}

