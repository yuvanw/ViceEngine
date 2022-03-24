#pragma once

#include "CoreGlobals.h"
#include "AssertionMacros.h"
#include "Alignment.h"

struct FAlignmentUtilities
{
	/**
	 * Aligns a address to a specified alignment.
	 * 
	 * @param InAddress: The address to align.
	 * @param InAlignment: The requested alignment.
	 * @returns: The aligned address.
	 */
	static void* Align(void* InAddress, EMemoryAlignment InAlignment)
	{
		// Since InAlignment is a power of 2, use bitwise AND for a faster modulo.
		uint64 Modulo = reinterpret_cast<uint64>(InAddress) & (static_cast<uint64>(InAlignment) - 1);
		if (Modulo == 0)
		{
			return InAddress;
		}
		return reinterpret_cast<int8*>(InAddress) + (static_cast<uint64>(InAlignment) - Modulo);
	}
};
