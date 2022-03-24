#include "ArenaAllocator.h"
#include "MemoryManager.h"
#include "AlignmentUtilities.h"


FArenaAllocator::FArenaAllocator(void* InStart, int32 InCapacity)
	: Start((int8*)InStart)
	, Capacity(InCapacity)
	, NumBytesUsed(0)
{
}

FArenaAllocator::~FArenaAllocator()
{
	// Do nothing
}

void* FArenaAllocator::Allocate(int32 InSizeBytes, EMemoryAlignment InAlignment /* = EMemoryAlignment::Default */)
{
	if (!InSizeBytes)
	{
		return nullptr;
	}

	int32 AlignedOffset = (int8*)FAlignmentUtilities::Align(Start + NumBytesUsed, InAlignment) - Start;
	int32 NewOffset = AlignedOffset + InSizeBytes;
	if (NewOffset >= Capacity)
	{
		return nullptr;
	}

	NumBytesUsed = NewOffset;
	return Start + AlignedOffset;
}

void FArenaAllocator::Deallocate(void* InAddress)
{
	// Do nothing
}

void FArenaAllocator::Clear()
{
	NumBytesUsed = 0;
}

FArenaAllocator& FArenaAllocator::GetDefaultAllocator()
{
	return FMemoryManager::Get().GetArenaAllocator();
}
