#pragma once

// Tests if a type is a pointer.
template <typename T>
struct TIsPointerType
{
	static constexpr bool Value = false;
};

template <typename T>
struct TIsPointerType<T*>
{
	static constexpr bool Value = true;
};

template <typename T>
struct TIsPointerType<const T>
{
	static constexpr bool Value = TIsPointerType<T>::Value;
};

template <typename T>
struct TIsPointerType<volatile T>
{
	static constexpr bool Value = TIsPointerType<T>::Value;
};

template <typename T>
struct TIsPointerType<const volatile T>
{
	static constexpr bool Value = TIsPointerType<T>::Value;
};
