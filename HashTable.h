#pragma once

#include<iostream>
using namespace std;

#include<assert.h>
#include<vector>
#include<string>

enum Status
{
	EXIST,
	DELETE,
	EMPTY,
};

template<class K, class V>
struct HashNode
{
	K _key;
	V _value;
	Status _status;

	HashNode(const K& key = K(), const V& value = V())
		: _key(key)
		, _value(value)
		, _status(EMPTY)
	{}
};

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


template<class K, class V, class Hashfunc = __Hashfunc<K>>
class HashTable
{
	typedef HashNode<K, V> Node;
public:
	HashTable(size_t size)
		: _size(0)
	{
		assert(size > 0);
		_tables.resize(size);
	}

	~HashTable()
	{}
	
	pair<Node*, bool> Insert(const K& key, const V& value = V())
	{
		_CheckCapacity();
		size_t index = _Hashfunc(key);
		while (_tables[index]._status == EXIST)
		{
			if (_tables[index]._key == key)
			{
				return make_pair((Node*)NULL, false);
			}
			++index;
			if (index == _tables.size())
			{
				index = 0;
			}
		}

		_tables[index]._key = key;
		_tables[index]._value = value;
		_tables[index]._status = EXIST;

		++_size;

		return make_pair(&_tables[index], true);
	}

	Node* Find(const K& key)
	{
		if (_tables.empty())
		{
			return NULL;
		}

		size_t index = _Hashfunc(key);
		while (_tables[index]._status != EMPTY)
		{
			if (_tables[index]._key == key)
			{
				return &_tables[index];
			}
			++index;
			if (_tables.size() == index)
			{
				index = 0;
			}
		}
		return NULL;
	}

	bool Remove(const K& key)
	{
		Node* del = Find(key);
		if (del)
		{
			del->_status = DELETE;
			--_size;
			return true;
		}
		return false;
	}
	 
protected:
	void _CheckCapacity()
	{
		if (_tables.size() == 0)
		{
			_tables.resize(7);
			return;
		}
		
		if (_size * 10 / _tables.size() >= 7)
		{
			//size_t newsize = _tables.size() * 2;
			size_t newsize = _GetNextPrimer(_tables.size());
			HashTable<K, V, Hashfunc> tmp(newsize);
			for (size_t i = 0; i < _tables.size(); i++)
			{
				if (_tables[i]._status == EXIST)
				{
					tmp.Insert(_tables[i]._key, _tables[i]._value);
				}
			}
			_Swap(tmp);
		}
	}

	void _Swap(HashTable<K, V, Hashfunc>& tmp)
	{
		_tables.swap(tmp._tables);
		swap(_size, tmp._size);
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
	vector<Node> _tables;
	size_t _size;
};


void TestHashTable()
{
	HashTable<int, int> ht(10);
	HashTable<string, string, __Hashfunc<string>> ht1(1);
	ht.Insert(1);
	ht.Insert(9);
	ht.Insert(4);
	ht.Insert(3);
	ht.Insert(6);
	ht1.Insert("left", "×ó");
	ht1.Insert("right", "ÓÒ");

}