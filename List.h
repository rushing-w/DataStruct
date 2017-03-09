#include<iostream>
#include<assert.h>
#include<string.h>
using namespace std;


template<typename T>
struct Node
{
	Node(const T& data)
	: _data(data)
	, _pNext(NULL)
	, _pPre(NULL)
	{}

	T _data;
	Node<T>* _pNext;
	Node<T>* _pPre;
};


template<typename T>
class List
{
public:
	List()
	{
		_pHead = _pTail = NULL;
		_size = 0;
	}
	List(const T arr[], size_t size)
		: _size(size)
		, _pHead(NULL)
		, _pTail(NULL)
	{
		for (size_t i = 0; i < _size; i++)
		{
			if (_pHead == NULL)
			{
				_pHead = _pTail = BuyNewNode(arr[i]);
			}
			else
			{
				Node<T>* pNew = BuyNewNode(arr[i]);
				_pTail->_pNextt = pNew;
				pNew->_pPre = _pTail;
				_pTail = _pTail->_pNext;
			}
		}
	}
	List(const List<T>& l)
		: _size(l._size)
		, _pHead(NULL)
		, _pTail(NULL)
	{
		Node<T>* pNewHead = l._pHead;
		while (pNewHead)
		{
			if (_pHead == NULL)
			{
				_pHead = _pTail = BuyNewNode(pNewHead->_data);
			}
			else
			{
				Node<T>* pNewNode = BuyNewNode(pNewHead->_data);
				_pTail->_pNext = pNewNode;
				pNewNode->_pNext = _pTail;
				_pTail->_pPre = pNewNode;
			}
			pNewHead = pNewHead->_pNext;
		}
	}
	List<T>& operator=(const List<T>& l)
	{
		if (this != &l)
		{
			_size = l._size;
			_pHead = NULL;
			_pTail = NULL;
			Node<T>* pNewHead = l._pHead;
			while (pNewHead)
			{
				if (_pHead == NULL)
				{
					_pHead = _pTail = BuyNewNode(pNewHead->_data);
				}
				else
				{
					Node<T>* pNewNode = BuyNewNode(pNewHead->_data);
					_pTail->_pNext = pNewNode;
					pNewNode->_pNext = _pTail;
					_pTail->_pPre = pNewNode;
				}
				pNewHead = pNewHead->_pNext;
			}
		}
		return *this;
	}
	~List()
	{
		if (_pHead != NULL)
		{
			delete _pHead;
			delete _pTail;
		}
		_pHead = NULL;
		_pTail = NULL;
		_size = 0;
	}
public:
	void PushBack(const T& data)
	{
		if (_size == 0)
		{
			_pHead = _pTail = BuyNewNode(data);
			++_size;
		}
		else
		{
			Node<T>* pNew = BuyNewNode(data);
			_pTail->_pNext = pNew;
			pNew->_pPre = _pTail;
			_pTail = pNew;
			++_size;
		}
	}

	void PopBack()
	{
		if (_size == 0)
		{
			return;
		}
		else if (_size == 1)
		{
			_pHead = _pTail = NULL;
			_size = 0;
		}
		else
		{
			_pTail = _pTail->_pPre;
			_pTail->_pNext = NULL;
			--_size;
		}
	}

	void PushFront(const T& data)
	{
		if (_size == 0)
		{
			_pHead = _pTail = BuyNewNode(data);
			++_size;
		}
		else
		{
			Node<T>* pNew = BuyNewNode(data);
			_pHead->_pPre = pNew;
			pNew->_pNext = _pHead;
			_pHead = pNew;
			++_size;
		}
	}

	void PopFront()
	{
		if (_size == 0)
		{
			return;
		}
		else if (_size == 1)
		{
			_pHead = _pTail = NULL;
			_size = 0;
		}
		else
		{
			_pHead = _pHead->_pNext;
			_pHead->_pPre = NULL;
			--_size;
		}
	}

	Node<T>* Find(const T& data)
	{
		Node<T>* pCur = _pHead;
		while (pCur)
		{
			if (pCur->_data == data)
			{
				return pCur;
			}
			pCur = pCur->_pNext;
		}
		return NULL;
	}

	void Insert(Node<T>* pos, const T& data)
	{
		assert(pos);
		if (pos->_pNext == NULL)
		{
			PushBack(data);
		}
		else
		{
			Node<T>* pNew = BuyNewNode(data);
			pNew->_pNext = pos->_pNext;
			pos->_pNext = pNew;
			pNew->_pPre = pos;
			++_size;
		}
	}

	void Erase(Node<T>* pos)
	{
		assert(pos);
		if (pos->_pNext == NULL)
		{
			PopBack();
		}
		else if (pos->_pPre == NULL)
		{
			PopFront();
		}
		else
		{
			pos->_pPre->_pNext = pos->_pNext;
			pos->_pNext->_pPre = pos->_pPre;
			--_size;
		}
	}

	Node<T>& Front()
	{
		return *_pHead;
	}

	const Node<T>& Front()const
	{
		return *_pHead;
	}

	Node<T>& Back()
	{
		return *_pTail;
	}

	const Node<T>& Back()const
	{
		return *_pTail;
	}

	void Clear()
	{
		_pHead = _pTail = NULL;
		_size = 0;
	}

	size_t Size()const
	{
		return _size;
	}

	bool Empty()const
	{
		if (_size == 0)
		{
			return true;
		}
		return false;
	}


private:
	Node<T>* BuyNewNode(const T& data)
	{
		Node<T>* NewNode = new Node<T>(data);
		NewNode->_data = data;
		return NewNode;
	}

private:
	Node<T>* _pHead;
	Node<T>* _pTail;
	size_t _size;
};
