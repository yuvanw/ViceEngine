#pragma once

#include "CoreGlobals.h"

class FCrc64
{
public:
	static uint64 GetTypeHash(const void* InData, int32 InSize, uint64 InCrc = 0);

private:
	// Look up table for CRC64 values.
	static const uint64 Crc64Table[256];
};
