#pragma once

#include<iostream>
using namespace std;
#include<assert.h>
#include<stack>

template<class T>
struct ListNode
{
	T _data;
	ListNode<T>* _next;

	ListNode(const T& x = T())
		: _data(x)
		, _next(NULL)
	{}
};


template<class T>
class SList
{
	typedef ListNode<T> Node;
public:
	SList()
		: _head(NULL)
	{}

	SList(const SList<T>& s)
	{
		Node* NewHead = s._head;
		if (!NewHead)
		{
			_head = NULL;
			return;
		}
		_head = _BuyNode(NewHead->_data);

		Node* cur = _head;
		Node* tmp = NewHead->_next;
		while (tmp)
		{
			Node* NewNode = _BuyNode(tmp->_data);
			cur->_next = NewNode;;
			cur = cur->_next;
			tmp = tmp->_next;
		}

	}

	SList<T>& operator=(const SList<T>& s)
	{
		if (this != &s)
		{
			Node* del = _head;
			delete del;

			Node* NewHead = s._head;
			if (!NewHead)
			{
				_head = NULL;
				return *this;
			}
			_head = _BuyNode(NewHead->_data);

			Node* cur = _head;
			Node* tmp = NewHead->_next;
			while (tmp)
			{
				Node* NewNode = _BuyNode(tmp->_data);
				cur->_next = NewNode;;
				cur = cur->_next;
				tmp = tmp->_next;
			}
		}
		return *this;
	}

	~SList()
	{
		if (_head)
			delete _head;
		_head = NULL;
	}

	void PushBack(const T x)
	{
		Node* NewNode = _BuyNode(x);
		if (_head == NULL)
		{
			_head = NewNode;
		}
		else
		{
			Node* cur = _head;
			while (cur->_next)
			{
				cur = cur->_next;
			}
			cur->_next = NewNode;
		}
	}

	void PopBack()
	{
		Node* cur = _head;
		if (!cur)
		{
			return;
		}
		else if (!cur->_next)
		{
			delete cur;
			cur = NULL;
		}
		else
		{
			while (cur)
			{
				if (cur->_next->_next == NULL)
				{
					Node* tail = cur->_next;
					delete tail;
					tail = NULL;
					tail = cur;
					tail->_next = NULL;
					break;
				}
				cur = cur->_next;
			}
		}
	}

	void PushFront(const T& x)
	{
		Node* NewNode = _BuyNode(x);
		if (_head == NULL)
		{
			_head = NewNode;
		}
		else
		{
			Node* cur = _head;
			NewNode->_next = cur;
			_head = NewNode;
		}
	}

	void PopFront()
	{
		Node* cur = _head;
		_head = _head->_next;
		delete cur;
		cur = NULL;
	}

	Node* Find(const T& x)
	{
		Node* cur = _head;
		while (cur)
		{
			if (cur->_data == x)
				return cur;

			cur = cur->_next;
		}

		return NULL;
	}

	void Insert(Node* pos, const T& x)
	{
		assert(pos);
		Node* NewNode = _BuyNode(x);
		if (NewNode)
		{
			NewNode->_next = pos->_next;
			pos->_next = NewNode;
		}
		else
			return;
	}

	void Erase(Node* pos)
	{
		assert(pos);
		Node* cur = _head;
		if (_head == pos)
		{
			PopFront();
			return;
		}
		while (cur)
		{
			if (cur->_next == NULL)
			{
				PopBack();
				break;
			}
			if (cur->_next == pos)
			{
				cur->_next = pos->_next;
				delete pos;
				pos = NULL;
				break;
			}
			cur = cur->_next;
		}
	}

	void Remove(const T& x)
	{
		Node* cur = _head;
		Node* prev = NULL;
		while (cur)
		{
			if (_head->_data == x)
			{
				PopFront();
				return;
			}
			prev = cur->_next;
			if (prev->_data == x)
			{
				cur->_next = prev->_next;
				delete prev;
				prev = NULL;
				return;
			}
			cur = cur->_next;
		}
	}

	void RemoveAll(const T& x)
	{
		Node* cur = _head;
		while (cur)
		{
			if (cur->_data == x)
			{
				Node* del = cur;
				cur = cur->_next;
				Erase(del);
				continue;
			}
			cur = cur->_next;
		}
	}

	void Destroy()
	{
		Node* cur = _head;
		Node* del = NULL;
		while (cur)
		{
			del = cur;
			cur = cur->_next;
			delete cur;
			cur = NULL;
		}
	}

	bool Empty()
	{
		if (_head)
			return true;
		return false;
	}

	size_t Size()
	{
		size_t count = 0;
		Node* cur = _head;
		while (cur)
		{
			count++;
			cur = cur->_next;
		}
		return count;
	}

	Node* Back()
	{
		Node* cur = _head;
		while (cur)
		{
			if (cur->_next == NULL)
				return cur;
			cur = cur->_next;
		}
		return NULL;
	}

	Node* Front()
	{
		return _head;
	}

	void PrintListFromTailToHead()
	{
		//_PrintListFromTailToHead(_head);
		_PrintListFromTailToHead_Stack(_head);
	}

	void DeleteNotTail(Node* pos)//���ñ�����ʽɾ����ͷ������ķ�β�ڵ�->����һ���ڵ�����ݸ���ǰ�ڵ��ɾ����һ���ڵ�
	{
		assert(pos);
		Node* cur = pos->_next;
		pos->_data = cur->_data;
		pos->_next = cur->_next;
		delete cur;
		cur = NULL;
	}

	// ʵ��Լɪ��
	T& JosephCircle(size_t M)
	{
		Node* cur = _head;
		while (cur)//���ɻ�
		{
			if (cur->_next == NULL)
			{
				cur->_next = _head;
			}
			cur = cur->_next;
		}

		cur = _head;
		while (1)
		{
			size_t n = M;
			if (cur = cur->_next)
				break;
			while (--n)//ͨ��ѭ���ó���Ҫ�����Ľڵ�
			{
				cur = cur->_next;
			}
			//ɾ�������Ҫ�����Ľڵ�-->���ýڵ����һ���ڵ��ֵ�����ýڵ㣬Ȼ��ɾ����һ���ڵ㣬ʵ��ɾ����ǰ�ڵ��Ŀ��
			Node* del = cur->_next;
			cur->_data = del->_data;
			cur->_next = del->_next;
			delete del;
			del = NULL;
		}
		return cur->_data;
	}

	//�ҵ����м�Ľڵ�-->���ÿ���ָ�룬����ߵ�������2������������꣬���ĸպ��ߵ��м�
	Node* FindMidNode()
	{
		Node* fast = _head;
		Node* slow = _head;
		while (fast && fast->_next)
		{
			fast = fast->_next->_next;
			slow = slow->_next;
		}
		return slow;
	}

	// �ҵ�������k�����-->���ÿ�ָ�����ߵ���k���ڵ㴦��Ȼ�����һ���ߣ������ߵ�β������ʱ�ߵ�������k���ڵ㴦
	Node* FindLastKNode(size_t k)
	{
		Node* fast = _head;
		Node* slow = _head;
		while (fast && k--)
		{
			fast = fast->_next;
		}
		if (k > 0)
			return NULL;

		while (fast)
		{
			fast = fast->_next;
			slow = slow->_next;
		}
		return slow;
	}

	// ����/��ת������
	Node* ReverseList()
	{
		Node* newhead = NULL;
		Node* cur = _head;
		Node* prev = NULL;

		if (_head == NULL)
			return NULL;

		while (cur)
		{
			Node* next = cur->_next;

			if (next == NULL)
				newhead = cur;

			cur->_next = prev;

			prev = cur;
			cur = next;
		}

		return newhead;
	}

	// ð������-->����
	void Bubblesort()
	{
		Node* cur = _head;
		if (_head == NULL || _head->_next == NULL)
			return;
		while (cur)
		{
			Node* next = cur->_next;
			while (next)
			{
				if (cur->_data > next->_data)
				{
					std::swap(cur->_data, next->_data);
				}
				next = next->_next;
			}
			cur = cur->_next;
		}
	}

	//��ӡ����
	void PrintList()
	{
		Node* cur = _head;
		while (cur)
		{
			cout << cur->_data << " ";
			cur = cur->_next;
		}
	}

protected:
	Node* _BuyNode(const T& x)
	{
		Node* tmp = new Node(x);
		tmp->_data = x;
		return tmp;
	}

	void _PrintListFromTailToHead(Node* head)
	{
		if (head)
		{
			_PrintListFromTailToHead(head->_next);
			cout << head->_data << " ";
		}
	}

	//����һ������ջ�ķ���
	void _PrintListFromTailToHead_Stack(Node* head)
	{
		stack<T> s;
		Node* cur = head;
		while (cur)
		{
			s.push(cur->_data);
			cur = cur->_next;
		}
		while (!s.empty())
		{
			cout << s.top() << " ";
			s.pop();
		}
	}

protected:
	Node* _head;
};

// �ж��Ƿ��л������������ĵ�
template <class T>
ListNode<T>* HasCircle(ListNode<T>* head)
{
	if (head == NULL)
		return NULL;

	ListNode<T>* fast = head;
	ListNode<T>* slow = head;
	while (fast && fast->_next)
	{
		fast = fast->_next->_next;
		slow = slow->_next;
		if (slow == fast)
			return fast;
	}
	return NULL;
}

//�ϲ�����-->����������������
template <class T>
ListNode<T>* MergeList(ListNode<T>* list1, ListNode<T>* list2)
{
	if (list1 == NULL)
		return list2;

	if (list2 == NULL)
		return list1;

	if (list1 == list2)
		return list1;

	ListNode<T>* cur = NULL;
	ListNode<T>* newhead = NULL;
	ListNode<T>* head1 = list1;
	ListNode<T>* head2 = list2;

	if (head1->_data < head2->_data)//��ȷ���������ͷ���
	{
		cur = head1;
		head1 = head1->_next;
	}
	else
	{
		cur = head2;
		head2 = head2->_next;	
	}
	newhead = cur;
	while (head1 && head2)//������������Ϊ�յ������
	{
		if (head1->_data <= head2->_data)//�ҵ����������н�С���Ǹ��ڵ㣬��������������ĺ���
		{
			cur->_next = head1;
			head1 = head1->_next;
		}
		else
		{
			cur->_next = head2;
			head2 = head2->_next;
		}
		cur = cur->_next;
	}

	if (head1 == NULL)//����ѭ�����ж�����һ������Ϊ�ճ���ѭ����Ȼ����һ�������������������
		cur->_next = head2;
	else
		cur->_next = head1;

	return newhead;
}

//�ж����������Ƿ��ཻ
template<class T>
bool IsCross(ListNode<T>* head1, ListNode<T>* head2)
{
	if (head1 == NULL || head2 == NULL)
		return false;

	ListNode<T>* p1 = head1;
	ListNode<T>* p2 = head2;
	while (p1->_next)
		p1 = p1->_next;

	while (p2->_next)
		p2 = p2->_next;

	if (p1 == p2)//���������ཻ����ôֻ������y�ͻ�v�ͣ�������x�ͣ�����ֻ��Ҫ�ж����һ���ڵ��Ƿ���Ⱦ���֪���Ƿ��ཻ
		return true;
	else
		return false;
}

//�ж��Ƿ��ཻ(��������˴����ཻ)
template<class T>
bool IsCrossWithCircle(ListNode<T>* head1, ListNode<T>* head2)
{
	if (head1 == NULL || head2 == NULL)
		return false;

	ListNode<T>* meetnode1 = HasCircle(head1);
	ListNode<T>* meetnode2 = HasCircle(head2);

	if (meetnode1 == NULL && meetnode2 == NULL)//1.��������������
	{
		if (IsCross(head1, head2) == true)
			return true;
		else
			return false;
	}
	else if (meetnode1 && meetnode2)//2.������������	
	{
		ListNode<T>* node = meetnode1;
		while (meetnode1->_next != node)//���������ཻ�������϶���ӵ�й�����
		{
			if (meetnode1 == meetnode2)//��������ҵ���һ�����еĽڵ�����һ�������е���������ͬ����ô�Ϳ���ȷ�������ཻ
				return true;
			meetnode1 = meetnode1->_next;
		}
		if (meetnode1 == meetnode2)//����ѭ�����������ж�һ����Ϊ��ȷ�������ڵ�ǰ����Ǹ��ڵ㣬����ı���������ڵ�©��
			return true;
		else
			return false;
	}
	else//3.һ�����������һ�����������������������ǲ����ཻ��
	{
		return false;
	}
}

//��ȡ����
template<class T>
ListNode<T>* GetCrossNode(ListNode<T>* head2, ListNode<T>* head1)
{
	ListNode<T>* p1 = head1;
	ListNode<T>* p2 = head2;

	if (IsCross(head1, head2) == false)//���ж��Ƿ��ཻ�����û���ཻ���Ǿ�ֱ�ӷ��ؿ�
	{
		return NULL;
	}

	int count = 0;
	while (p1)
	{
		count++;
		p1 = p1->_next;
	}
	int s1 = count;//��������������size��
	count = 0;
	while (p2)
	{
		count++;
		p2 = p2->_next;
	}
	int s2 = count;//��������������size��
	
	p1 = head1;
	p2 = head2;
	int i = 0;
	if (s1 - s2 >= 0)//������Ϊ���ҵ�����������ͬ�Ĳ��ֵĴ�С��Ȼ��������һ��������ô�࣬��������һ���ߣ�
	{							//���������ߵ����ʱ����˵���Ѿ��ҵ��ཻ�ĵ��ˣ��Ϳ��Է�����
		i = s1 - s2;
		while (i--)
			p1 = p1->_next;
	}
	else
	{
		i = s2 - s1;
		while (i)
			p1 = p1->_next;
	}
	while (p1 && p2)
	{
		if (p1 == p2)
			break;
		p1 = p1->_next;
		p2 = p2->_next;
	}
	return p2;
}

//��ȡ���ĳ���-->��һ�����ڵĽڵ�
template<class T>
size_t GetCircleLen(ListNode<T>* node)
{
	if (node == NULL)
		return 0;
	size_t count = 1;
	ListNode<T>* cur = node;
	while (node != cur->_next)
	{
		count++;
		cur = cur->_next;
	}
	return count;
}

// ��ȡ������ڵ�
template<class T>
ListNode<T>* GetEnterNode(ListNode<T>* head, ListNode<T>* meetNode)
{
	ListNode<T>* node1 = head;
	ListNode<T>* node2 = meetNode;
	if (head == NULL || meetNode == NULL)
		return NULL;

	while (node1 != node2)
	{
		node1 = node1->_next;
		node2 = node2->_next;
	}
	return node1;
}

//��������������������ͬ������
template<class T>
ListNode<T>* UnionSet(ListNode<T>* l1, ListNode<T>* l2)
{
	ListNode<T>* newhead = NULL;
	ListNode<T>* head1 = l1;
	ListNode<T>* head2 = l2;
	while (head1 && head2)
	{
		if (head1->_data < head2->_data)
			head1 = head1->_next;
		else if (head1->_data>head2->_data)
			head2 = head2->_next;
		else
		{
			newhead->_next = head1;
			head1 = head1->_next;
			head2 = head2->_next;
		}
	}
	return newhead;
}

void TestSList()
{
	SList<int> s1;
	s1.PushBack(1);
	s1.PushBack(2);
	s1.PushBack(3);
	s1.PushBack(4);
	s1.PushBack(7);
	s1.PushBack(8);
	s1.PushBack(5);
	s1.PushBack(6);
	s1.PushBack(9);
	s1.PushBack(10);

	ListNode<int>* node1 = s1.Find(7);
	ListNode<int>* node2 = s1.Back();
	node2->_next = node1;

	ListNode<int>* r = HasCircle(s1.Front());
	cout << GetCircleLen(s1.Find(5)) << endl;

	ListNode<int>* rs = GetEnterNode(s1.Front(), s1.Find(5));

	s1.Bubblesort();
	ListNode<int>* ret = s1.ReverseList();
	s1.RemoveAll(1);
	s1.PrintListFromTailToHead();
	s1.Remove(3);
	s1.Erase(s1.Find(3));
	s1.PopBack();
	s1.PushFront(0);
	s1.PopFront();
	s1.Insert(s1.Find(1), 5);
	cout << s1.Find(1)->_data << endl;
	s1.PrintList();
	SList<int> s2(s1);
	SList<int> s3;
	s3.PushBack(1);
	s3.Remove(1);
	s3.Erase(s3.Find(1));
	s3 = s2;
}

void TestSlist2()
{
	SList<int> s1;
	SList<int> s2;

	s1.PushBack(1);
	s1.PushBack(2);
	s1.PushBack(4);
	s1.PushBack(5);
	s1.PushBack(7);
	s1.PushBack(10);
	s1.PushBack(13);

	s2.PushBack(3);
	s2.PushBack(6);
	s2.PushBack(8);
	s2.PushBack(10);
	s2.PushBack(15);

	ListNode<int>* Node = MergeList(s1.Front(), s2.Front());


	ListNode<int>* node1 = s1.Find(10);
	ListNode<int>* node2 = s2.Back();
	node2->_next = node1;
	cout << IsCross(s1.Front(), s2.Front()) << endl;
	ListNode<int>* ret = GetCrossNode(s1.Front(), s2.Front());
}

void TestSlist3()
{
	SList<int> s1;
	SList<int> s2;

	s1.PushBack(1);
	s1.PushBack(4);
	s1.PushBack(5);
	s1.PushBack(6);
	s1.PushBack(8);


	s2.PushBack(2);
	s2.PushBack(3);
	s2.PushBack(5);
	s2.PushBack(7);
	s2.PushBack(9);
	s2.PushBack(10);

	ListNode<int>* node1 = s1.Find(5);
	ListNode<int>* node2 = s1.Back();
	node2->_next = node1;

	ListNode<int>* node3 = s1.Find(5);
	ListNode<int>* node4 = s2.Back();
	node4->_next = node3;

	cout << IsCrossWithCircle(s1.Front(), s2.Front()) << endl;
}

