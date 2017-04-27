#pragma once

#include<iostream>
using namespace std;
#include<assert.h>
#include"TypeTraits.h"

template <class T>
class Vector
{
public:
	typedef T* Iterator;//顺序表的迭代器是原生指针
	typedef const T* ConstIterator;

public:
	Vector()
		: _start(0)
		, _finish(0)
		, _endOfStorage(0)
	{}

	Vector(const Vector<T>& v)
		: _start(new T[v.Size()])
	{
		_finish = _start + v.Size();
		_endOfStorage = _start + v.Size();
		if (TypeTraits<T>::__IsPODType().Get() == true)
		{
			memcpy(_start, v._start, v.Size()*sizeof(T));
		}
		else
		{
			for (size_t i = 0; i < v.Size(); ++i)
			{
				_start[i] = v._start[i];
			}

		}
	}

	Vector<T>& operator=(Vector<T> v)//传参时直接传一份临时拷贝，然后在释放时就不用手动释放了，直接由系统回收
	{
		std::swap(_start, v._start);
		std::swap(_finish, v._finish);
		std::swap(_endOfStorage, v._endOfStorage);
		return *this;
	}

	~Vector()
	{
		if (_start)
			delete[] _start;

		_start = _finish = _endOfStorage = NULL;
	}

	const T& operator[](size_t index)const
	{
		return _start[index];
	}

	void PushBack(const T& x)
	{
		_CheckendOfStorage();
		_start[Size()] = x;
		++_finish;
	}

	void PopBack()
	{
		assert(Size());
		--_finish;
	}

	void PushFront(const T& x)
	{
		size_t i = Size();
		_CheckendOfStorage();
		while (i > 0)
		{
			_start[i] = _start[i - 1];
			i--;
		}
		_start[0] = x;
		++_finish;
	}

	void PopFront()
	{
		assert(Size());
		size_t i = 0;
		while (i < Size())
		{
			_start[i] = _start[i + 1];
			i++;
		}
		_finish--;
	}

	void Insert(Iterator pos, const T& x)
	{
		assert(pos && *pos <= Size());
		_CheckendOfStorage();
		size_t size = Size();
		for (size_t i = size; i > *pos; i--)
		{
			_start[i] = _start[i - 1];
		}
		_start[*pos] = x;
		++_finish;
	}

	void Erase(Iterator pos)
	{
		assert(pos && *pos <= Size());
		size_t size = Size();
		for (size_t i = *pos; i < size; ++i)
		{
			_start[i] = _start[i + 1];
		}
		--_finish;
	}

	Iterator Begin()
	{
		return	_start;
	}

	ConstIterator Begin()const
	{
		return _start;
	}

	Iterator End()
	{
		return _finish;
	}

	ConstIterator End()const
	{
		return _finish;
	}

	size_t Size()
	{
		return _finish - _start;
	}

	size_t Size()const
	{
		return _finish - _start;
	}

	size_t Capacity()
	{
		return _endOfStorage - _start;
	}

	size_t Capacity()const
	{
		return _endOfStorage - _start;
	}

	void Resize(size_t size, T n)
	{
		if (size > Size())
		{
			T* tmp = new T[size];
			size_t oldsize = Size();
			size_t capacity = Capacity();
			for (size_t i = 0; i < oldsize; i++)
			{
				tmp[i] = _start[i];
			}
			for (size_t i = oldsize; i < size; i++)
			{
				tmp[i] = n;
			}
			delete[] _start;
			_start = tmp;
			_finish = _start + newsize;

			if (size > Capacity())
				_endOfStorage = _start + size;
			else
				_endOfStorage = _start + capacity;
		}
		else
		{
			_finish = _start + size;
		}
	}

	void Reverse(size_t newcapacity)
	{
		if (newcapacity <= Capacity())
		{
			if (newcapacity < Size())
			{
				_finish = _start + newcapacity;
			}
			_endOfStorage = _start + newcapacity;
		}
		else
		{
			T* tmp = new T[newcapacity];
			size_t size = Size();
			for (size_t i = 0; i < size; ++i)
			{
				tmp[i] = _start[i];
			}
			delete[] _start;
			_start = tmp;
			_finish = _start + size;
			_endOfStorage = _start + newcapacity;
		}
	}

	void Clear()
	{
		{
			Vector<T> tmp;//利用临时变量，出了里面这个作用域就销毁
			std::swap(tmp, *this);
		}
	}

protected:
	void _CheckendOfStorage()
	{
		size_t size = Size();
		size_t capacity = Capacity();

		if (_finish == _endOfStorage)
		{
			size_t newcapacity = capacity * 2 + 3;
			T* tmp = new T[newcapacity];
			for (size_t i = 0; i < size; ++i)
			{
				tmp[i] = _start[i];
			}
			delete[] _start;
			_start = tmp;
			_finish = _start + size;
			_endOfStorage = _start + newcapacity;
		}

	}
protected:
	Iterator _start;
	Iterator _finish;
	Iterator _endOfStorage;
};

