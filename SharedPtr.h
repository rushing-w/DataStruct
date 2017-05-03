#pragma once

#include<iostream>
using namespace std;

//由于上面的指针存在各种各样的问题，只能适用于特定的场合，所以我们需要实现一种比较通用的，
//在这里，实现了SharedPtr，即引用计数的深拷贝

template<class T>
class SharedPtr
{
public:
	SharedPtr(T* ptr = NULL)
		: _ptr(ptr)
	{
		_count = new int(1);
	}

	SharedPtr(SharedPtr<T>& sp)
		: _ptr(sp._ptr)
	{
		(*sp._count)++;
		_count = sp._count;
	}

	SharedPtr<T>& operator=(SharedPtr<T>& sp)
	{
		
		if (_ptr != sp._ptr)
		{
			_ptr = sp._ptr;

			if (*sp._count == 1)
			{
				(*sp._count)++;
				_count = sp._count;
			}
			else
			{
				(*sp._count)++;
				(*_count)--;
				_count = sp._count;
			}
		}
		
		return *this;
	}

	~SharedPtr()
	{
		if (_ptr && --(*_count) == 0)
		{
			delete _ptr;
			delete _count;
		}
		_ptr = NULL;
		_count = NULL;
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

private:
	T* _ptr;
	int* _count;
};

void TestSharedPtr()
{
	SharedPtr<int> sp1(new int(3));
	SharedPtr<int> sp2(sp1);
	SharedPtr<int> sp3;
	SharedPtr<int> sp4(sp3);
	sp4 = sp1;
}

//虽然这种引用计数版本的简化版智能指针SharedPtr解决了释放空间时的问题，但是还存在不少问题
//1、存在线程安全问题

//2、循环引用问题-->当我们使用双向链表时，p1的next指向p2，p2的prev指向p1，这时p1和p2的_count值都为2，因为
//都有两个指针指向了一块空间，所以删除时，需要将其中一个的_count--，但是另外一个还在使用这一块空间，所以总是存在
//两个指针指向这块空间，所以直接删除是删除不了的

//---------->>>所以，在boost库中引入了weak_ptr来打破循环引用（weak_ptr不增加引用计数），weak_ptr是一种
//不控制的所指向对象生存期的智能指针，它指向由一个shared_ptr管理的对象。将一个weak_ptr绑定到一个shared_ptr
//上，不会改变shared_ptr的引用计数。即存在循环引用时将weak_ptr的_count++，不去处理shared_ptr的_count。
//一旦最后一个指向对象的shared_ptr被销毁的话，对象就会被释放，即使weak_ptr有指向的对象，对象也还是会被释放 。


//3、定置删除器--->需要包含boost库，所以这里就贴一下代码(vs里面似乎没有这个库)
//#include <boost/shared_ptr.hpp>
//using namespace boost;
//// 定置的删除器仿函数
//class FClose
//{
//public :
//	void operator () (void* ptr)
//	{
//		cout << "fclose" << endl;
//		fclose((FILE *)ptr);
//	}
//};
//
//class Free
//{
//public :
//	void operator () (void* ptr)
//	{
//		cout << "free" << endl;
//		free(ptr);
//	}
//};
//void Test()
//{
//	// 定制删除器
//	shared_ptr<FILE> p1(fopen("test.txt", "w"), FClose());
//	// 定制删除器和分配器
//	shared_ptr<int> p2((int *)malloc(sizeof(int)), Free(), allocator<int>());
//}


