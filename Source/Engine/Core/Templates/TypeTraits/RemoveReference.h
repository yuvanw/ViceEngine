#pragma once

// Follows Unreal's implementation.
// Removes all types of references, both lvalue and rvalue references.
template <typename T>
struct TRemoveReference
{
	using Type = T;
};

template <typename T>
struct TRemoveReference<T&>
{
	using Type = T;
};

template <typename T>
struct TRemoveReference<T&&>
{
	using Type = T;
};
