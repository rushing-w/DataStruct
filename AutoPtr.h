#pragma once

#include<iostream>
using namespace std;

//AutoPtr是主要用于浅拷贝，不涉及到深拷贝，用到拷贝构造或赋值时就需要去处理原空间

template<class T>
class AutoPtr
{
public:
	AutoPtr(T* ptr = NULL)
		: _ptr(ptr)
	{}

	AutoPtr(AutoPtr<T>& ap)
		: _ptr(ap._ptr)
	{
		ap._ptr = NULL;
	}

	AutoPtr<T>& operator=(AutoPtr<T>& ap)
	{
		if (this != &ap)
		{
			_ptr = ap._ptr;
			ap._ptr = NULL;
		}
		return *this;
	}

	~AutoPtr()
	{
		if (_ptr)
			delete _ptr;
		_ptr = NULL;
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
};

struct B
{
	int a;
	double b;
	char c;
};

void TestAutoPtr()
{
	AutoPtr<int> ap1(new int(3));
	AutoPtr<int> ap2(ap1);
	AutoPtr<int> ap3;
	ap3 = ap2;

	AutoPtr<B> ap;
}



