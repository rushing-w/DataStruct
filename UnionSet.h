#pragma once

#include<iostream>
using namespace std;
#include<vector>

class UnionFindSet
{
public:
	UnionFindSet(size_t n)
		: _n(n)
	{
		_parent = new int[_n];
		for (size_t i = 0; i < n; ++i)
		{
			_parent[i] = -1;
		}
	}

	~UnionFindSet()
	{
		delete[] _parent;
	}

	size_t FindRoot(size_t x)
	{
		size_t cur = x;
		while (_parent[cur] >= 0)
		{
			cur = _parent[cur];
		}

		return cur;
	}

	bool IsInSet(size_t x1, size_t x2)
	{
		return FindRoot(x1) == FindRoot(x2);
	}

	void Union(size_t x1, size_t x2)
	{
		size_t root1 = FindRoot(x1);
		size_t root2 = FindRoot(x2);

		if (root1 != root2)
		{
			_parent[root1] += _parent[root2];
			_parent[root2] = root1;
		}
	}

	void Print()
	{
		for (size_t i = 0; i < _n; ++i)
		{
			cout << _parent[i] << " ";
		}
		cout << endl;
	}
		
protected:
	int* _parent;
	size_t _n;
};

void TestUnionFindSet()
{
	UnionFindSet u(10);

	u.Union(0, 6);
	u.Union(0, 7);
	u.Union(0, 8);

	u.Union(1, 9);
	u.Union(1, 4);

	u.Union(2, 3);
	u.Union(2, 5);

	u.Print();
}