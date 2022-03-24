#pragma once

/**
 * Same as Unreal's TIdentity.
 * This is used to inhibit argument type deduction and force users to specify the necessary type.
 * 
 * Examples:
 * template <typename T>
 * void Func(T Arg); // Can be called Func(123) or Func<int>(123);
 * 
 * template <typename T>
 * void Func(TIdentity<T> Arg); // Can only be called via Func<int>(123);
 */
template<typename T>
struct TIdentity
{
	using Type = T;
};
