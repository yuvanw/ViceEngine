#pragma once

// Type traits for const types.
template <typename T>
struct TIsConstType
{
	static constexpr bool Value = false;
};

template <typename T>
struct TIsConstType<const T>
{
	static constexpr bool Value = false;
};
