#pragma once

#include<iostream>
using namespace std;

//��֮ǰ������ʹ�õ�AutoPtr��UniquePtr������ԭ����Ҫ����ǳ���������⣬�������͸�����������������ʱ��ָ���ͷ�����
//���ԣ�����������ʵ��ScopedPtr��������

template<class T>
class ScopedPtr
{
public:
	ScopedPtr(T* ptr = NULL)
		: _ptr(ptr)
	{}

	~ScopedPtr()
	{
		if (_ptr)
			delete _ptr;
		_ptr = NULL;
	}

protected:

	ScopedPtr(ScopedPtr<T>& sp);
	ScopedPtr<T>& operator=(ScopedPtr<T>& sp);

protected:
	T* _ptr;
};

void TestScopedPtr()
{
	ScopedPtr<int> sp1;
	//ScopedPtr<int> sp2(sp1);
	ScopedPtr<int> sp2;
	//sp2 = sp1;
}