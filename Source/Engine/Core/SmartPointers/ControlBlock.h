#pragma once

#include "CoreGlobals.h"

/**
 * Maintains strong and weak reference counts associate with a resource.
 */
class FControlBlock
{
public:
	static FControlBlock* Create()
	{
		return new FControlBlock;
	}

	FControlBlock() = default;
	~FControlBlock() = default;

	FControlBlock(const FControlBlock&) = default;
	FControlBlock& operator=(const FControlBlock&) = default;

	FControlBlock(FControlBlock&&) = default;
	FControlBlock& operator=(FControlBlock&&) = default;

	void IncrementStrongRef()
	{
		++StrongRefCount;
	}

	void DecrementStrongRef()
	{
		--StrongRefCount;
	}

	void IncrementWeakRef()
	{
		++WeakRefCount;
	}

	void DecrementWeakRef()
	{
		--WeakRefCount;
	}

	int32 GetStrongRefCount() const
	{
		return StrongRefCount;
	}

	int32 GetWeakRefCount() const
	{
		return WeakRefCount;
	}

private:
	int32 StrongRefCount = 0;
	int32 WeakRefCount = 0;
};
