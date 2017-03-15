#pragma once

#include<iostream>
using namespace std;
#include<assert.h>
#include<stack>

void Print(int* a, size_t n)
{
	assert(a);
	for (size_t i = 0; i < n; ++i)
	{
		cout << a[i] << " ";
	}
	cout << endl;
}
	
//直接插入排序
void InsertSort(int* a, size_t n)
{
	assert(a);
	for (size_t i = 1; i < n; ++i)
	{
		int end = i - 1;//用end标记这个数
		int tmp = a[i];
		while (end >= 0)
		{
			if (a[end] > tmp)
			{
				a[end + 1] = a[end];
				end--;
			}
			else
			{
				break;
			}
		}
		a[end + 1] = tmp;
	}
}

void TestInsertSort()
{
	int a[] = { 2, 5, 4, 9, 3, 6, 8, 7, 1, 0 };
	InsertSort(a, sizeof(a) / sizeof(a[0]));
	Print(a, sizeof(a) / sizeof(a[0]));
}


//希尔排序
void ShellSort(int* a, size_t n)
{
	assert(a);
	int gap = n;
	while (gap > 1)
	{
		gap = gap / 3 + 1;
		for (size_t i = gap; i < n; ++i)
		{
			int tmp = a[i];
			int end = i - gap;
			while (end >= 0)
			{
				if (a[end] > tmp)
				{
					a[end + gap] = a[end];
					end -= gap;
				}
				else
				{
					break;
				}
			}
			a[end + gap] = tmp;
		}
	}
}

void TestShellSort()
{
	int a[] = { 2, 5, 4, 9, 3, 6, 8, 7, 1, 0 };
	ShellSort(a, sizeof(a) / sizeof(a[0]));
	Print(a, sizeof(a) / sizeof(a[0]));
}

//选择排序
void SelectSort(int* a, int n)
{
	assert(a);
	int i = 0; 
	int j = 0;
	int min = 0;
	for (j = 0; j < n - 1; j++)
	{
		for (i = j + 1; i < n; ++i)
		{
			if (a[min] > a[i])
			{
				min = i;
			}
		}	

		if (min != j)
		{
			swap(a[min], a[j]);
		}
	}
	
}

void TestSelectSort()
{
	int a[] = { 2, 5, 4, 9, 3, 6, 8, 7, 1, 0 };
	SelectSort(a, sizeof(a) / sizeof(a[0]));
	Print(a, sizeof(a) / sizeof(a[0]));
}


//堆排序
void AdJustDown(int* a, size_t n, size_t k)
{
	assert(a);
	int parent = k;
	int child = 2 * k + 1;

	while (child < n)
	{
		if (child + 1 < n && a[child + 1] > a[child])//判断左右孩子哪个大，然后得到较大的一个孩子
		{
			child++;
		}

		if (a[child] > a[parent])//(建大堆)如果孩子节点的值比父节点大，那么交换，向下进行调整，利用循环不断调整
		{							//如果建小堆的话就将这里的大于改成小于就好
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

void HeapSort(int* a, size_t n)//堆排序，如果升序则需要大堆，将最大值放到堆顶，然后将其与最后一个叶子节点的值交换
{
	assert(a);
	//建堆
	for (int i = (n - 2) / 2; i >= 0; --i)
	{
		AdJustDown(a, n, i);
	}

	//排序
	int end = n - 1;//用end标记最后一个叶子节点
	while (end > 0)
	{
		swap(a[0], a[end]);//每一次交换完，将堆顶的最大值和最后一个叶子节点的值交换后，
		AdJustDown(a, end, 0); // 就将这个节点的值取出，然后再将其余的数重新调整
		--end;
	}
}

void TestHeapSort()
{
	int a[] = { 2, 5, 4, 9, 3, 6, 8, 7, 1, 0 };
	HeapSort(a, sizeof(a) / sizeof(a[0]));
	Print(a, sizeof(a) / sizeof(a[0]));
}


//冒泡排序
void BubbleSort(int* a, int n)
{
	for (int i = 0; i < n - 1; ++i)
	{
		for (int j = i + 1; j < n; ++j)
		{
			if (a[i] > a[j])
			{
				swap(a[i], a[j]);
			}
		}
	}
}

//当然，冒泡排序是存在优化的，如果它冒了一半就已经有序了，那么你就可以停止循环了，这样就减少了开销
void BubbleSort_Y(int* a, int n)
{
	int flag = 1;
	for (int i = 0; i < n - 1 && flag; ++i)
	{
		flag = 0;
		for (int j = i + 1; j < n; ++j)
		{
			if (a[i] > a[j])
			{
				swap(a[i], a[j]);
				flag = 1;
			}
		}
	}
}

void TestBubbleSort()
{
	int a[] = { 2, 5, 4, 9, 3, 6, 8, 7, 1, 0 };
	BubbleSort(a, sizeof(a) / sizeof(a[0]));
	Print(a, sizeof(a) / sizeof(a[0]));
}

//快速排序

//快排的优化
//1.三数取中，避免最坏的情况O(N^2)，如果每一次取到的key值都是最大或最小，那么就会导致最坏的情况
int GetMidIndex(int* a, int left, int right)
{
	int mid = left - ((left - right) >> 1);
	if (a[mid] < a[left])
	{
		if (a[mid] > a[right])
		{
			return mid;
		}
		else if (a[left] < a[right])
		{
			return left;
		}
		else
		{
			return right;
		}
	}
	else //a[mid]>=a[left]
	{
		if (a[mid] < a[right])
		{
			return mid;
		}
		else if (a[left]>a[right])
		{
			return left;
		}
		else
		{
			return right;
		}
	}
}

//2.小区间优化，就是当划分开的区间已经比较小的时候，如果继续使用递归，那么会产生较大的压栈的开销，这个时候我们
// 可以通过使用直接插入排序来减小压栈的开销，使用直接插入排序的原因是划分几次之后数据是接近有序，直接插入排序效率更高

//快排的三种方式
//1.左右指针交换法
int PartSort_1(int* a, int left, int right)
{
	int mid = GetMidIndex(a, left, right);
	swap(a[mid], a[right]);

	int key = right;
	while (left < right)
	{
		while (left < right && a[left] <= a[key])
		{
			left++;
		}

		while (left < right && a[right] >= a[key])
		{
			right--;
		}

		if (left < right)
		{
			swap(a[left], a[right]);
		}

	}
	swap(a[left], a[key]);//与key位置的值交换
	return left;
}

//2.挖坑法
int PartSort_2(int* a, int left, int right)
{
	int mid = GetMidIndex(a, left, right);
	swap(a[mid], a[right]);

	int key = a[right];
	while (left < right)
	{
		while (left < right && a[left] <= key)
		{
			left++;
		}
		a[right] = a[left];

		while (left < right && a[right] >= key)
		{
			right--;
		}
		
		a[left] = a[right];
	}

	a[left] = key;
	return left;
}

//3.前后指针法
int PartSort_3(int* a, int left, int right)
{
	int mid = GetMidIndex(a, left, right);
	swap(a[mid], a[right]);

	int key = right;
	int cur = left;
	int prev = cur - 1;
	while (cur < key)
	{
		if (a[cur] <= a[key])
		{
			prev++;
			swap(a[cur], a[prev]);
		}
		cur++;
	}
	swap(a[++prev], a[right]);//注意这里的prev一定要++
	return prev;
}

//递归方式
void QuickSort_R(int* a, int left, int right)
{
	if (left < right)
	{
		int ret = PartSort_3(a, left, right);

		//小区间优化的方式
		if (right - left < 20)
		{
			InsertSort(a + left, right - left + 1);
			return;
		}
		QuickSort_R(a, left, ret - 1);
		QuickSort_R(a, ret + 1, right);
	}
}

//非递归方式，利用栈去实现
void QuickSortNonR(int* a, int left, int right)
{
	stack<int> s;
	s.push(right);
	s.push(left);

	while (!s.empty())
	{
		int begin = s.top();
		s.pop();//注意要加上pop()函数
		int end = s.top();
		s.pop();

		int div = PartSort_3(a, begin, end);

		if (begin < div - 1)
		{
			s.push(div - 1);
			s.push(begin);
		}

		if (div + 1 < end)
		{
			s.push(end);
			s.push(div + 1);
		}
	}
}

void TestQuickSort()
{
	int a[] = { 2, 0, 4, 9, 3, 6, 8, 7, 1, 5 };
	QuickSort_R(a, 0, sizeof(a) / sizeof(a[0]) - 1);
	//QuickSortNonR(a, 0, sizeof(a) / sizeof(a[0]) - 1);
	Print(a, sizeof(a) / sizeof(a[0]));
}


// 归并排序
void _MerSort(int* a, int* tmp, int left, int mid, int right)
{
	int begin1 = left, end1 = mid;
	int begin2 = mid + 1, end2 = right;
	int index = begin1;
	while (begin1 <= end1 && begin2 <= end2)//肯定有一个区间会先跳出，而另外一个区间还没有全传过去
	{
		if (a[begin1] < a[begin2])
		{
			tmp[index++] = a[begin1++];
		}
		else
		{
			tmp[index++] = a[begin2++];
		}
	}

	//当其中一个区间跳出时，要将另外一个区间剩下的全部继续放进去
	while (begin1 <= end1)
	{
		tmp[index++] = a[begin1++];
	}

	while (begin2 <= end2)
	{
		tmp[index++] = a[begin2++];
	}

	for (size_t i = left; i <= right; ++i)
	{
		a[i] = tmp[i];
	}

}

void _MergeSort(int* a, int* tmp, int left, int right)
{
	if (left >= right)
		return;

	int mid = left + ((right - left) >> 1);

	_MergeSort(a, tmp, left, mid);
	_MergeSort(a, tmp, mid + 1, right);

	_MerSort(a, tmp, left, mid, right);

}

void MergeSort(int* a, int n)
{
	assert(a);

	int* tmp = new int[n];

	_MergeSort(a, tmp, 0, n - 1);

	delete[] tmp;
}

void TestMergeSort()
{
	int a[] = { 2, 0, 4, 9, 3, 6, 8, 7, 1, 5 };
	MergeSort(a, sizeof(a) / sizeof(a[0]));
	Print(a, sizeof(a) / sizeof(a[0]));
}


// 基数排序
size_t GetMaxDigit(int* a, size_t n)
{
	int base = 10;
	int digit = 1;

	for (size_t i = 0; i < n; ++i)
	{
		while (a[i] >= base)
		{
			++digit;
			base *= 10;
		}
	}

	return digit;
}

//先低位再高位
void LSDSort(int* a, size_t n)
{
	int digit = GetMaxDigit(a, n);
	int base = 1;

	for (size_t i = 0; i < digit; ++i)
	{
		int count[10] = { 0 };
		int start[10] = { 0 };

		for (size_t j = 0; j < n; ++j)//记录数据的个数
		{
			int num = (a[j] / base) % 10;
			count[num]++;
		}

		for (size_t j = 1; j < n; ++j)//记录数据的起始位置
		{
			start[j] = count[j - 1] + start[j - 1];
		}

		int* tmp = new int[n];
		for (size_t j = 0; j < n; ++j)//将数据放到另一个临时数组中去
		{
			int num = (a[j] / base) % 10;
			tmp[start[num]++] = a[j];
		}

		memcpy(a, tmp, sizeof(int)*n);
		base *= 10;

		delete[] tmp;
	}
}

void TestLSDSort()
{
	int a[] = { 2, 0, 4, 9, 3, 6, 8, 7, 1, 5 };
	int  n = sizeof(a) / sizeof(int);
	LSDSort(a, n);
	Print(a, n);
}

//先高位再低位
void MSDSort(int* a, size_t n)
{
	int digit = GetMaxDigit(a, n);
	int base = 10 * (digit - 1);
	if (base == 0)
	{
		base = 1;
	}

	for (size_t i = 0; i < digit; ++i)
	{
		int  count[10] = { 0 };
		int	 start[10] = { 0 };

		for (size_t j = 0; j < n; ++j)//记录数据的个数
		{
			int num = (a[j] / base) % 10;
			count[num]++;
		}

		for (size_t j = 1; j < n; ++j)//记录数据的起始位置
		{
			start[j] = count[j - 1] + start[j - 1];
		}

		int* tmp = new int[n];
		for (size_t j = 0; j < n; ++j)//将数据放到另一个临时数组中去
		{
			int num = (a[j] / base) % 10;
			tmp[start[num]++] = a[j];
		}
		memcpy(a, tmp, sizeof(int)*n);
		base /= 10;

		delete[] tmp;
	}
}

void TestMSDSort()
{
	int a[] = { 2, 0, 4, 9, 3, 6, 8, 7, 1, 5 };
	int  n = sizeof(a) / sizeof(int);
	MSDSort(a, n);
	Print(a, n);
}