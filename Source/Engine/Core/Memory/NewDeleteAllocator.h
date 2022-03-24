#pragma once
#include "CoreGlobals.h"
#include "Alignment.h"
#include "IAllocator.h"

/**
 * Allocator that uses operator new and delete.
 */
class FNewDeleteAllocator : public IAllocator
{
public:
	// Non-copyable.
	FNewDeleteAllocator(const FNewDeleteAllocator&) = delete;
	FNewDeleteAllocator& operator=(const FNewDeleteAllocator&) = delete;

	// Begin IAllocator interface.
	void* Allocate(int32 InSize, EMemoryAlignment InAlignment = EMemoryAlignment::Default) override 
	{ 
		return ::operator new(InSize); 
	}
	void  Deallocate(void* InAddress) override 
	{ 
		::operator delete(InAddress); 
	}
	// End IAllocator interface.

	static FNewDeleteAllocator& GetDefaultAllocator()
	{
		static FNewDeleteAllocator NewDeleteAllocator;
		return NewDeleteAllocator;
	}

private:
	FNewDeleteAllocator() = default;
	~FNewDeleteAllocator() = default;
};
