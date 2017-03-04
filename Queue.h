#include<iostream>
#include<assert.h>

using namespace std;

template<class T>
struct QueueNode
{
	QueueNode()
	:_data(0)
	,_next(NULL)
	{}

	T _data;
	QueueNode<T>* _next;
};

template<class T>
class Queue
{

public:
	Queue()
		:_head(NULL)
		,_tail(NULL)
	{}

	/*Queue(const T& data)
		:_head(NULL)
		,_tail(NULL)
	{
		if (_head == NULL)
		{
			_head = _tail = BuyNewNode(data);
		}
		else
		{
			QueueNode<T>* Cur = _head;
			while (Cur)
			{
				if (Cur->_next == NULL)
				{
					QueueNode<T>* New = BuyNewNode(data);
					_tail->_next = New;
					_tail = _tail->_next;
				}
				Cur = Cur->_next;
			}
		}
	}*/

	~Queue()
	{
		if (_head != NULL)
		{
			delete _head;
			delete _tail;
		}
		_head = NULL;
		_tail = NULL;
	}

	void Push(const T& data)
	{
		QueueNode<T>* tmp = BuyNewNode(data);
		if (_head == NULL)
		{
			_head = _tail = tmp;
		}
		else
		{
			_tail->_next = tmp;
			_tail = _tail->_next;
		}
	}

	void Pop()
	{
		QueueNode<T>* tmp = _head;
		_head = _head->_next;
		delete tmp;
		tmp = NULL;
	}

	size_t Size()
	{
		size_t count = 0;
		QueueNode<T>* Cur = _head;
		while (Cur)
		{
			count++;
			Cur = Cur->_next;
		}
		return count;
	}

	bool Empty()
	{
		if (_head == NULL)
		{
			return true;
		}
		return false;
	}

	T& Back()
	{
		return _tail->_data;
	}

protected:

	QueueNode<T>* BuyNewNode(const T& data)
	{
		QueueNode<T>* NewNode = new QueueNode<T>;
		NewNode->_data = data;
		return NewNode;
	}

	QueueNode<T>* _head;
	QueueNode<T>* _tail;
};