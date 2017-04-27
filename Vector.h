#pragma once

#include<iostream>
using namespace std;
#include"TypeTraits.h"
#include<assert.h>

template<class T>
class Vector
{
public:
	Vector()
		: _arr(new T[3])
		, _size(0)
		, _capacity(3)
	{}

	Vector(const T* arr, size_t size)
		: _arr(new T[size])
		, _size(size)
		, _capacity(size)
	{
		if (TypeTraits<T>::__IsPODType().Get() == true)
		{
			memcpy(_arr, v._arr, v._size*sizeof(T));
		}
		else
		{
			for (size_t i = 0; i < size; ++i)
			{
				_arr[i] = arr[i];
			}
		}
	}

	Vector(const Vector<T>& v)
		: _arr(new T[v._capacity])
		, _size(v._size)
		, _capacity(v._capacity)
	{
		if (TypeTraits<T>::__IsPODType().Get() == true)
		{
			memcpy(_arr, v._arr, v._size*sizeof(T));
		}
		else
		{
			for (size_t i = 0; i < v._size; ++i)
			{
				_arr[i] = v._arr[i];
			}
		}
	}

	Vector<T>& operator=(const Vector<T>& v)
	{
		if (this != &v)
		{
			_size = v._size;
			_capacity = v._capacity;
			T* tmp = new T[v._capacity];
			if (TypeTraits<T>::__IsPODType().Get() == true)
			{
				memcpy(tmp, v._arr, v._size*sizeof(T));
			}
			else
			{
				for (size_t i = 0; i < v._size; ++i)
				{
					tmp[i] = v._arr[i];
				}
			}
			delete[] _arr;
			_arr = tmp;
		}
		return *this;
	}
	
	~Vector()
	{
		if (_arr)
			delete[] _arr;
		_arr = NULL;
		_size = 0;
		_capacity = 0;
	}

	void PushBack(const T& data)
	{
		_CheckCapacity();
		_arr[_size++] = data;
	}

	void PopBack()
	{
		if (_size == 0)
			return;
		--_size;
	}

	void PushFront(const T& data)
	{
		size_t i = _size;
		_CheckCapacity();
		while (i > 0)
		{
			_arr[i] = _arr[i - 1];
			i--;
		}
		_arr[0] = data;
		_size++;
	}

	void PopFront()
	{
		if (_size == 0)
			return;
		size_t i = 0;
		while (i < _size)
		{
			_arr[i] = _arr[i + 1];
			i++;
		}
		_size--;
	}

	void Insert(const T& data, size_t pos)
	{
		if (pos > _size)
			return;
		_CheckCapacity();

		for (size_t i = _size; i > pos; --i)
		{
			_arr[i] = _arr[i - 1];
		}
		_arr[pos] = data;
		_size++;
	}

	void Erase(size_t pos)
	{
		if (pos>_size)
			return;
		for (size_t i = pos; i < _size; ++i)
		{
			_arr[i] = _arr[i + 1];
		}
		--_size;
	}

	bool Empty()const
	{
		if (_size == 0)
		{
			return true;
		}
		return false;
	}

	size_t Size()const
	{
		return _size;
	}

	size_t Capacity()const
	{
		return _capacity;
	}

	void Clear()
	{
		delete[] _arr;
		_arr = NULL;
		_size = 0;
		_capacity = 0;
	}

	void Resize(size_t size, const T data = T())
	{
		if (_size >= size)
		{
			_size = size;
		}
		else
		{
			_CheckCapacity();
			size_t oldsize = _size;
			for (size_t i = oldsize; i < size; ++i)
			{
				_arr[i] = data;
			}
			_size = size;
		}
	}

	void Reverse(size_t newcapacity)
	{
		if (_capacity >= newcapacity)
		{
			if (newcapacity < _size)
			{
				_size = newcapacity;
			}
			_capacity = newcapacity;
		}
		else
		{
			T* tmp = new T[newcapacity];
			for (size_t i = 0; i < _size; ++i)
			{
				tmp[i] = _arr[i];
			}
			delete[] _arr;
			_arr = tmp;
			
			_capacity = newcapacity;
		}
	}

	T& operator[](size_t index)
	{
		assert(index < _size);
		return _arr[index];
	}

	const T& operator[](size_t index)const
	{
		assert(index < _size);
		return _arr[index];
	}

	T& Front()
	{
		return _arr[0];
	}

	const T& Front()const
	{
		return _arr[0];
	}

	T& Back()
	{
		return _arr[_size - 1];
	}

	const T& Back()const
	{
		return _arr[_size - 1];
	}

protected:
	void _CheckCapacity()
	{
		if (_size >= _capacity)
		{
			T* tmp = new T[_capacity * 2];
			if (TypeTraits<T>::__IsPODType().Get() == true)
			{
				memcpy(tmp, _arr, _capacity*sizeof(T));
			}
			else
			{
				for (size_t i = 0; i < _size; i++)
				{
					tmp[i] = _arr[i];
				}
			}
			delete[] _arr;
			_arr = tmp;
			_capacity *= 2;
		}
	}

protected:
	T* _arr;
	size_t _capacity;
	size_t _size;
};

void TestVector()
{
	Vector<int> v;
	v.PushBack(1);
	v.PushBack(2);
	v.PushBack(3);
	v.PushBack(4);
	v.Reverse(10);
	v.Resize(6);

	int ret1 = v.Front();
	int ret2 = v.Back();
	v.PopBack();
	v.PushFront(0);
	v.PopFront();
	cout << v.Size() << endl;
	cout << v.Capacity() << endl;
	v.Insert(10, 3);
	v.Erase(3);
	cout << v.Empty() << endl;
	v.Clear();
	cout << v.Empty() << endl;
	

	Vector<int> v1(v);
	Vector<int> v2;
	v2 = v;

}