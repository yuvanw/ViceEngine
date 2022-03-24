#pragma once

#include "CoreGlobals.h"
#include "IAllocator.h"
#include "Alignment.h"

/**
 * Linearly allocates memory arenas for use by engine subsystems.
 */
class FArenaAllocator : public IAllocator
{
public:
	/**
	 * Constructor. Creates an arena allocator with a specified starting address and capacity.
	 * Clients must provide the allocator memory themselves.
	 * 
	 * @param InStart: Starting address of the allocator's memory.
	 * @param InCapacity: Total size in bytes of the allocator's memory.
	 */
	explicit FArenaAllocator(void* InStart, int32 InCapacity);

	// Destructor.
	~FArenaAllocator();

	// Non-copyable.
	FArenaAllocator(const FArenaAllocator&) = delete;
	FArenaAllocator& operator=(const FArenaAllocator&) = delete;

	// Begin IAllocator interface.
	virtual void* Allocate(int32 InSizeBytes, EMemoryAlignment InAlignment = EMemoryAlignment::Default) override;
	virtual void  Deallocate(void* InAddress) override;
	// End IAllocator interface.

	/**
	 * Deallocates all memory currently in use.
	 */
	void Clear();

	static FArenaAllocator& GetDefaultAllocator();

private:
	// Starting address of the allocator's memory.
	int8* Start;
	// Total size in bytes of the allocator's memory.
	int32 Capacity;
	// Number of bytes of allocator memory already used.
    int32 NumBytesUsed;
};
