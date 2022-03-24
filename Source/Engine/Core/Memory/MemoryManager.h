#pragma once

#include "CoreGlobals.h"
#include "Memory/ArenaAllocator.h"

/**
 * Singleton class that owns and provides access to engine memory.
 */
class FMemoryManager
{
public:
	// Non-copyable.
	FMemoryManager(const FMemoryManager&) = delete;
	FMemoryManager& operator=(const FMemoryManager&) = delete;

	static FMemoryManager& Get()
	{
		static FMemoryManager MemoryManager;
		return MemoryManager;
	}

	FArenaAllocator& GetArenaAllocator() 
	{ 
		return *ArenaAllocator; 
	}

private:
	FMemoryManager();
	~FMemoryManager();

	// Starting address of engine memory.
	int8* MemoryStart;
	// Total size in bytes of engine memory.
	uint64 TotalSize;
	// Allocator used to provide memory arenas for engine subsystems.
	FArenaAllocator* ArenaAllocator;
};
