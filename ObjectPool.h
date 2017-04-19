#pragma once;

template<class T>
class ObjectPool
{
	struct Node
	{
		void* memory; //ָ�����ڴ��ָ��
		size_t n;	  //��ǰ�ڵ��������n������
		Node* next;   //ָ����һ���ڵ�

		Node(size_t nobjs)
		{
			n = nobjs;
			memory = operator new(GetObjSize()*n);
			next = NULL;
		}

		~Node()
		{
			operator delete(memory);
			memory = NULL;
		}
	};

	void AllocNewNode()
	{
		size_t n = _tail->n * 2;
		if (n > _maxNObjs)
			n = _maxNObjs;

		Node* node = new Node(n);
		_tail->next = node;
		_tail = node;

		_useInCount = 0;
	}

public:
	ObjectPool(size_t initNObjs = 16, size_t maxNObjs = 1024)
		: _initNObjs(initNObjs)
		, _maxNObjs(maxNObjs)
		, _lastDelete(NULL)
	{
		_head = _tail = new Node(initNObjs);
		_useInCount = 0;
	}

	~ObjectPool()
	{
		Node* cur = _head;
		while (cur)
		{
			Node* Next = cur->next;
			delete cur; 
			cur = Next;
		}
	}

	inline static size_t GetObjSize()
	{
		return sizeof(T) > sizeof(T*) ? sizeof(T) : sizeof(T*);
	}

	template <class Val>
	T* New(const Val& val)
	{
		void* obj = Allocate();
		return new(obj)T(val);
	}

	void Delete(T* ptr)
	{
		if (ptr)
		{
			ptr->~T();
			Deallocate(ptr);
		}
	}

	//O(1)
	void* Allocate()
	{
		//1������ʹ���ͷŻ����Ķ���
		if (_lastDelete)
		{
			void* obj = _lastDelete;
			_lastDelete = *((T**)_lastDelete);//������T**���ڽ�������Ϊ�˵õ�T*����ָ��Ĵ�С��ȡ��ǰ���4/8���ֽڣ�
			return obj;							//ָ����һ����ַ��Ҫ��ɾ���Ŀռ�
			
		}

		//2����Node����ȡ����
		if (_useInCount >= _tail->n)
			AllocNewNode();

		void* obj = (char*)_tail->memory + _useInCount*GetObjSize();
		_useInCount++;
		return obj;
	}

	void Deallocate(void* ptr)
	{
		*(T**)ptr = _lastDelete;
		_lastDelete = (T*)ptr;
	}

	size_t _initNObjs;
	size_t _maxNObjs;
	Node* _head;
	Node* _tail;
	size_t _useInCount;

	T* _lastDelete;
};

void TestObjPool()
{
	{
		ObjectPool<int> pool;

		void* p1 = pool.Allocate();
		void* p2 = pool.Allocate();
		pool.Deallocate(p2);
		pool.Deallocate(p1);
		void* p3 = pool.Allocate();
		void* p4 = pool.Allocate();
	}

	ObjectPool<string> pool1;
	string* p10 = pool1.New("����");
	pool1.Delete(p10);
}
