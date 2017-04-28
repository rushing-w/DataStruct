#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include<iostream>
using namespace std; 
#include<string.h>

//ǳ����
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

//���--��ͨ��

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

	String& operator+(const String& s)//�൱��strcat
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
	//��������ÿ⺯��
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
			if (*str1 == 0)//���str1�Ѿ��ߵ�'\0'�����ʾ�����ַ�������ȵ�
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


//���--����
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
		: _str(NULL)//������������ó�ʼ���б��ʼ������ô�ͻ�����һ�����ֵ����ô�ڽ���������ͷžͻ������
	{
		String tmp(s._str);
		std::swap(_str, tmp._str);	//����tmp�����ʱ�����ᱻ���٣���ϵͳ�����ͷ�һ�������ָ�룬���Ծ���Ҫ��ʼ��
	}

	String& operator=(const String& s)//���ﲻ��Ҫ��ʼ������Ϊһ��ʼ��_str������Ч�ռ䣬����ֱ���ͷ�
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

//ǳ����--���ü���
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
			if (_str != s._str)		//����ַ������ֵĵ�ַ��ȣ��ǾͲ��ø��ˣ�˵���������ȵ�,
			{						//�������ʵҲ���Բ�Ҫ�ģ�����������ĵ�ַ��Ⱦʹ�����������ȵ�
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

//ǳ����--���ü������Ż���ģ��new[]-->��ǰ�ĸ��ֽ�������ʹ��
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

//дʱ����--Copy On Write
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
		
		if (*_count > 1)//�����ô�������1ʱ�����¿ռ䣬��������������Ϊ1
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

