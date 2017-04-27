struct __TrueType
{
	bool Get()
	{
		return true;
	}
};

struct __FalseType
{
	bool Get()
	{
		return false;
	}
};

template<class T>
struct TypeTraits
{
	typedef __FalseType  __IsPODType;
};

template<>
struct TypeTraits<bool>
{
	typedef __TrueType  __IsPODType;
};

template<>
struct TypeTraits<char>
{
	typedef __TrueType  __IsPODType;
};

template<>
struct TypeTraits<unsigned char>
{
	typedef __TrueType  __IsPODType;
};

template<>
struct TypeTraits<short>
{
	typedef __TrueType  __IsPODType;
};

template<>
struct TypeTraits<unsigned short>
{
	typedef __TrueType  __IsPODType;
};

template<>
struct TypeTraits<int>
{
	typedef __TrueType  __IsPODType;
};

template<>
struct TypeTraits<unsigned int>
{
	typedef __TrueType  __IsPODType;
};

template<>
struct TypeTraits<long>
{
	typedef __TrueType  __IsPODType;
};

template<>
struct TypeTraits<unsigned long>
{
	typedef __TrueType  __IsPODType;
};

template<>
struct TypeTraits<long long>
{
	typedef __TrueType  __IsPODType;
};

template<>
struct TypeTraits<unsigned long long>
{
	typedef __TrueType  __IsPODType;
};

template<>
struct TypeTraits<double>
{
	typedef __TrueType  __IsPODType;
};

template<>
struct TypeTraits<float>
{
	typedef __TrueType  __IsPODType;
};

template<>
struct TypeTraits<long double>
{
	typedef __TrueType  __IsPODType;
};

template<class T>
struct TypeTraits<T*>
{
	typedef __TrueType  __IsPODType;
};