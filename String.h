#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include<iostream>
using namespace std; 
#include<string.h>

//浅拷贝
/*
class String
{
public:
	String(const char* str = " ")
	{
		if (str == NULL)
		{
			_str = new char[1];
			*_str = '\0';
		}
		else
		{
			_str = new char[strlen(str) + 1];
			strcpy(_str, str);
		}
	}

	String(const String& s)
		: _str(s._str)
	{}

	String& operator=(const String& s)
	{
		if (this != &s)
		{
			_str = s._str;
		}
		return *this;
	}

	~String()
	{
		if (_str)
			delete[] _str;
		_str = NULL;
	}

private:
	char* _str;
};
*/

//深拷贝--普通版

class String
{
public:
	String(const char* str = " ")
	{
		if (str == NULL)
		{
			_str = new char[1];
			*_str = '\0';
		}
		else
		{
			_str = new char[strlen(str) + 1];
			strcpy(_str, str);
		}
	}

	String(const String& s)
		: _str(new char[strlen(s._str) + 1])
	{
		strcpy(_str, s._str);
	}

	String& operator=(const String& s)
	{
		if (this != &s)
		{
			char* tmp = new char[strlen(s._str) + 1];
			if (tmp)
			{
				strcpy(tmp, s._str);
				delete[] _str;
				_str = tmp;
			}
		}
		return *this;
	}

	~String()
	{
		if (_str)
			delete[] _str;
		_str = NULL;
	}

	void Copy(const String& s)
	{
		String tmp(s);
		std::swap(_str, tmp._str);
	}

	String StrStr(const String& s)
	{
		if (!s._str)
			return *this;

		char* dest = _str;
		char* src = s._str;
		char* cp = dest;
		while (cp)
		{
			dest = cp;
			while (*dest != '\0' && *src != '\0' && *dest == *src)
			{
				dest++;
				src++;
			}
			if (*src == '\0')
			{
				return String(cp);
			}
			src = s._str;
			cp++;
		}
		return String(NULL);
	}

	size_t Size()
	{
		char* str = _str;
		size_t count = 0;
		while (*str)
		{
			count++;
			str++;
		}
		return count;
	}

	bool Empty()
	{
		return (*_str == 0);
	}

	String& operator+(const String& s)//相当于strcat
	{
		String tmp(s);
		char* str1 = tmp._str;
		char* str2 = _str;
		char* cur = new char[tmp.Size() + Size() + 1];
		char* str3 = cur;
		while (*str2)
		{
			*str3 = *str2;
			str3++;
			str2++;
		}
		while (*str1)
		{
			*str3 = *str1;
			str3++;
			str1++;
		}
		*str3 = 0;
		delete[] _str;
		_str = cur;
		return *this;
	}

	bool operator<(const String& s)
	{
		return (Strcmp(s._str) < 0);
	}

	bool operator>(const String& s)
	{
		return (Strcmp(s._str) > 0);
	}

	bool operator==(const String& s)
	{
		return (Strcmp(s._str) == 0);
	}

	bool operator!=(const String& s)
	{
		return (Strcmp(s._str) != 0);
	}

protected:
	//不允许调用库函数
	void Strcpy(const char* str)
	{
		String s(str);
		std::swap(_str, s._str);
	}

	int Strcmp(const char* str)
	{
		char* str1 = _str;
		char* str2 = (char*)str;
		while (*str1 == *str2)
		{
			if (*str1 == 0)//如果str1已经走到'\0'这里表示两个字符串是相等的
				return 0;
			str1++;
			str2++;
		}
		if (*str1 > *str2)
			return 1;
		else
			return -1;
	}

private:
	char *_str;
};


//深拷贝--简洁版
/*
class String
{
public:
	String(const char* str = " ")
	{
		if (str == NULL)
		{
			_str = new char[1];
			*_str = 0;
		}
		else
		{
			_str = new char[strlen(str) + 1];
			strcpy(_str, str);
		}
	}

	String(const String& s)
		: _str(NULL)//在这里如果不用初始化列表初始化，那么就会生成一个随机值，那么在交换后进行释放就会出问题
	{
		String tmp(s._str);
		std::swap(_str, tmp._str);	//由于tmp这个临时变量会被销毁，而系统不能释放一个随机的指针，所以就需要初始化
	}

	String& operator=(const String& s)//这里不需要初始化是因为一开始的_str就是有效空间，可以直接释放
	{
		if (this != &s)
		{
			String tmp(s._str);
			std::swap(_str, tmp._str);
		}
		return *this;
	}
	
	~String()
	{
		if (_str)
			delete[] _str;
		_str = NULL;
	}

private:
	char* _str;
};
*/

//浅拷贝--引用计数
/*
class String
{
public:
	String(const char* str = " ")
	{
		if (str == NULL)
		{
			_str = new char[1];
			*_str = 0;
		}
		else
		{
			_str = new char[strlen(str) + 1];
			strcpy(_str, str);
		}
		_count = new int(1);
	}

	String(String& s)
		:_str(s._str)
	{
		(*s._count)++;
		_count = s._count;
	}

	String& operator=(String& s)
	{
		if (this != &s)
		{
			if (_str != s._str)		//如果字符串部分的地址相等，那就不用赋了，说明这就是相等的,
			{						//但这个其实也可以不要的，基本上这里的地址相等就代表他们是相等的
				if (!(--(*_count)))
				{
					delete[] _str;
					delete[] _count;
					_str = NULL;
					_count = NULL;
				}
				_str = s._str;
				(*s._count)++;
				_count = s._count;
			}
		}
		return *this;
	}

	~String()
	{
		if (!(--(*_count)))
		{
			delete[] _str;
			delete[] _count;
		}
		_str = NULL;
		_count = NULL;
	}

private:
	char* _str;
	int* _count;
};
*/

//浅拷贝--引用计数的优化，模仿new[]-->将前四个字节留出来使用
/*
class String
{
public:
	String(const char* str = " ")
	{
		if (str == NULL)
		{
			_str = new char[1 + 4];
			_str += 4;
			*_str = 0;
		}
		else
		{
			_str = new char[strlen(str) + 1 + 4];
			_str += 4;
			strcpy(_str, str);
		}
		*(int*)(_str - 4) = 1;
	}

	String(String& s)
		: _str(s._str)
	{
		(*(int*)(_str - 4))++;
	}

	String& operator=(String& s)
	{
		if (this != &s)
		{
			if (!(--(*(int*)(_str - 4))))
			{
				delete[] (_str - 4);
				_str = NULL;
			}
			_str = s._str;
			(*(int*)(_str - 4))++;
			
		}
		return *this;
	}

	~String()
	{
		if (!(--(*(_str - 4))))
		{
			delete[] (_str - 4);
		}
		_str = NULL;
	}

private:
	char* _str;
};
*/

//写时拷贝--Copy On Write
/*
class String
{
public:
	String(const char* str = "")
	{
		if (str == NULL)
		{
			_str = new char[1];
			*_str = 0;
		}
		else
		{
			_str = new char[strlen(str) + 1];
			strcpy(_str, str);
		}
		_count = new int(1);
	}

	String(String& s)
		: _str(s._str)
	{
		++(*s._count);
		_count = s._count;
	}

	String& operator=(String& s)
	{
		if (this != &s)
		{
			if (_str != s._str)
			{
				if (!(--(*_count)))
				{
					delete[] _str;
					delete[] _count;
					_str = NULL;
					_count = NULL;
				}
				_str = s._str;
				++(*s._count);
				_count = s._count;
			}
		}
		return *this;
	}

	~String()
	{
		if (!(--(*_count)))
		{
			delete[] _str;
			delete[] _count;
		}
		_str = NULL;
		_count = NULL;
	}

	char& operator[](const size_t index)
	{
		
		if (*_count > 1)//当引用次数大于1时开辟新空间，并将计数重新置为1
		{
			--(*_count);
			char* tmp = new char[strlen(_str) + 1];
			strcpy(tmp, _str);
			_count = new int(1);
		}
		return *(_str + index);
	}

private:
	char* _str;
	int* _count;
};
*/

