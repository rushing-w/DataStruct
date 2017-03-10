#include<iostream>
#include<string.h>
#include<assert.h>
using namespace std;

bool CheckPODType(const char* TypeName)
{
	const char* arr[] = { "int", "char", "double" };
	for (size_t i = 0; i < 3; i++)
	{
		if (strcmp(TypeName, arr[i]) == 0)
		{
			return true;
		}
	}
	return false;
}

template<typename T>
class Vector
{
public:
	Vector()
		: _pData(new T[3])
		, _size(0)
		, _capacity(3)
	{}
	Vector(const T arr[], size_t size)
		: _pData(new T[size])
		, _size(size)
		, _capacity(size)
	{
		for (int i = 0; i < size; i++)
		{
			_pData[i] = arr[i];
		}
	}
	Vector(const Vector& v)
		: _pData(v._pData)
		, _size(v._size)
		, _capacity(v._capacity)
	{
		for (int i = 0; i < v._size; i++)
		{
			_pData[i] = v._pData[i];
		}
	}
	Vector& operator=(const Vector& s)
	{
		if (this != &s)
		{
			_size = s._size;
			_capacity = s._capacity;
			T* pTmp = new T[s._capacity];
			for (int i = 0; i < s._size; i++)
			{
				pTmp[i] = s._pData[i];
			}
			delete[] _pData;
			_pData = pTmp;
		}
		return *this;
	}
	~Vector()
	{
		if (NULL != _pData)
		{
			delete[] _pData;
		}
		_pData = NULL;
		_size = 0;
		_capacity = 0;
	}

public:
	void PushBack(const T& data)
	{
		_CheckCapacity();
		_pData[_size++] = data;
	}

	void PopBack()
	{
		if (_size == 0)
		{
			return;
		}
		--_size;
	}

	void Insert(size_t pos, const T& data)
	{
		assert(pos <= _size);
		_CheckCapacity();
		for (size_t i = _size; i > pos; i--)
		{
			_pData[i] = _pData[i - 1];
		}
		_pData[pos] = data;
		++_size;
	}

	void Erase(size_t pos)
	{
		assert(pos <= _size);
		for (size_t i = pos; i < _size; i++)
		{
			_pData[i] = _pData[i + 1];
		}
		--_size;
	}

	const T& operator[](size_t index)const
	{
		assert(index < _size);
		return _pData[index];
	}

	T& operator[](size_t index)
	{
		assert(index < _size);
		return _pData[index];
	}

	T& Front()
	{
		return _pData[0];
	}

	const T& Front()const
	{
		return _pData[0];
	}

	T& Back()
	{
		return _pData[_size];
	}

	const T& Back()const
	{
		return _pData[_size];
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
		delete[] _pData;
		_pData = NULL;
		_size = 0;
		_capacity = 0;
	}

	void Resize(size_t size, const T data = T())
	{
		if (_size > size)
		{
			_size = size;
		}
		else if (_size < size)
		{
			_CheckCapacity();
			size_t oldsize = _size;
			_size = size;
			for (size_t i = oldsize; i < _size; i++)
			{
				_pData[i] = data;
			}
		}
	}


private:
	void _CheckCapacity()
	{
		if (_size >= _capacity)
		{
			T* pTmp = new T[_capacity * 2];
			if (CheckPODType(typeid(T).name()) == true)
			{
				memcpy(pTmp, _pData, sizeof(T)*_capacity);
			}
			else
			{
				for (size_t i = 0; i < _size; i++)
				{
					pTmp[i] = _pData[i];
				}
			}
			delete[] _pData;
			_pData = pTmp;
			_capacity *= 2;
		}
	}
private:
	T* _pData;
	size_t _capacity;
	size_t _size;
};
