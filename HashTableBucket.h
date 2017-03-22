#pragma once

#include<iostream>
using namespace std;

#include<vector>

namespace Bucket
{
	template<class K, class V>
	struct HashNode
	{
		pair<K, V> _kv;
		HashNode<K, V>* _next;

		HashNode(const pair<K, V> kv)
			: _kv(kv)
			, _next(NULL)
		{}
	};

	template<class K, class V, class Hashfunc = __Hashfunc<K>>
	class HashTableBucket;

	template<class K>
	struct __Hashfunc
	{
		size_t operator()(const K& key)
		{
			return key;
		}
	};

	template<>
	struct __Hashfunc<string>
	{
		size_t BKDHash(const char* str)
		{
			register size_t  hash = 0;
			while (*str)
			{
				hash = hash * 131 + *str;
				++str;
			}
			return hash;
		}

		bool operator()(const string& s)
		{
			return BKDHash(s.c_str());
		}
	};

	template<class K, class V, class Ref, class Ptr>
	struct __HashTableIterator
	{
		typedef HashNode<K, V> Node;
		Node* _node;
		HashTableBucket<K, V>* _ht;

		typedef __HashTableIterator<K, V, Ref, Ptr> Self;

		__HashTableIterator(Node* node, HashTableBucket<K, V>* ht)
			: _node(node)
			, _ht(ht)
		{}

		Ref operator*()
		{
			return _node->_kv;
		}

		Ptr operator->()
		{
			return &(operator*());
		}

		bool operator!=(const Self& s) const
		{
			return _node != s._node;
		}

		bool operator==(const Self& s) const
		{
			return _node == s._node;
		}

		Self& operator++()
		{
			_node = _Next(_node);
			return *this;
		}

	protected:
		Node* _Next(Node* node)
		{
			Node* next = node->_next;
			if (next)
			{
				return next;
			}
			else
			{
				size_t index = _ht->_Hashfunc(node->_kv.first) + 1;
				for (; index < _ht->_tables.size(); ++index)
				{
					next = _ht->_tables[index];
					if (next)
					{
						return next;
					}
				}
			}
			return NULL;
		}


	};

	template<class K, class V, class Hashfunc>
	class HashTableBucket
	{
		typedef HashNode<K, V> Node;
	public:

		typedef __HashTableIterator<K, V, pair<K, V>&, pair<K, V>*> Iterator;
		typedef __HashTableIterator<K, V, const pair<K, V>&, const pair<K, V>*> ConstIterator;

		friend struct Iterator;
		friend struct ConstIterator;

		HashTableBucket()
			: _size(0)
		{}

		HashTableBucket(size_t size)
			: _size(size)
		{
			_tables.resize(size);
		}

		~HashTableBucket()
		{}

		pair<Node*, bool> Insert(const K& key, const V& value)
		{
			_CheckCapacity();
			size_t index = _Hashfunc(key);
			Node* cur = _tables[index];

			if (_tables[index] == NULL)
			{
				_tables[index] = new Node(make_pair(key, value));
				_size++;
				return make_pair(_tables[index], true);
			}

			while (cur)
			{
				if (cur->_kv.first == key)
				{
					cur->_kv.second++;
					return make_pair(cur, false);
				}
				cur = cur->_next;
			}

			Node* newnode = new Node(make_pair(key, value));
			newnode->_next = _tables[index];
			_tables[index] = newnode;
			_size++;
			return make_pair(_tables[index], true);
		}

		Node* Find(const K& key)
		{
			size_t index = _Hashfunc(key);
			Node* cur = _tables[index];
			while (cur)
			{
				if (cur->_kv.first == key)
				{
					return cur;
				}

				cur = cur->_next;
			}

			return NULL;
		}

		bool Erase(const K& key)
		{
			size_t index = _Hashfunc(key);
			Node* &cur = _tables[index];
			Node* prev = NULL;

			if (cur == NULL)
			{
				return false;
			}

			while (cur)
			{
				if (cur->_next == NULL)
				{
					delete cur;
					cur = NULL;
					break;
				}
				else if (cur->_next->_kv.first == key)
				{
					prev->_next = cur->_next;
					delete cur;
					cur = NULL;
					break;
				}
				prev = cur;
				cur = cur->_next;
			}
			--_size;
			return true;
		}

		void Clear()
		{
			for (size_t i = 0; i < _tables.size(); i++)
			{
				Node* &cur = _tables[i];
				Node* prev = NULL;
				while (cur)
				{
					prev = cur;
					cur = cur->_next;
					prev = NULL;
				}
			}

			_size = 0;
		}

		Iterator Begin()
		{
			for (size_t i = 0; i < _tables.size(); ++i)
			{
				Node* cur = _tables[i];
				if (cur)
				{
					return Iterator(cur, this);
				}
			}
			return Iterator(NULL, this);
		}

		ConstIterator CBegin() const
		{
			or(size_t i = 0; i < _tables.size(); ++i)
			{
				Node* cur = _tables[i];
				if (cur)
				{
					return Iterator(cur, this);
				}
			}
			return Iterator(NULL, this);
		}

		Iterator End()
		{
			return Iterator(NULL, this);
		}

		ConstIterator CEnd() const
		{
			return Iterator(NULL, this);
		}

	protected:
		void _CheckCapacity()
		{

			if (_tables.size() == _size)
			{
				size_t newsize = _GetNextPrimer(_tables.size());
				HashTableBucket<K, V, Hashfunc> tmp(newsize);
				for (size_t i = 0; i < _tables.size(); i++)
				{
					Node* cur = _tables[i];
					while (cur)
					{
						tmp.Insert(cur->_kv.first, cur->_kv.second);
						cur = cur->_next;
					}
				}

				_Swap(tmp);
			}
		}

		void _Swap(HashTableBucket<K, V>& ht)
		{
			_tables.swap(ht._tables);
			std::swap(_size, ht._size);
		}

		size_t _GetNextPrimer(size_t n)
		{
			const int _PrimeSize = 28;
			static const unsigned long _PrimeList[_PrimeSize] =
			{
				53ul, 97ul, 193ul, 389ul, 769ul,
				1543ul, 3079ul, 6151ul, 12289ul, 24593ul,
				49157ul, 98317ul, 196613ul, 393241ul, 786433ul,
				1572869ul, 3145739ul, 6291469ul, 12582917ul, 25165843ul,
				50331653ul, 100663319ul, 201326611ul, 402653189ul, 805306457ul,
				1610612741ul, 3221225473ul, 4294967291ul
			};

			for (size_t i = 0; i < _PrimeSize; i++)
			{
				if (n < _PrimeList[i])
				{
					return _PrimeList[i];
				}
			}

			return _PrimeList[_PrimeSize - 1];
		}

		size_t _Hashfunc(const K& key)
		{
			Hashfunc hf;
			size_t ret = hf(key);
			return ret % _tables.size();
		}

	protected:
		vector<Node*> _tables;
		size_t _size;		
	};


	//¥Ê‘⁄Œ Ã‚
	template<class K, class V, class Hashfunc = __Hashfunc<K>>
	class UnorderedMap
	{
		typedef HashNode<K, V> Node;

	public:
		typedef typename HashTableBucket<K, V, Hashfunc>::Iterator Iterator;
		typedef typename HashTableBucket<K, V, Hashfunc>::ConstIterator ConstIterator;

		UnorderedMap()
		{}

		pair<Node*, bool> Insert(const K& key, const V& value)
		{
			return _ht.Insert(key, value);
		}

		Node* Find(const K& key)
		{
			return _ht.Find(key);
		}

		bool Erase(const K& key)
		{
			return _ht.Erase(key);
		}

		Iterator Begin()
		{
			return _ht.Begin();
		}

		ConstIterator CBegin() const
		{
			return _ht.CBegin();
		}

		Iterator End()
		{
			return _ht.End();
		}

		ConstIterator CEnd()const
		{
			return _ht.CEnd();
		}

	protected:
		HashTableBucket<K, V, Hashfunc> _ht;
	};
}


void TestHashTableBucket()
{
	Bucket::HashTableBucket<int, int> htb;

	htb.Insert(51, 3);
	htb.Insert(105, 3);
	htb.Insert(52, 3);
	htb.Insert(3, 3);
	htb.Insert(55, 3);
	htb.Insert(2, 3);
	htb.Insert(106, 3);
	htb.Insert(53, 3);
	htb.Insert(0, 3);

	Bucket::HashTableBucket<int, int>::Iterator it = htb.Begin();
	while (it != htb.End())
	{
		cout << it->first << ":" << it->second;
		cout << endl;
		++it;
	}

	Bucket::HashNode<int,int>* ret = htb.Find(55);
	if (ret)
	{
		cout << ret->_kv.first << endl;
	}

	htb.Erase(3);

	htb.Clear();
}

void TestUnorderedMap()
{
	Bucket::UnorderedMap<int, int> um;
	um.Insert(1, 1);
	um.Insert(2, 1);
	um.Insert(3, 1);
	um.Insert(4, 1);
	Bucket::UnorderedMap<int, int>::Iterator it = um.Begin();
	while (it != um.End())
	{
		cout << it->first << ":" << it->second;
		cout << endl;
		++it;
	}
}

