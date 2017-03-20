#pragma once

#include<vector>
#include<iostream>
using namespace std;

class BitMap
{
public:
	BitMap(size_t N = 1024)
	{
		_array.resize((N >> 5) + 1);
	}

	void Set(size_t value)
	{
		size_t index = value >> 5;
		size_t num = value % 32;

		_array[index] |= (1 << num);
	}

	void Reset(size_t value)
	{
		size_t index = value >> 5;
		size_t num = value % 32;

		_array[index] = value &= ~(1 << num);
	}

	bool Test(size_t value)
	{
		size_t index = value >> 5;
		size_t num = value % 32;

		return (_array[index]) & (1 << num);
	}

protected:
	vector<size_t> _array;
}; 

void TestBitmap()
{
	BitMap bm;
	bm.Set(1);
	cout << "1:" << bm.Test(1) << endl;
	cout << "2:" << bm.Test(2) << endl;

}