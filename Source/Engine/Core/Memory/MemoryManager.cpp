#include "MemoryManager.h"
#include "AssertionMacros.h"

// System includes for malloc and placement new.
#include <cstdlib>
#include <new>

FMemoryManager::FMemoryManager()
	// 50 MB
	: TotalSize(50 * 1024 * 1024)
{
	MemoryStart = (int8*)malloc(TotalSize);
	ensure(MemoryStart);
	ArenaAllocator = new (MemoryStart) FArenaAllocator(MemoryStart + sizeof(FArenaAllocator), TotalSize - sizeof(FArenaAllocator));
}

FMemoryManager::~FMemoryManager()
{
	// We need to manually call the destructor because we used placement new to construct the arena allocator.
	ArenaAllocator->~FArenaAllocator();
	free(MemoryStart);
}
