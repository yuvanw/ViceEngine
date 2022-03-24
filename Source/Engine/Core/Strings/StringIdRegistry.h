#pragma once

#include "Containers/Map.h"
#include "String.h"

/**
 * A registry of hashed string IDs to their original strings for fast retrieval and single storage.
 */
class FStringIdRegistry
{
public:
	static FStringIdRegistry& Get()
	{
		static FStringIdRegistry StringIdRegistry;
		return StringIdRegistry;
	}

private:
	FStringIdRegistry() = default;
	TMap<uint64, FANSIString> StringIdRegistry;

	friend class FStringId;
};
