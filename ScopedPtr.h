#pragma once

#include<iostream>
using namespace std;

//在之前，我们使用的AutoPtr和UniquePtr会出错的原因主要是深浅拷贝的问题，即拷贝和复制引发的析构对象时的指针释放问题
//所以，我们在这里实现ScopedPtr来防拷贝

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