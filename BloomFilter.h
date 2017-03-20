#pragma once

#include"BItMap.h"
#include<string>

struct _HashFunc1
{
	size_t BKDRHash(const char *s)
	{
		register size_t hash = 0;
		while (size_t ch = (size_t)*s++)
		{
			hash = hash * 131 + ch;            
		}
		return hash;
	}
	size_t operator()(const string& s)
	{
		return BKDRHash(s.c_str());
	}
};

struct _HashFunc2
{
	size_t SDBMHash(const char *s)
	{
		register size_t hash = 0;
		while (size_t ch = (size_t)*s++)
		{
			hash = 65599 * hash + ch;
		}
		return hash;
	}
	size_t operator()(const string& s)
	{
		return SDBMHash(s.c_str());
	}
};

struct _HashFunc3
{
	size_t RSHash(const char *s)
	{
		register size_t hash = 0;
		size_t magic = 63689;
		while (size_t ch = (size_t)*s++)
		{
			hash = hash * magic + ch;
			magic *= 378551;
		}
		return hash;
	}
	size_t operator()(const string& s)
	{
		return RSHash(s.c_str());
	}
};

struct _HashFunc4
{
	size_t APHash(const char *s)
	{
		register size_t hash = 0;
		size_t ch;
		for (long i = 0; ch = (size_t)*s++; i++)
		{
			if ((i & 1) == 0)
			{
				hash ^= ((hash << 7) ^ ch ^ (hash >> 3));
			}
			else
			{
				hash ^= (~((hash << 11) ^ ch ^ (hash >> 5)));
			}
		}
		return hash;
	}
	size_t operator()(const string& s)
	{
		return APHash(s.c_str());
	}
};

struct _HashFunc5
{
	size_t JSHash(const char *s)
	{
		if (!*s)
			return 0;
		register size_t hash = 1315423911;
		while (size_t ch = (size_t)*s++)
		{
			hash ^= ((hash << 5) + ch + (hash >> 2));
		}
		return hash;
	}
	size_t operator()(const string& s)
	{
		return JSHash(s.c_str());
	}
};

template<class K = string,
class HashFunc1 = _HashFunc1,
class HashFunc2 = _HashFunc2,
class HashFunc3 = _HashFunc3,
class HashFunc4 = _HashFunc4,
class HashFunc5 = _HashFunc5>
class BloomFilter
{
public:
	BloomFilter(size_t N)
		: _bm(N * 10)
		, _size(N * 10)
	{}

	void Set(const K& num)
	{
		size_t hash1 = HashFunc1()(num);
		size_t hash2 = HashFunc2()(num);
		size_t hash3 = HashFunc3()(num);
		size_t hash4 = HashFunc4()(num);
		size_t hash5 = HashFunc5()(num);

		_bm.Set(hash1);
		_bm.Set(hash2);
		_bm.Set(hash3);
		_bm.Set(hash4);
		_bm.Set(hash5);

		cout << hash1 << endl;
		cout << hash2 << endl;
		cout << hash3 << endl;
		cout << hash4 << endl;
		cout << hash5 << endl;
		cout << endl;
	}

	bool Test(const K& num)
	{
		size_t hash1 = HashFunc1()(num);
		if (_bm.Test(hash1) == false)
			return false;

		size_t hash2 = HashFunc2()(num);
		if (_bm.Test(hash2) == false)
			return false;

		size_t hash3 = HashFunc3()(num);
		if (_bm.Test(hash3) == false)
			return false;

		size_t hash4 = HashFunc4()(num);
		if (_bm.Test(hash4) == false)
			return false;

		size_t hash5 = HashFunc5()(num);
		if (_bm.Test(hash5) == false)
			return false;

		return true;
	}

protected:
	BitMap _bm;
	size_t _size;
};

void TestBloomFilter()
{
	BloomFilter<> bf((size_t)-1);
	bf.Set("peter1");
	bf.Set("peter2");
	bf.Set("peter3");

	cout << "peter1:" << bf.Test("peter1") << endl;
	cout << "peter2:" << bf.Test("peter2") << endl;
	cout << "peter3:" << bf.Test("peter3") << endl;
	cout << "peter4:" << bf.Test("peter4") << endl;
}