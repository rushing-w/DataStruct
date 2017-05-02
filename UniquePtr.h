#pragma once

#include<iostream>
using namespace std;

//��AutoPtr�Ļ����ϣ����������UniquePtr���ָ�룬��AutoPtr_owner��������һ��������ʾָ��ĺϷ���
//���������ͷ��ڴ�ʱ�Ͳ�����ֱ����ˣ����������ǰ���󲻺Ϸ�����ô�ͻ���Ч�����ÿ��������븴�ƾͻᱨ��

template<class T>
class UniquePtr
{
public:
	explicit UniquePtr(T* ptr = NULL)
		: _ptr(ptr)
	{
		_owner = true;
	}

	UniquePtr(UniquePtr<T>& up)
		: _ptr(up._ptr)
	{
		up._owner = false;
		_owner = true;
	}

	UniquePtr<T>& operator=(UniquePtr<T>& up)
	{
		if (this != &up)
		{
			_ptr = up._ptr;
			up._owner = false;
			_owner = true;
		}
		return *this;
	}

	~UniquePtr()
	{
		if (_ptr && _owner == true)
		{
			delete _ptr;
			_ptr = NULL;
		}
		else
		{
			_ptr = NULL;
		}
	}

	T operator*()
	{
		if (_ptr)
			return *_ptr;
		return 0;
	}

	T* operator->()
	{
		return _ptr;
	}

protected:
	T* _ptr;
	bool _owner;
};

void TestUniquePtr()
{
	UniquePtr<int> up1(new int(3));
	UniquePtr<int> up2(up1);
	UniquePtr<int> up3;
	up3 = up2;
}