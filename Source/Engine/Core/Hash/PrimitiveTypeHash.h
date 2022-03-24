#pragma once

#include "CoreGlobals.h"

// Identity hashes for all primitive types.
inline uint64 GetTypeHash(const int8 InElement)
{
	return (uint64)InElement;
}

inline uint64 GetTypeHash(const uint8 InElement)
{
	return (uint64)InElement;
}

inline uint64 GetTypeHash(const int16 InElement)
{
	return (uint64)InElement;
}

inline uint64 GetTypeHash(const uint16 InElement)
{
	return (uint64)InElement;
}

inline uint64 GetTypeHash(const int32 InElement)
{
	return (uint64)InElement;
}

inline uint64 GetTypeHash(const uint32 InElement)
{
	return (uint64)InElement;
}

inline uint64 GetTypeHash(const int64 InElement)
{
	return (uint64)InElement;
}

inline uint64 GetTypeHash(const uint64 InElement)
{
	return (uint64)InElement;
}
