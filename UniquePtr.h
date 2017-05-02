#pragma once

#include<iostream>
using namespace std;

//在AutoPtr的基础上，产生了这个UniquePtr这个指针，即AutoPtr_owner，带上了一个变量表示指针的合法性
//这样，在释放内存时就不会出现报错了，但是如果当前对象不合法，那么就会无效，调用拷贝构造与复制就会报错

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