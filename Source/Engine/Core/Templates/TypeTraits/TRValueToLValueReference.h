#pragma once

template <typename T> 
struct TRValueToLValueReference 
{ 
	using Type = T; 
};

template <typename T> 
struct TRValueToLValueReference<T&&> 
{
	using Type = T&;
};
