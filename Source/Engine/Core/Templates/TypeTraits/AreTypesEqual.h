#pragma once

// Tests if 2 types are the same type
template <typename A, typename B>
struct TAreTypesEqual
{
	static constexpr bool Value = false;
};

template <typename A>
struct TAreTypesEqual<A,A>
{
	static constexpr bool Value = true;
};
