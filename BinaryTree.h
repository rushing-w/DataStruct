#pragma once

#include<iostream>
#include<assert.h>
#include<queue>
#include<stack>
#include<vector>
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

	//�ǵݹ�ʵ��ǰ�����
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

	//�ǵݹ�ʵ���������
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

	//�ǵݹ�ʵ�ֺ������
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
			if (top->_right == NULL || top->_right == prev)//�����ǰ�ڵ�����������Ѿ����ʹ��Ļ���Ϊ�գ���ô�Ͳ����ʣ�
			{														//���������������
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

	//�������-->���ö��а���ʵ��
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

	//��ȡ�ڵ����
	size_t Size()
	{
		return _Size(_root);
	}

	//��ȡ�������
	size_t Depth()
	{
		return _Depth(_root);
	}

	//��ȡҶ�ӽڵ�ĸ���
	size_t GetLeafSize()
	{
		size_t count = 0;
		_GetLeafSize(_root, count);
		return count;
	}

	//��ȡ��k��ڵ����
	size_t GetKLevelSize(size_t k)
	{
		assert(k);
		return _GetKLevelSize(_root, k);
	}

	//�������ڵ�������������
	Node* GetCommonParent(Node* x1, Node* x2)
	{
		//return _GetCommonParent(_root, x1, x2);//O(N^2)�ķ���
		//O(N)�ķ���
		vector<Node*> v1;
		vector<Node*> v2;
		if (_GetPath(v1, _root, x1) == false || _GetPath(v2, _root, x2) == false)
			return NULL;

		
		for (size_t i = 0; i < v1.size() && i < v2.size(); ++i)
		{
			if (v1[i] != v2[i])
				return v1[i - 1];
		}
		return NULL;
	}

	Node* Getx1Node()
	{
		return _root->_left->_left;
	}

	Node* Getx2Node()
	{
		return _root->_left->_right;
	}

	//�����������Զ�������ڵ�ľ���-->������Ҫô���������е������룬Ҫô���������е������룬Ҫô���������ڵ��е�
	//���ڵ��������+�������ڵ��е����ڵ�������룬ͬʱ��¼���������������ڵ��е����ڵ�������룬Ȼ��ݹ�ȥ�߼���
	size_t GetMaxDistance()
	{
		//1��
		//int maxDistance = 0;
		//_GetMaxDistance(_root, maxDistance);
		//return maxDistance;

		//2��
		int maxLeft = 0, maxRight = 0;
		return _GetMaxDistance(_root, maxLeft, maxRight);
	}

	//��������ľ���
	void GetImage()
	{
		_GetImage(_root);
	}

	//�ж�һ�����Ƿ�����ȫ������-->����������нڵ㣬�����������һ���NULL�ڵ�ʱ��������滹�нڵ���ֵ����ô�Ͳ���
	bool IsCompleteBinaryTree_1()
	{
		if (_root == NULL)
			return true;

		queue<Node*> q;

		q.push(_root);
		bool flag = false;//����������һ��NULL�ڵ�
		while (!q.empty())
		{
			Node* front = q.front();
			q.pop();

			if (front)
			{
				if (flag)
					return false;
				q.push(front->_left);
				q.push(front->_right);
			}
			else
				flag = true;
		}
		return true;
	}

	//�ж�һ�����Ƿ�����ȫ������
	bool IsCompleteBinaryTree_2()
	{
		if (_root == NULL)
			return true;

		queue<Node*> q;
		q.push(_root);
		bool flag = false;//��ʾ���ǵ�ǰ��״̬��  true�Ѿ�������Ҷ�ӽڵ�  false ��ʾ��û������
		while (!q.empty())
		{
			Node* front = q.front();
			q.pop();
			if (flag == false)//��ǰ��û������Ҷ�ӽڵ�
			{
				if (front->_left == NULL && front->_right)
					return  false;
				else if (front->_left == NULL && front->_right == NULL)//��ʾ�ýڵ�ΪҶ�ӽڵ�
					flag = true;//״̬��Ϊtrue
				else
				{
					if (front->_left)
						q.push(front->_left);
					if (front->_right)
						q.push(front->_right);
				}
			}
			else//״̬Ϊ �Ѿ�������Ҷ�ӽڵ�
			{
				if (front->_left || front->_right)//Ҫ�ǵ�ǰ�ڵ㲻��Ҷ�ӽڵ�   ��ô�����Ͳ���  ��ȫ������
					return  false;
			}
		}
		return true;
	}

	//�ж�һ�����Ƿ�����ȫ������-->����������нڵ㣬����������һ��û�����ӻ��Һ��ӵĽڵ㣬�������Ȼ���������ߣ�
	//�������Ľڵ��л����ں��ӽڵ㣬��ô�Ͳ�����ȫ������
	bool IsCompleteBinaryTree_3()
	{
		if (_root == NULL)
			return false;

		queue<Node*> q;
		bool flag = false;
		q.push(_root);
		while (!q.empty())
		{
			Node* front = q.front();
			q.pop();

			if (flag == true && (front->_left || front->_right))
				return false;

			if (front->_left)
				q.push(front->_left);
			else
				flag = true;

			if (front->_right && flag == true)
				return false;

			if (front->_right)
				q.push(front->_right);
			else
				flag = true;
		}
		return true;
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

	void _GetImage(Node* root)
	{
		if (root == NULL)
			return;

		if (root->_left == NULL && root->_right == NULL)
			return;

		std::swap(root->_left, root->_right);

		if (root->_left)
			_GetImage(root->_left);

		if (root->_right)
			_GetImage(root->_right);
	}

	Node* _GetCommonParent(Node* root, Node* x1, Node* x2)
	{
		if (root == NULL)
			return NULL;

		if (root == x1 || root == x2)
			return root;

		Node* Left = _GetCommonParent(root->_left, x1, x2);
		Node* Right = _GetCommonParent(root->_right, x1, x2);
		Node* tmp = NULL;

		if (Left != NULL && Right != NULL)
		{
			return root;
		}
		else
		{
			if (Left != NULL)
				tmp = Left;
			else
				tmp = Right;

			return tmp;
		}
	}

	bool _GetPath(vector<Node*> &v, Node* root, Node* x)
	{
		if (root == NULL)
			return false;

		if (root->_data == x->_data)
		{
			v.push_back(root);
			return true;
		}

		v.push_back(root);

		if (_GetPath(v, root->_left, x))//����bool����ֵ�ж��Ƿ��ҵ�
			return true;
		if (_GetPath(v, root->_right, x))
			return true;

		v.pop_back();
		return false;
	}

	size_t _GetMaxDistance(Node* root, int& maxDistance)
	{
		if (root == NULL)
			return 0;

		size_t left = _GetMaxDistance(root->_left, maxDistance);
		size_t right = _GetMaxDistance(root->_right, maxDistance);

		if (maxDistance < left + right)
		{
			maxDistance = left + right;
		}

		return left>right ? left + 1 : right + 1;
	}

	size_t _GetMaxDistance(Node* root, int& maxLeft, int& maxRight)
	{
		if (root == NULL)
		{
			maxLeft = 0;
			maxRight = 0;
			return 0;
		}

		int maxLL, maxLR, maxRL, maxRR;
		int maxLeftDistance, maxRightDistance;

		if (root->_left)
		{
			maxLeftDistance = _GetMaxDistance(root->_left, maxLL, maxLR);
			maxLeft = maxLL > maxLR ? maxLL + 1 : maxLR + 1;
		}
		else
		{
			maxLeftDistance = 0;
			maxLeft = 0;
		}

		if (root->_right)
		{
			maxRightDistance = _GetMaxDistance(root->_right, maxRL, maxRR);
			maxRight = maxRL > maxRR ? maxRL + 1 : maxRR + 1;
		}
		else
		{
			maxRightDistance = 0;
			maxRight = 0;
		}
		int maxDistance = maxLeftDistance > maxRightDistance ? maxLeftDistance : maxRightDistance;

		return maxDistance > (maxLeft + maxRight) ? maxDistance : (maxLeft + maxRight);
	}

protected:
	Node* _root;
};


void TestBinaryTree()
{
	int a[10] = { 1, 2, 3, '#', '#', 4, '#', '#', 5, 6 };
	BinaryTree<int> t1(a, sizeof(a) / sizeof(a[0]), '#');
	
	BinaryTreeNode<int>* x1 = t1.Getx1Node();
	BinaryTreeNode<int>* x2 = t1.Getx2Node();
	BinaryTreeNode<int>* ret = t1.GetCommonParent(x1, x2);
	cout << t1.GetMaxDistance() << endl;
	cout << t1.Size() << endl;
	cout << t1.Depth() << endl;
	cout << t1.GetLeafSize() << endl;
	cout << t1.GetKLevelSize(3) << endl;
	t1.GetImage();
	t1.PrevOrder();
	t1.PrevOrderNonR();
	t1.InOrder();
	t1.InOrderNonR();
	t1.PostOrder();
	t1.PostOrderNonR();
	t1.LevelOrder();
}

void TestIsCompleteTree()
{
	int a[] = { 1, 2, 3, '#', '#', 4, '#', '#', 5, 6, '#', '#', 7 };
	BinaryTree<int> t1(a, sizeof(a) / sizeof(a[0]), '#');
	cout << t1.IsCompleteBinaryTree_1() << endl;
	cout << t1.IsCompleteBinaryTree_2() << endl;
	cout << t1.IsCompleteBinaryTree_3() << endl;
}