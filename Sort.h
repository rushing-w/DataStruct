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
	
//ֱ�Ӳ�������
void InsertSort(int* a, size_t n)
{
	assert(a);
	for (size_t i = 1; i < n; ++i)
	{
		int end = i - 1;//��end��������
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


//ϣ������
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

//ѡ������
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


//������
void AdJustDown(int* a, size_t n, size_t k)
{
	assert(a);
	int parent = k;
	int child = 2 * k + 1;

	while (child < n)
	{
		if (child + 1 < n && a[child + 1] > a[child])//�ж����Һ����ĸ���Ȼ��õ��ϴ��һ������
		{
			child++;
		}

		if (a[child] > a[parent])//(�����)������ӽڵ��ֵ�ȸ��ڵ����ô���������½��е���������ѭ�����ϵ���
		{							//�����С�ѵĻ��ͽ�����Ĵ��ڸĳ�С�ھͺ�
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

void HeapSort(int* a, size_t n)//�����������������Ҫ��ѣ������ֵ�ŵ��Ѷ���Ȼ���������һ��Ҷ�ӽڵ��ֵ����
{
	assert(a);
	//����
	for (int i = (n - 2) / 2; i >= 0; --i)
	{
		AdJustDown(a, n, i);
	}

	//����
	int end = n - 1;//��end������һ��Ҷ�ӽڵ�
	while (end > 0)
	{
		swap(a[0], a[end]);//ÿһ�ν����꣬���Ѷ������ֵ�����һ��Ҷ�ӽڵ��ֵ������
		AdJustDown(a, end, 0); // �ͽ�����ڵ��ֵȡ����Ȼ���ٽ�����������µ���
		--end;
	}
}

void TestHeapSort()
{
	int a[] = { 2, 5, 4, 9, 3, 6, 8, 7, 1, 0 };
	HeapSort(a, sizeof(a) / sizeof(a[0]));
	Print(a, sizeof(a) / sizeof(a[0]));
}


//ð������
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

//��Ȼ��ð�������Ǵ����Ż��ģ������ð��һ����Ѿ������ˣ���ô��Ϳ���ֹͣѭ���ˣ������ͼ����˿���
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

//��������

//���ŵ��Ż�
//1.����ȡ�У�����������O(N^2)�����ÿһ��ȡ����keyֵ����������С����ô�ͻᵼ��������
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

//2.С�����Ż������ǵ����ֿ��������Ѿ��Ƚ�С��ʱ���������ʹ�õݹ飬��ô������ϴ��ѹջ�Ŀ��������ʱ������
// ����ͨ��ʹ��ֱ�Ӳ�����������Сѹջ�Ŀ�����ʹ��ֱ�Ӳ��������ԭ���ǻ��ּ���֮�������ǽӽ�����ֱ�Ӳ�������Ч�ʸ���

//���ŵ����ַ�ʽ
//1.����ָ�뽻����
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
	swap(a[left], a[key]);//��keyλ�õ�ֵ����
	return left;
}

//2.�ڿӷ�
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

//3.ǰ��ָ�뷨
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
	swap(a[++prev], a[right]);//ע�������prevһ��Ҫ++
	return prev;
}

//�ݹ鷽ʽ
void QuickSort_R(int* a, int left, int right)
{
	if (left < right)
	{
		int ret = PartSort_3(a, left, right);

		//С�����Ż��ķ�ʽ
		if (right - left < 20)
		{
			InsertSort(a + left, right - left + 1);
			return;
		}
		QuickSort_R(a, left, ret - 1);
		QuickSort_R(a, ret + 1, right);
	}
}

//�ǵݹ鷽ʽ������ջȥʵ��
void QuickSortNonR(int* a, int left, int right)
{
	stack<int> s;
	s.push(right);
	s.push(left);

	while (!s.empty())
	{
		int begin = s.top();
		s.pop();//ע��Ҫ����pop()����
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


// �鲢����
void _MerSort(int* a, int* tmp, int left, int mid, int right)
{
	int begin1 = left, end1 = mid;
	int begin2 = mid + 1, end2 = right;
	int index = begin1;
	while (begin1 <= end1 && begin2 <= end2)//�϶���һ���������������������һ�����仹û��ȫ����ȥ
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

	//������һ����������ʱ��Ҫ������һ������ʣ�µ�ȫ�������Ž�ȥ
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


// ��������
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

//�ȵ�λ�ٸ�λ
void LSDSort(int* a, size_t n)
{
	int digit = GetMaxDigit(a, n);
	int base = 1;

	for (size_t i = 0; i < digit; ++i)
	{
		int count[10] = { 0 };
		int start[10] = { 0 };

		for (size_t j = 0; j < n; ++j)//��¼���ݵĸ���
		{
			int num = (a[j] / base) % 10;
			count[num]++;
		}

		for (size_t j = 1; j < n; ++j)//��¼���ݵ���ʼλ��
		{
			start[j] = count[j - 1] + start[j - 1];
		}

		int* tmp = new int[n];
		for (size_t j = 0; j < n; ++j)//�����ݷŵ���һ����ʱ������ȥ
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

//�ȸ�λ�ٵ�λ
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

		for (size_t j = 0; j < n; ++j)//��¼���ݵĸ���
		{
			int num = (a[j] / base) % 10;
			count[num]++;
		}

		for (size_t j = 1; j < n; ++j)//��¼���ݵ���ʼλ��
		{
			start[j] = count[j - 1] + start[j - 1];
		}

		int* tmp = new int[n];
		for (size_t j = 0; j < n; ++j)//�����ݷŵ���һ����ʱ������ȥ
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