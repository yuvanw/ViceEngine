#pragma once

template <typename T>
struct TIsPODType
{
	static constexpr bool Value = __is_pod(T);
};
