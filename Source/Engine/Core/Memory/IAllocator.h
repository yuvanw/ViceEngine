#pragma once
#include "CoreGlobals.h"
#include "Memory/Alignment.h"

/**
 * Allocator interface that can be derived to customize allocator behavior.
 */
class IAllocator
{
public:
	virtual ~IAllocator() = default;

	virtual void* Allocate(int32 InSizeBytes, EMemoryAlignment InAlignment = EMemoryAlignment::Default) = 0;
	virtual void Deallocate(void* InAddress) = 0;
};
