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

//���ڵ���׷�ݵ�trace log
inline static void __trace_debug(const char* function,const char* filename, int line, char* format, ...)
{
#ifdef __DEBUG__

	//������ú�������Ϣ
	fprintf(stdout, "�� %s:%d�� %s", GetFileName(filename).c_str(), line, function);
	
	//����û����trace��Ϣ
	va_list args;
	va_start(args, format);
	vfprintf(stdout, format, args);
	va_end(args);
#endif
}
#define __TRACE_DEBUG(...) \
__trace_debug(__FUNCTION__, __FILE__, __LINE__, __VA_ARGS__);

#define __THROW_BAD_ALLOC cerr << " out of memory " << endl; exit(1)

//�ڴ����ʧ���Ժ���ľ��handler����
typedef void(*__mallocAllocOomHandler)();

//һ���ռ�������
template <int inst>
class __MallocAllocTemplate
{
private:

	//Oom: out of memory  ����ĺ������������ڴ治������
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

	//���þ��,ģ��C++��set_new_handler()�������ڴ治������
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

	// 1�� �����ڴ�ɹ��� ��ֱ�ӷ���
	// 2�� ������ʧ�ܣ� �����Ƿ����ô����handler��
	//��������Ժ��ٷ��䡣 �����ظ�������̣� ֱ������ɹ�Ϊֹ��
	//û�����ô����handler�� ��ֱ�ӽ�������

	while (1)//ͨ��ѭ�����ϳ����ͷš�����
	{
		myMallocHandler = __mallocAllocOomHandler;
		if (0 == myMallocHandler)//���û�ܷ�������쳣
		{
			__THROW_BAD_ALLOC;
		}

		(*myMallocHandler)();//��������ô��������ͼ�ͷ��ڴ�
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

	while (1)//ͨ��ѭ�����ϳ����ͷš�����
	{
		myMallocHandler = __mallocAllocOomHandler;
		if (0 == myMallocHandler)//���û�ܷ�������쳣
		{
			__THROW_BAD_ALLOC;
		}

		(*myMallocHandler)();//��������ô��������ͼ�ͷ��ڴ�
		result = realloc(p, n);
		if (result)
			return result;
	}
}


typedef __MallocAllocTemplate<0> MallocAlloc;

//�����ռ�������
template <bool threads, int inst>
class __DefaultAllocTemplate
{
private:

	enum { __ALIGN = 8 };// ������������м��
	enum { __MAX_BYTES = 128 };// ���ֵ
	enum { __NFREELISTS = __MAX_BYTES / __ALIGN }; // �������Ĵ�С
	
	union Obj//����_freeList�Ľڵ�
	{
		union Obj* _freeListLink;//����������ָ����һ���ڴ���ָ��
		char client_data[1];    /* The client sees this.*/
	};

	static size_t ROUND_UP(size_t bytes)// ����bytes��8�ı���
	{
		return (((bytes) + __ALIGN - 1) & ~(__ALIGN - 1));
	}

	static size_t FREELIST_INDEX(size_t bytes)//��ȡ����������±�
	{
		return ((bytes) + (__ALIGN - 1) / (__ALIGN - 1));
	}

	//�������ռ䣬��ȡ����ڴ���뵽����������
	static void* ReFill(size_t n);

	//�����ڴ�أ����л�ȡ����ڴ�
	static char* ChunkAlloc(size_t size, int& nobjs);

private:

	static Obj* volatile _freeList[__NFREELISTS]; //��������
	static char* _startFree; //�ڴ����ʼλ��
	static char* _endfree; //�ڴ�ؽ���λ��
	static size_t _heapSize; //�Ѿ�������ڴ�
	
public:
	static void* Allocate(size_t n);

	static void Deallocate(void* p, size_t n);

	static void* Reallocate(void* p, size_t oldSize, size_t newSize);
};



//��ʼ��ȫ�־�̬����
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
	char* chunk = ChunkAlloc(n, nobjs);//�������ڴ���з���n���ֽڵ��ڴ�
	
	//���ֻ�ܷ���һ�����Ǿ�ֱ�ӷ�������ڴ�
	if (nobjs == 1)
		return chunk;

	//�ܷ������Ϳ�ʼ����freeList�������½ڵ�
	size_t index = FREELIST_INDEX(n);
	Obj* ret = (Obj*)chunk; //��һ�鷵�ظ��ͻ���
	Obj* cur = (Obj*)(chunk + n);// �ҵ�����������Ӧ������ڵ��λ��
	_freeList[index] = cur;
	for (int i = 2; i < nobjs; ++i)//ѭ������nobjs-2�Σ��������ڵ㴮����
	{
		cur->_freeListLink = (Obj*)(chunk + n*i);//ͷ������ڵ�
		cur = cur->_freeListLink;
	}

	cur->_freeListLink = NULL;//������һ���ڵ���¸���Ϊ��
	return ret;
}

template <bool threads, int inst>
char* __DefaultAllocTemplate<threads, inst>::ChunkAlloc(size_t size, int& nobjs)
{
	__TRACE_DEBUG("(size: %u, nobjs: %d)\n", size, nobjs);

	char* ret;
	size_t bytesNeed = size * nobjs;
	size_t bytesLeft = _endfree - _startFree;//�����ڴ��ʣ��Ŀռ�

	if (bytesLeft >= bytesNeed)// 1.�ڴ���е��ڴ��㹻�� bytesLeft>=bytesNeed�� ��ֱ�Ӵ��ڴ����ȡ��
	{
		__TRACE_DEBUG("�ڴ�����ڴ��㹻����%d������\n", nobjs);

		ret = _startFree;
		_startFree += bytesNeed;
	}
	else if (bytesLeft >= size)	// 2.�ڴ���е��ڴ治�㣬 ���ǹ�һ����bytesLeft >= size�� ��ֱ��ȡ�ܹ�ȡ������
	{
		__TRACE_DEBUG("�ڴ�����ڴ治������%d������ ֻ�ܷ���%d������\n", nobjs, bytesLeft / size);

		ret = _startFree;
		nobjs = bytesLeft / size;
		_startFree += nobjs * size;
	}
	else// 3.�ڴ���е��ڴ治�㣬 ���ϵͳ�ѷ������ڴ浽�ڴ���С�
	{
		if (bytesLeft > 0)//���ڴ���л���С��ʣ���ڴ棬 ����ͷ�嵽���ʵ���������
		{
			size_t index = FREELIST_INDEX(size);
			((Obj*)_startFree)->_freeListLink = _freeList[index];
			_freeList[index] = (Obj*)_startFree;
			_startFree = NULL;

			__TRACE_DEBUG("���ڴ����ʣ��Ŀռ䣬 �����freeList[%d]\n", index);
		}

		//��ϵͳ�ѷ������� + �ѷ����heapSize / 16���ڴ浽�ڴ����
		size_t bytesToGet = 2 * bytesNeed + ROUND_UP(_heapSize >> 4);
		_startFree = (char*)malloc(bytesToGet);

		__TRACE_DEBUG("�ڴ�ؿռ䲻�㣬 ϵͳ�ѷ���%u bytes�ڴ�\n", bytesToGet);

		//�� ����֮�١�
		//�����ϵͳ�����ڴ����ʧ�ܣ� ���Ե����������и���Ľڵ��з���
		if (_startFree == NULL)
		{
			__TRACE_DEBUG("ϵͳ�������㹻�� ����֮�£� ���ܵ����������п���\n");

			size_t index = FREELIST_INDEX(size);
			for (; index < __MAX_BYTES; ++index)
			{
				if (_freeList[index])
				{
					_startFree = (char*)_freeList[index];
					_freeList[index] = ((Obj*)_startFree)->_freeListLink;
					_endfree = _startFree + (index + 1)*__ALIGN;//(index + 1)*__ALIGN�Ľ����������ֵ���������Ľڵ�ֵ��С
					return ChunkAlloc(size, nobjs);//�ݹ�����Լ�������nobjs����ȡ�ڴ�
				}
			}
			
			_endfree = NULL;
			//�� ɽ��ˮ����
			//����������Ҳû�з��䵽�ڴ棬 ���ٵ�һ���������з����ڴ棬һ���������п��������õĴ����ڴ棬 �����ܷ��䵽�ڴ档
			__TRACE_DEBUG("ϵͳ�Ѻ��������������ڴ棬 һ�������������һ������\n");
			_startFree = (char*)MallocAlloc::Allocate(bytesToGet);//���ȷʵû�о����쳣
		}
		//���ٴ�������ڴ���ϣ��õ���ϵͳ�ѷ�������ֽ���
		_heapSize += bytesToGet;
		_endfree = _startFree + bytesToGet;

		return ChunkAlloc(size, nobjs);//�ݹ�����Լ�������nobjs����ȡ�ڴ�
	}
	return ret;
}

template <bool threads, int inst>
void* __DefaultAllocTemplate<threads, inst>::Allocate(size_t n)
{
	__TRACE_DEBUG("(n: %u)\n", n);

	if (n > __MAX_BYTES)//����128�͵���һ���ռ�������
	{
		return MallocAlloc::Allocate(n);
	}
	//Ѱ�����������к��ʵ���һ��
	size_t index = FREELIST_INDEX(n);
	Obj* ret = _freeList[index];
	if (ret == NULL)//�����������û�о���ReFill()�����
	{
		return ReFill(ROUND_UP(n));
	}
	//�ҵ��˾�ֱ�ӷ���һ���ڵ���ڴ�
	__TRACE_DEBUG("��������ȡ�ڴ�:_freeList[%d]\n", index);
	_freeList[index] = ret->_freeListLink;
	return ret;

}

template <bool threads, int inst>
void __DefaultAllocTemplate<threads, inst>::Deallocate(void* p, size_t n)
{
	__TRACE_DEBUG("(p:%p, n: %u)\n", p, n);

	if (n > __MAX_BYTES)//�������128��ֱ�ӹ黹��һ���ռ�������
	{
		MallocAlloc::Deallocate(p, n);
		return;
	}
	//����ͷ�������������
	size_t index = FREELIST_INDEX(n);
	Obj* tmp = (Obj*)p;
	tmp->_freeListLink = _freeList[index];
	_freeList[index] = tmp;

}

template <bool threads, int inst>
void* __DefaultAllocTemplate<threads, inst>::Reallocate(void* p, size_t oldSize, size_t newSize)
{
	if (oldSize > __MAX_BYTES && newSize > __MAX_BYTES)//���������128��ֱ�ӵ���һ���ռ�������
	{
		return MallocAlloc::Reallocate(p, newSize);
	}
	if (ROUND_UP(oldSize) == ROUND_UP(newSize))//����������������ͬһ��λ�ã��Ǿ�ֱ�ӷ�������ڵ�
	{
		return p;
	}
	//��������У��Ǿ��ȿ���newSize���ռ䣬Ȼ�����˭��˭С��ԭ���ݿ���ȥ���ͷ�ԭ�ռ�
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
	cout << "�����ڴ�ؿռ䲻�����20��" << endl;
	// 8*20->8*2->320
	char* p1 = SimpleAlloc<char, Alloc>::Allocate(8);
	char* p2 = SimpleAlloc<char, Alloc>::Allocate(8);
	cout << "�����ڴ�ؿռ䲻�㣬 ϵͳ�ѽ��з���" << endl;
	char*p3 = SimpleAlloc<char, Alloc>::Allocate(12);
}

//����ϵͳ���ڴ�ľ��ĳ���
void TestAlloc3()
{
	cout << "����ϵͳ���ڴ�ľ�" << endl;
	SimpleAlloc<char, Alloc>::Allocate(1024 * 1024 * 1024);
	//SimpleAlloc<char, Alloc>::Allocate(1024*1024*1024);SimpleAlloc<char,Alloc>::Allocate(1024*1024);
	//���ò��ԣ� ˵��ϵͳ����С���ڴ���������Ǻ�ǿ�ġ�
	for (int i = 0; i< 100000; ++i)
	{
		char* p1 = SimpleAlloc<char, Alloc>::Allocate(128);
	}
}