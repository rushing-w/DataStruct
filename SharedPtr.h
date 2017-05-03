#pragma once

#include<iostream>
using namespace std;

//���������ָ����ڸ��ָ��������⣬ֻ���������ض��ĳ��ϣ�����������Ҫʵ��һ�ֱȽ�ͨ�õģ�
//�����ʵ����SharedPtr�������ü��������

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

//��Ȼ�������ü����汾�ļ򻯰�����ָ��SharedPtr������ͷſռ�ʱ�����⣬���ǻ����ڲ�������
//1�������̰߳�ȫ����

//2��ѭ����������-->������ʹ��˫������ʱ��p1��nextָ��p2��p2��prevָ��p1����ʱp1��p2��_countֵ��Ϊ2����Ϊ
//��������ָ��ָ����һ��ռ䣬����ɾ��ʱ����Ҫ������һ����_count--����������һ������ʹ����һ��ռ䣬�������Ǵ���
//����ָ��ָ�����ռ䣬����ֱ��ɾ����ɾ�����˵�

//---------->>>���ԣ���boost����������weak_ptr������ѭ�����ã�weak_ptr���������ü�������weak_ptr��һ��
//�����Ƶ���ָ����������ڵ�����ָ�룬��ָ����һ��shared_ptr����Ķ��󡣽�һ��weak_ptr�󶨵�һ��shared_ptr
//�ϣ�����ı�shared_ptr�����ü�����������ѭ������ʱ��weak_ptr��_count++����ȥ����shared_ptr��_count��
//һ�����һ��ָ������shared_ptr�����ٵĻ�������ͻᱻ�ͷţ���ʹweak_ptr��ָ��Ķ��󣬶���Ҳ���ǻᱻ�ͷ� ��


//3������ɾ����--->��Ҫ����boost�⣬�����������һ�´���(vs�����ƺ�û�������)
//#include <boost/shared_ptr.hpp>
//using namespace boost;
//// ���õ�ɾ�����º���
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
//	// ����ɾ����
//	shared_ptr<FILE> p1(fopen("test.txt", "w"), FClose());
//	// ����ɾ�����ͷ�����
//	shared_ptr<int> p2((int *)malloc(sizeof(int)), Free(), allocator<int>());
//}


