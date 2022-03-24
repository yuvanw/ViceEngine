#pragma once

// Traits for L value reference.
template <typename T>
struct TIsLValueReference
{
	static constexpr bool Value = false;
};

template <typename T>
struct TIsLValueReference<T&>
{
	static constexpr bool Value = true;
};
