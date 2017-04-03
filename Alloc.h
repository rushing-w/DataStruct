#pragma once

#include<iostream>
using namespace std;
#include<stdarg.h>
#include<string>
#include<vector>

#define __DEBUG__
static string GetFileName(const string& path)
{
	char ch = '/';
#ifdef _WIN32
	ch = '\\';
#endif
	size_t pos = path.rfind(ch);
	if (pos == string::npos)
		return path;
	else
		return path.substr(pos + 1);
}

//用于调试追溯的trace log
inline static void __trace_debug(const char* function,const char* filename, int line, char* format, ...)
{
#ifdef __DEBUG__

	//输出调用函数的信息
	fprintf(stdout, "【 %s:%d】 %s", GetFileName(filename).c_str(), line, function);
	
	//输出用户打的trace信息
	va_list args;
	va_start(args, format);
	vfprintf(stdout, format, args);
	va_end(args);
#endif
}
#define __TRACE_DEBUG(...) \
__trace_debug(__FUNCTION__, __FILE__, __LINE__, __VA_ARGS__);

#define __THROW_BAD_ALLOC cerr << " out of memory " << endl; exit(1)

//内存分配失败以后处理的句柄handler类型
typedef void(*__mallocAllocOomHandler)();

//一级空间配置器
template <int inst>
class __MallocAllocTemplate
{
private:

	//Oom: out of memory  下面的函数用来处理内存不足的情况
	static void* OomMalloc(size_t n);

	static void* OomRealloc(void* p, size_t n);

	static void(*__mallocAllocOomHandler)();


public:
	static void* Allocate(size_t n)
	{
		__TRACE_DEBUG("(n:%u)\n", n);

		void* result = malloc(n);
		if (0 == result)
		{
			result = OomMalloc(n);
		}
		return result;
	}

	static void Deallocate(void* p)
	{
		__TRACE_DEBUG("(p:%p)\n", p);

		free(p);
	}

	static void Reallocate(void* p, size_t newSize)
	{
		void* result = realloc(p, newSize);

		if (0 == newSize)
		{
			result = OomRealloc(p, newSize);
		}

		return result;
	}

	//设置句柄,模拟C++中set_new_handler()来处理内存不足的情况
	static void(*__SetMallocHandler(void* f()))()
	{
		void(*old)() = __mallocAllocOomHandler;
		__mallocAllocOomHandler = f;
		return (old);
	}

};

template<int inst>
void(*__MallocAllocTemplate<inst>::__mallocAllocOomHandler)() = 0;

template<int inst>
void* __MallocAllocTemplate<inst>::OomMalloc(size_t n)
{
	void(*myMallocHandler)();
	void* result;

	// 1： 分配内存成功， 则直接返回
	// 2： 若分配失败， 则检查是否设置处理的handler，
	//有则调用以后再分配。 不断重复这个过程， 直到分配成功为止。
	//没有设置处理的handler， 则直接结束程序。

	while (1)//通过循环不断尝试释放、配置
	{
		myMallocHandler = __mallocAllocOomHandler;
		if (0 == myMallocHandler)//如果没能分配就抛异常
		{
			__THROW_BAD_ALLOC;
		}

		(*myMallocHandler)();//在这里调用处理程序，企图释放内存
		result = malloc(n);
		if (result)
			return result;
	}
}

template<int inst>
void* __MallocAllocTemplate<inst>::OomRealloc(void* p, size_t n)
{
	void(*myMallocHandler)();
	void* result;

	while (1)//通过循环不断尝试释放、配置
	{
		myMallocHandler = __mallocAllocOomHandler;
		if (0 == myMallocHandler)//如果没能分配就抛异常
		{
			__THROW_BAD_ALLOC;
		}

		(*myMallocHandler)();//在这里调用处理程序，企图释放内存
		result = realloc(p, n);
		if (result)
			return result;
	}
}


typedef __MallocAllocTemplate<0> MallocAlloc;

//二级空间配置器
template <bool threads, int inst>
class __DefaultAllocTemplate
{
private:

	enum { __ALIGN = 8 };// 自由链表的排列间隔
	enum { __MAX_BYTES = 128 };// 最大值
	enum { __NFREELISTS = __MAX_BYTES / __ALIGN }; // 排列链的大小
	
	union Obj//构造_freeList的节点
	{
		union Obj* _freeListLink;//自由链表中指向下一个内存块的指针
		char client_data[1];    /* The client sees this.*/
	};

	static size_t ROUND_UP(size_t bytes)// 调整bytes至8的倍数
	{
		return (((bytes) + __ALIGN - 1) & ~(__ALIGN - 1));
	}

	static size_t FREELIST_INDEX(size_t bytes)//获取自由链表的下标
	{
		return ((bytes) + (__ALIGN - 1) / (__ALIGN - 1));
	}

	//重新填充空间，获取大块内存插入到自由链表中
	static void* ReFill(size_t n);

	//狭义内存池，从中获取大块内存
	static char* ChunkAlloc(size_t size, int& nobjs);

private:

	static Obj* volatile _freeList[__NFREELISTS]; //自由链表
	static char* _startFree; //内存池起始位置
	static char* _endfree; //内存池结束位置
	static size_t _heapSize; //已经申请的内存
	
public:
	static void* Allocate(size_t n);

	static void Deallocate(void* p, size_t n);

	static void* Reallocate(void* p, size_t oldSize, size_t newSize);
};



//初始化全局静态对象
template <bool threads, int inst>
typename __DefaultAllocTemplate<threads, inst>::Obj* volatile \
__DefaultAllocTemplate<threads, inst>::_freeList[__DefaultAllocTemplate<threads, inst>::__NFREELISTS] = {0};

template <bool threads, int inst>
char* __DefaultAllocTemplate<threads, inst>::_startFree = 0;

template <bool threads, int inst>
char* __DefaultAllocTemplate<threads, inst>::_endfree = 0;

template <bool threads, int inst>
size_t __DefaultAllocTemplate<threads, inst>::_heapSize = 0;

template <bool threads, int inst>
void* __DefaultAllocTemplate<threads, inst>::ReFill(size_t n)
{
	__TRACE_DEBUG("(n:%u)\n", n);

	int nobjs = 20;
	char* chunk = ChunkAlloc(n, nobjs);//在狭义内存池中分配n个字节的内存
	
	//如果只能分配一个，那就直接返回这块内存
	if (nobjs == 1)
		return chunk;

	//能分配多个就开始调整freeList，纳入新节点
	size_t index = FREELIST_INDEX(n);
	Obj* ret = (Obj*)chunk; //第一块返回给客户端
	Obj* cur = (Obj*)(chunk + n);// 找到自由链表中应当插入节点的位置
	_freeList[index] = cur;
	for (int i = 2; i < nobjs; ++i)//循环走了nobjs-2次，并将各节点串起来
	{
		cur->_freeListLink = (Obj*)(chunk + n*i);//头插这个节点
		cur = cur->_freeListLink;
	}

	cur->_freeListLink = NULL;//最后将最后一个节点的下个置为空
	return ret;
}

template <bool threads, int inst>
char* __DefaultAllocTemplate<threads, inst>::ChunkAlloc(size_t size, int& nobjs)
{
	__TRACE_DEBUG("(size: %u, nobjs: %d)\n", size, nobjs);

	char* ret;
	size_t bytesNeed = size * nobjs;
	size_t bytesLeft = _endfree - _startFree;//狭义内存池剩余的空间

	if (bytesLeft >= bytesNeed)// 1.内存池中的内存足够， bytesLeft>=bytesNeed， 则直接从内存池中取。
	{
		__TRACE_DEBUG("内存池中内存足够分配%d个对象\n", nobjs);

		ret = _startFree;
		_startFree += bytesNeed;
	}
	else if (bytesLeft >= size)	// 2.内存池中的内存不足， 但是够一个，bytesLeft >= size， 则直接取能够取出来。
	{
		__TRACE_DEBUG("内存池中内存不够分配%d个对象， 只能分配%d个对象\n", nobjs, bytesLeft / size);

		ret = _startFree;
		nobjs = bytesLeft / size;
		_startFree += nobjs * size;
	}
	else// 3.内存池中的内存不足， 则从系统堆分配大块内存到内存池中。
	{
		if (bytesLeft > 0)//若内存池中还有小块剩余内存， 则将它头插到合适的自由链表
		{
			size_t index = FREELIST_INDEX(size);
			((Obj*)_startFree)->_freeListLink = _freeList[index];
			_freeList[index] = (Obj*)_startFree;
			_startFree = NULL;

			__TRACE_DEBUG("将内存池中剩余的空间， 分配给freeList[%d]\n", index);
		}

		//从系统堆分配两倍 + 已分配的heapSize / 16的内存到内存池中
		size_t bytesToGet = 2 * bytesNeed + ROUND_UP(_heapSize >> 4);
		_startFree = (char*)malloc(bytesToGet);

		__TRACE_DEBUG("内存池空间不足， 系统堆分配%u bytes内存\n", bytesToGet);

		//【 无奈之举】
		//如果在系统堆中内存分配失败， 则尝试到自由链表中更大的节点中分配
		if (_startFree == NULL)
		{
			__TRACE_DEBUG("系统堆已无足够， 无奈之下， 智能到自由链表中看看\n");

			size_t index = FREELIST_INDEX(size);
			for (; index < __MAX_BYTES; ++index)
			{
				if (_freeList[index])
				{
					_startFree = (char*)_freeList[index];
					_freeList[index] = ((Obj*)_startFree)->_freeListLink;
					_endfree = _startFree + (index + 1)*__ALIGN;//(index + 1)*__ALIGN的结果是这个部分的自由链表的节点值大小
					return ChunkAlloc(size, nobjs);//递归调用自己，修正nobjs，获取内存
				}
			}
			
			_endfree = NULL;
			//【 山穷水尽】
			//自由链表中也没有分配到内存， 则再到一级配置器中分配内存，一级配置器中可能有设置的处理内存， 或许能分配到内存。
			__TRACE_DEBUG("系统堆和自由链表都已无内存， 一级配置器做最后一根稻草\n");
			_startFree = (char*)MallocAlloc::Allocate(bytesToGet);//如果确实没有就抛异常
		}
		//将再次申请的内存加上，得到从系统堆分配的总字节数
		_heapSize += bytesToGet;
		_endfree = _startFree + bytesToGet;

		return ChunkAlloc(size, nobjs);//递归调用自己，修正nobjs，获取内存
	}
	return ret;
}

template <bool threads, int inst>
void* __DefaultAllocTemplate<threads, inst>::Allocate(size_t n)
{
	__TRACE_DEBUG("(n: %u)\n", n);

	if (n > __MAX_BYTES)//大于128就调用一级空间配置器
	{
		return MallocAlloc::Allocate(n);
	}
	//寻找自由链表中合适的那一个
	size_t index = FREELIST_INDEX(n);
	Obj* ret = _freeList[index];
	if (ret == NULL)//如果自由链表没有就用ReFill()来填充
	{
		return ReFill(ROUND_UP(n));
	}
	//找到了就直接返回一个节点块内存
	__TRACE_DEBUG("自由链表取内存:_freeList[%d]\n", index);
	_freeList[index] = ret->_freeListLink;
	return ret;

}

template <bool threads, int inst>
void __DefaultAllocTemplate<threads, inst>::Deallocate(void* p, size_t n)
{
	__TRACE_DEBUG("(p:%p, n: %u)\n", p, n);

	if (n > __MAX_BYTES)//如果大于128就直接归还给一级空间配置器
	{
		MallocAlloc::Deallocate(p, n);
		return;
	}
	//否则就放在自由链表中
	size_t index = FREELIST_INDEX(n);
	Obj* tmp = (Obj*)p;
	tmp->_freeListLink = _freeList[index];
	_freeList[index] = tmp;

}

template <bool threads, int inst>
void* __DefaultAllocTemplate<threads, inst>::Reallocate(void* p, size_t oldSize, size_t newSize)
{
	if (oldSize > __MAX_BYTES && newSize > __MAX_BYTES)//如果都大于128就直接调用一级空间配置器
	{
		return MallocAlloc::Reallocate(p, newSize);
	}
	if (ROUND_UP(oldSize) == ROUND_UP(newSize))//如果两个都在链表的同一个位置，那就直接返回这个节点
	{
		return p;
	}
	//如果都不行，那就先开辟newSize个空间，然后根据谁大谁小将原数据拷进去并释放原空间
	void* ret = Allocate(newSize);
	size_t copySize;
	if (newSize > oldSize)
		copySize = oldSize;
	else
		copySize = newSize;

	memcpy(ret, p, copySize);
	Deallocate(p, oldSize);
	return ret;
}

#ifdef __USE_MALLOC

typedef MallocAlloc  Alloc;

#else

typedef __DefaultAllocTemplate<false, 0> Alloc;

#endif


template<class T, class Alloc>
class SimpleAlloc 
{
public:
	static T* Allocate(size_t n)
	{
		return 0 == n ? 0 : (T*)Alloc::Allocate(n * sizeof (T));
	}
	static T* Allocate(void)
	{
		return (T*)Alloc::Allocate(sizeof (T));
	}
	static void Deallocate(T* p, size_t n)
	{
		if (0 != n) Alloc::Deallocate(p, n * sizeof (T));
	}
	static void Deallocate(T* p)
	{
		Alloc::Deallocate(p, sizeof (T));
	}
};

void TestAlloc1()
{
	SimpleAlloc<int, Alloc> sa;
	vector<int*> v;
	for (size_t i = 0; i < 20; ++i)
	{
		v.push_back(sa.Allocate(1));
	}
	v.push_back(sa.Allocate(1));

}

void TestAlloc2()
{
	cout << "测试内存池空间不足分配20个" << endl;
	// 8*20->8*2->320
	char* p1 = SimpleAlloc<char, Alloc>::Allocate(8);
	char* p2 = SimpleAlloc<char, Alloc>::Allocate(8);
	cout << "测试内存池空间不足， 系统堆进行分配" << endl;
	char*p3 = SimpleAlloc<char, Alloc>::Allocate(12);
}

//测试系统堆内存耗尽的场景
void TestAlloc3()
{
	cout << "测试系统堆内存耗尽" << endl;
	SimpleAlloc<char, Alloc>::Allocate(1024 * 1024 * 1024);
	//SimpleAlloc<char, Alloc>::Allocate(1024*1024*1024);SimpleAlloc<char,Alloc>::Allocate(1024*1024);
	//不好测试， 说明系统管理小块内存的能力还是很强的。
	for (int i = 0; i< 100000; ++i)
	{
		char* p1 = SimpleAlloc<char, Alloc>::Allocate(128);
	}
}