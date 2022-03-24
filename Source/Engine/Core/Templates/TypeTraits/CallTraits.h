#pragma once

#include "AndOrNot.h"
#include "IsPODType.h"
#include "IsArithmeticType.h"

template <typename T, bool bIsSmallType>
struct TCallTraitsParamTypeHelper
{
	using ParamType = const T&;
	using ConstParamType = const T&;
};

// This specialization is applicable for types T that should be passed by value and aren't pointers.
template <typename T>
struct TCallTraitsParamTypeHelper<T, true>
{
	using ParamType = const T;
	using ConstParamType = const T;
};

// This specialization is applicable for types T that should be passed by value and are pointers.
template <typename T>
struct TCallTraitsParamTypeHelper<T*, true>
{
	using ParamType = T*;
	using ConstParamType = const T*;
};

template <typename T>
struct TCallTraitsBase
{
private:
	// Pass T by value in either of the following conditions:
	// 1. T is less than or equal the size of a pointer AND it is a POD type.
	// 2. T is an arithmetic (i.e. T is a built-in, excluding pointers).
	static constexpr bool PassByValue = TOr<TAndValue<(sizeof(T) <= sizeof(void*)), TIsPODType<T>>, TIsArithmeticType<T>>::Value;

public:
	using ValueType = T;
	using ReferenceType = T&;
	using ConstReferenceType = const T&;
	using ParamType = typename TCallTraitsParamTypeHelper<T, PassByValue>::ParamType;
	using ConstParamType = typename TCallTraitsParamTypeHelper<T, PassByValue>::ConstParamType;
};

template <typename T>
struct TCallTraits : public TCallTraitsBase<T>
{
};
