#pragma once

#include "IsArithmeticType.h"
#include "IsPointerType.h"

template<typename T>
struct TIsDefaultConstructable
{
	static constexpr bool Value = !TIsArithmeticType<T>::Value && !TIsPointerType<T>::Value;
};
