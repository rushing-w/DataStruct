#pragma once

#include<iostream>
#include<assert.h>
#include<vector>
using namespace std;

template<class T>
class SymmetricMatrix //对称矩阵
{
public:
	SymmetricMatrix(T* a, size_t n)
		: _a(new T[n*(n + 1) / 2])
		, _n(n)
	{
		assert(a);
		size_t index = 0;
		for (size_t i = 0; i < n; ++i)
		{
			for (size_t j = 0; j < n; ++j)
			{
				if (i >= j)
				{
					_a[index++] = a[i*n + j];
				}
				else
					continue;
			}
		}
	}

	~SymmetricMatrix()
	{
		if (_a)
		{
			delete[] _a;
		}
		_a = NULL;
		_n = 0;
	}

	T& Access(size_t i, size_t j)
	{
		if (i < j)
		{
			swap(i, j);
		}
		return _a[i*(i + 1) / 2 + j];
	}

	void Display()
	{
		for (size_t i = 0; i < _n; ++i)
		{
			for (size_t j = 0; j < _n; ++j)
			{
				cout << Access(i, j) << " ";
			}
			cout << endl;
		}
		cout << endl;
	}

protected:
	T* _a;//数组指针
	size_t _n;//数组大小
};


template<class T>
struct Triple
{
	T _value;
	size_t _row;
	size_t _col;

	Triple(size_t row = 0, size_t col = 0, const T& value = T())//带上默认参数
		: _row(row)
		, _col(col)
		, _value(value)
	{}
};

template<class T>
class SparseMatrix
{
public:
	SparseMatrix(T* a = 0, size_t m = 0, size_t n = 0, const T& invalid = T())
		: _a(vector<Triple<T>>())
		, _m(m)
		, _n(n)
		, _invalid(invalid)
	{
		for (size_t i = 0; i < _m; i++)
		{
			for (size_t j = 0; j < _n; j++)
			{
				if (a[i*n + j] != _invalid)
				{
					_a.push_back(Triple<T>(i, j, a[i*n + j]));
				}
			}
		}
	}
	
	SparseMatrix<T> Transport()
	{
		SparseMatrix<T> sm;
		sm._m = _n;
		sm._n = _m;
		sm._invalid = _invalid;
		sm._a.reserve(_a.size());
		
		size_t index = 0;
		while (sm._a.size() < _a.size())
		{
			for (size_t i = 0; i < _a.size(); ++i)
			{
				if (_a[i]._col == index)
				{
					sm._a.push_back(Triple<T>(_a[i]._col, _a[i]._row, _a[i]._value));
				}
			}
			index++;
		}
		return sm;
	}

	SparseMatrix<T> FastTransport()
	{
		int* count = new int[_n];
		int* start = new int[_n];

		SparseMatrix<T> sm;
		sm._m = _n;
		sm._n = _m;
		sm._invalid = _invalid;
		sm._a.resize(_a.size());

		for (size_t i = 0; i < _n; ++i)
		{
			size_t index = 0;
			for (size_t j = 0; j < _a.size(); ++j)
			{
				if (_a[j]._col == i)
				{
					index++;
				}
			}
			count[i] = index;
		}

		start[0] = 0;
		size_t index = 1;
		while (index < _n)
		{
			start[index] = start[index - 1] + count[index - 1];
			++index;
		}

		index = 0;
		while (index < _a.size())
		{
			sm._a[start[_a[index]._col]++] = { _a[index]._col, _a[index]._row, _a[index]._value };
			index++;
		}


		delete[] count;
		delete[] start;
		count = NULL;
		start = NULL;

		return sm;
	}

	void Display()
	{
		size_t index = 0;
		for (size_t i = 0; i < _m; ++i)
		{
			for (size_t j = 0; j < _n; ++j)
			{
				
				if (index < _a.size() && i == _a[index]._row && j == _a[index]._col)
				{
					cout << _a[index]._value << " ";
					++index;
				}
				else
				{
					cout << _invalid << " ";
				}
			}
			cout << endl;
		}
		cout << endl;
	}
protected:
	vector<Triple<T>> _a;
	size_t _m;
	size_t _n;
	T _invalid;
};


void TestSymmetricMatrix()
{
	int a[5][5] =
	{
		{ 0, 1, 2, 3, 4 },
		{ 1, 0, 1, 2, 3 },
		{ 2, 1, 0, 1, 2 },
		{ 3, 2, 1, 0, 1 },
		{ 4, 3, 2, 1, 0 },
	};

	SymmetricMatrix<int> s((int*)a, 5);
	s.Display();
}

void TestSparseMatrix()
{
	int array[6][5] = { 
	{ 1, 0, 3, 0, 5 },
	{ 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0 },
	{ 2, 0, 4, 0, 6 },
	{ 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0 } };
	SparseMatrix<int> s((int*)array, 6, 5, 0);
	s.Display();
	SparseMatrix<int> s1 = s.Transport();
	s1.Display();
	SparseMatrix<int> s2 = s.FastTransport();
	s2.Display();
}
