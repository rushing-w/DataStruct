#include<iostream>
#include<assert.h>
#include<queue>
#include<stack>
using namespace std;

template<class T>
struct BinaryTreeNode
{
	T _data;
	BinaryTreeNode<T>* _left;
	BinaryTreeNode<T>* _right;

	BinaryTreeNode(const T& x)
		: _data(x)
		, _left(NULL)
		, _right(NULL)
	{}
};

template<class T>
class BinaryTree
{
	typedef BinaryTreeNode<T> Node;
public:
	BinaryTree(T* a, size_t size, const T& invalid = T())
	{
		assert(a);
		size_t index = 0;
		_root = _CreateTree(a, size, invalid, index);
	}

	BinaryTree(const BinaryTree<T>& t)
	{
		_root = _Copy(t._root);
	}


	BinaryTree<T>& operator=(BinaryTree<T> t)
	{
		std::swap(_root, t._root);
		return *this;
	}

	~BinaryTree()
	{
		_Destroy(_root);
	}

	void PrevOrder()
	{
		_PrevOrder(_root);
		cout << endl;
	}

	//非递归实现前序遍历
	void PrevOrderNonR()
	{
		stack<Node*> s;
		Node* cur = _root;

		while (!s.empty() || cur)
		{
			while (cur)
			{
				cout << cur->_data << " ";
				s.push(cur);
				cur = cur->_left;
			}

			Node* top = s.top();
			cur = top->_right;
			s.pop();
		}
		cout << endl;
	}

	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}

	//非递归实现中序遍历
	void InOrderNonR()
	{
		stack<Node*> s;
		Node* cur = _root;
		while (cur || !s.empty())
		{
			while (cur)
			{
				s.push(cur);
				cur = cur->_left;
			}

			Node* top = s.top();
			cout << top->_data << " ";
			cur = top->_right;

			s.pop();
		}
		cout << endl;
	}

	void PostOrder()
	{
		_PostOrder(_root);
		cout << endl;
	}

	//非递归实现后序遍历
	void PostOrderNonR()
	{
		stack<Node*> s;
		Node* cur = _root;
		Node* prev = NULL;
		while (!s.empty() || cur)
		{
			while (cur)
			{
				s.push(cur);
				cur = cur->_left;
			}
			Node* top = s.top();
			if (top->_right==NULL || top->_right == prev)
			{
				cout << top->_data << " ";
				prev = top;
				s.pop();
			}
			else
			{
				cur = top->_right;
			}
		}
		cout << endl;
	}

	void LevelOrder()
	{
		if (_root == NULL)
		{
			return;
		}
		queue<Node*> q;
		q.push(_root);
		while (!q.empty())
		{
			Node* front = q.front();
			cout << front->_data << " ";
			q.pop();
			if (front->_left != NULL)
			{
				q.push(front->_left);
			}
			if (front->_right != NULL)
			{
				q.push(front->_right);
			}
		}
		cout << endl;
	}

	size_t Size()
	{
		return _Size(_root);
	}

	size_t Depth()
	{
		return _Depth(_root);
	}

	size_t GetLeafSize()
	{
		size_t count = 0;
		_GetLeafSize(_root, count);
		return count;
	}

	size_t GetKLevelSize(size_t k)
	{
		assert(k);
		return _GetKLevelSize(_root, k);
	}

protected:
	Node* _CreateTree(T* a, size_t size, const T& invalid, size_t& index)
	{
		if (index < size && a[index] != invalid)
		{
			Node* root = new Node(a[index]);
			root->_left = _CreateTree(a, size, invalid, ++index);
			root->_right = _CreateTree(a, size, invalid, ++index);
			return root;
		}
		return NULL;
	}

	void _Destroy(Node* root)
	{
		if (root == NULL)
		{
			return;
		}
		if (root->_left != NULL)
		{
			_Destroy(root->_left);
			root->_left = NULL;
		}
		if (root->_right != NULL)
		{
			_Destroy(root->_right);
			root->_right = NULL;
		}

		delete root;
		root = NULL;
		return;
	}

	Node* _Copy(Node* root)
	{
		if (root == NULL)
		{
			return NULL;
		}

		Node* NewNode = new Node(root->_data);
		NewNode->_left = _Copy(root->_left);
		NewNode->_right = _Copy(root->_right);

		return NewNode;
	}

	void _PrevOrder(Node* root)
	{
		if (root == NULL)
		{
			return;
		}
		cout << root->_data << " ";
		_PrevOrder(root->_left);
		_PrevOrder(root->_right);
	}

	void _InOrder(Node* root)
	{
		if (root == NULL)
		{
			return;
		}
		_InOrder(root->_left);
		cout << root->_data << " ";
		_InOrder(root->_right);
	}

	void _PostOrder(Node* root)
	{
		if (root == NULL)
		{
			return;
		}
		_PostOrder(root->_left);
		_PostOrder(root->_right);
		cout << root->_data << " ";
	}

	size_t _Size(Node* root)
	{
		if (root == NULL)
		{
			return 0;
		}
		return _Size(root->_left) + _Size(root->_right) + 1;
	}

	size_t _Depth(Node* root)
	{
		if (root == NULL)
		{
			return 0;
		}
		size_t left = _Depth(root->_left);
		size_t right = _Depth(root->_right);
		if (left < right)
		{
			return right + 1;
		}
		else
		{
			return left + 1;
		}
	}

	void _GetLeafSize(Node* root,size_t& count)
	{
		if (root->_left == NULL&&root->_right == NULL)
		{
			count++;
			return;
		}
		if (root->_left != NULL)
		{
			_GetLeafSize(root->_left, count);
		}
		if (root->_right != NULL)
		{
			_GetLeafSize(root->_right, count);
		}
	}

	size_t _GetKLevelSize(Node* root, size_t k)
	{
		if (root == NULL)
		{
			return 0;
		}
		if (k == 1)
		{
			return 1;
		}
		return _GetKLevelSize(root->_left, k - 1) + _GetKLevelSize(root->_right, k - 1);
	}

protected:
	Node* _root;
};


void TestBinaryTree()
{
	int a[10] = { 1, 2, 3, '#', '#', 4, '#', '#', 5, 6 };
	BinaryTree<int> t1(a, sizeof(a) / sizeof(a[0]), '#');
	
	cout << t1.Size() << endl;
	cout << t1.Depth() << endl;
	cout << t1.GetLeafSize() << endl;
	cout << t1.GetKLevelSize(3) << endl;
	t1.PrevOrder();
	t1.PrevOrderNonR();
	t1.InOrder();
	t1.InOrderNonR();
	t1.PostOrder();
	t1.PostOrderNonR();
	t1.LevelOrder();
}