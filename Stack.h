#include<iostream>
#include<assert.h>
using namespace std;

template<class T>
class Stack
{
public:
	Stack()
		:_data(NULL)
		, _size(0)
		, _capacity(0)
	{}

	Stack(const Stack& s)
		:_data(new T[s._capacity])
		, _size(s._size)
		, _capacity(s._capacity)
	{
		for (size_t i = 0; i<s._size; ++i)
		{
			_data[i] = s._data[i];
		}
	}

	Stack& operator=(const Stack& s)
	{
		if (this != &s)
		{
			T* tmp = new T[s._capacity];
			for (size_t i = 0; i<s._size; ++i)
			{
				tmp[i] = s._data[i];
			}
			delete[] _data;
			_data = tmp;
			_size = s._size;
			_capacity = s._capacity;
		}
		return *this;
	}

	~Stack()
	{
		if (_data != NULL)
		{
			delete[] _data;
		}
		_data = NULL;
		_size = 0;
		_capacity = 0;
	}

	void Push(const T& data)
	{
		_CheckCapacity();
		_data[_size++] = data;
	}

	void Pop()
	{
		assert(_size);
		_size--;
	}
	bool Empty()
	{
		return _size == 0;
	}

	size_t Size()
	{
		return _size;
	}

	T& Top()
	{
		return _data[_size - 1];
	}


protected:
	void _CheckCapacity()
	{
		if (_size >= _capacity)
		{
			size_t newcapacity = 2 * _capacity + 3;
			T* tmp = new T[newcapacity];

			/*memcpy(tmp,_data,_size*sizeof(T));*/
			for (size_t i = 0; i<_capacity; ++i)
			{
				tmp[i] = _data[i];
			}

			delete[] _data;
			_data = tmp;
			_capacity = newcapacity;
		}
	}

	T* _data;
	size_t _size;
	size_t _capacity;
};