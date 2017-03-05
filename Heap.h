#pragma once

#include <vector>
#include <assert.h>

template<class T>
struct Less
{
	bool operator()(const T& l, const T& r)
	{
		return l < r;
	}
};

template<class T>
struct Greater
{
	bool operator()(const T& l, const T& r)
	{
		return l > r;
	}
};

template <class T, class Compare = Greater<T>>
class Heap
{
public:
	Heap()
	{}

	Heap(T* a, size_t n)
		: _a(a, a + n)
	{
		for (int i = (_a.size() - 2) / 2; i >= 0; i--)
		{
			_AdjustDown(i);
		}
	}

	void Push(const T& x)
	{
		_a.push_back(x);
		_AdjustUp(_a.size() - 1);
	}

	void Pop()
	{
		swap(_a[0], _a[_a.size() - 1]);
		_a.pop_back();
		_AdjustDown(0);
	}

	size_t Size()
	{
		return _a.size();
	}

	bool Empty()
	{
		return _a.empty();
	}

	T& Top()
	{
		assert(_a.empty());
		return _a[0];
	}

protected:
	void _AdjustDown(int root)//根的左右子树是子问题
	{
		Compare cmp;
		int parent = root;
		int child = 2 * root + 1;
		while (child < _a.size())
		{
			if (child + 1 < _a.size() && cmp(_a[child + 1], _a[child]))
			{
				child++;
			}
			if (cmp(_a[child], _a[parent]))
			{
				swap(_a[child], _a[parent]);
				parent = child;
				child = parent * 2 + 1;
			}
			else
			{
				break;
			}
		}
	}

	void _AdjustUp(int child)
	{
		Compare cmp;
		int parent = (child - 1) / 2;
		while (child > 0)
		{
			if (cmp(_a[child], _a[parent]))
			{
				swap(_a[child], _a[parent]);
				child = parent;
				parent = (child - 1) / 2;
			}
			else
			{
				break;
			}
		}
	}

protected:
	vector<T> _a;
};

template<class T, class Compare = Greater<int>>
class PriorityQueue
{
public:
	void Push(const T& x)
	{
		_hp.Push(x);
	}

	void Pop()
	{
		_hp.Pop();
	}

	void Top()
	{
		_hp.Top();
	}

	size_t Size()
	{
		return _hp.Size();
	}

	bool Empty()
	{
		return _hp.Empty();
	}

protected:
	Heap<T, Compare> _hp;
};

void TestHeap()
{
	int a[] = { 10, 11, 13, 12, 16, 18, 15, 17, 14, 19 };
	Heap<int, Less<int>> hp1(a, sizeof(a) / sizeof(a[0]));
	hp1.Push(20);
	hp1.Pop();
}

void AdjustDown(int *a, int n, int k)
{
	int parent = k;
	int child = 2 * k + 1;
	while (child < n)
	{
		if (child + 1 < n && a[child] < a[child + 1])
		{
			child++;
		}
		if (a[child] > a[parent])
		{
			swap(a[child], a[parent]);
			parent = child;
			child = 2 * parent + 1;
		}
		else
		{
			break;
		}
	}
}

void GetTopK(int* a, const int n, const int k)
{
	assert(n > k && a);
	int* heap = new int[k];
	for (size_t i = 0; i < k; i++)
	{
		heap[i] = a[i];
	}
	for (int i = 0; i>(k - 2) / 2; i--)
	{
		AdjustDown(heap, k, i);
	}
	for (size_t i = k; i < n; i++)
	{
		if (heap[0] < a[i])
		{
			heap[0] = a[i];
		}
		AdjustDown(heap, k, 0);
	}
	
	for (size_t i = 0; i < k; i++)//打印数据
	{
		cout << heap[i] << " ";
	}
	cout << endl;

	delete[] heap;//开辟后不要忘记释放
}

void TestTopK()
{
	const int N = 1000, K = 5;
	int a[N];
	for (size_t i = 0; i < N; ++i)
	{
		a[i] = rand() % N;
	}

	a[0] = N + 1;
	a[10] = N + 2;
	a[100] = N + 3;
	a[99] = N + 4;
	a[50] = N + 5;


	GetTopK(a, N, K);
}

//排序 降序
void HeapSort(int* a, int n)
{
	assert(n);
	for (int i = (n - 2) / 2; i >= 0; --i)
	{
		AdjustDown(a, n, i);
	}

	int end = n - 1;
	while (end > 0)
	{
		swap(a[0], a[end]);
		AdjustDown(a, end, 0);
		--end;
	}
}
