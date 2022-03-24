#pragma once

#include "CoreGlobals.h"
#include "Containers/Map.h"
#include "Strings/String.h"
#include "StringIdRegistry.h"

/*
* Hashed string ID for fast string comparisons. Only ANSI strings are accepted.
* Refer to Unreal Engine's FName for the design inspiration.
*/
class FStringId
{
public:
	// Constructors.
	FStringId();
	FStringId(const ANSICHAR* InANSIString);

	// Copy operations.
	FStringId(const FStringId& InStringId) = default;
	FStringId& operator=(const FStringId& InStringId) = default;

	uint64 GetId() const;

	const FANSIString& GetString() const;

	bool operator==(const FStringId& InStringId) const;
	bool operator!=(const FStringId& InStringId) const;

	static FStringId Null();

private:
	// ID obtained by hashing the associated string.
	uint64 Id;

	// A registry of hashed string IDs to their original strings for fast retrieval and single storage.
	static const FStringId NullStringId;
};

// Support FStringId hashing.
inline uint64 GetTypeHash(const FStringId& InStringId)
{
	return InStringId.GetId();
}
