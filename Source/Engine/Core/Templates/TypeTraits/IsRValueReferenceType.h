#pragma once

// Traits for R value reference.
template<typename T>
struct TIsRValueReference
{
	static constexpr bool Value = false;
};

template <typename T>
struct TIsRValueReference<T&&>
{
	static constexpr bool Value = true;
};
