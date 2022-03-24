#pragma once

#include "CoreGlobals.h"

// Tests if a type is arithmetic.
template <typename T>
struct TIsArithmeticType
{
	static constexpr bool Value = false;
};

template <>
struct TIsArithmeticType<float>
{ 
	static constexpr bool Value = true; 
};

template <>
struct TIsArithmeticType<double>
{ 
	static constexpr bool Value = true; 
};

template <>
struct TIsArithmeticType<long double>
{ 
	static constexpr bool Value = true; 
};

template <>
struct TIsArithmeticType<uint8>
{ 
	static constexpr bool Value = true; 
};

template <>
struct TIsArithmeticType<uint16>
{ 
	static constexpr bool Value = true; 
};

template <>
struct TIsArithmeticType<uint32> 
{ 
	static constexpr bool Value = true; 
};

template <>
struct TIsArithmeticType<uint64>
{ 
	static constexpr bool Value = true; 
};

template <>
struct TIsArithmeticType<int8>
{ 
	static constexpr bool Value = true; 
};

template <>
struct TIsArithmeticType<int16>
{ 
	static constexpr bool Value = true; 
};

template <>
struct TIsArithmeticType<int32>
{ 
	static constexpr bool Value = true; 
};

template <>
struct TIsArithmeticType<int64>
{ 
	static constexpr bool Value = true; 
};

template <>
struct TIsArithmeticType<long>
{ 
	static constexpr bool Value = true; 
};

template <>
struct TIsArithmeticType<unsigned long>
{ 
	static constexpr bool Value = true; 
};

template <>
struct TIsArithmeticType<bool>
{ 
	static constexpr bool Value = true; 
};

template <typename T>
struct TIsArithmeticType<const T>
{ 
	static constexpr bool Value = TIsArithmeticType<T>::Value; 
};

template <typename T>
struct TIsArithmeticType<volatile T>
{ 
	static constexpr bool Value = TIsArithmeticType<T>::Value; 
};

template <typename T>
struct TIsArithmeticType<const volatile T>
{ 
	static constexpr bool Value = TIsArithmeticType<T>::Value; 
};
