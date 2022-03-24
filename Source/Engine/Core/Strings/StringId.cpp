#include "StringId.h"
#include "StringIdRegistry.h"

const FStringId FStringId::NullStringId("");

FStringId::FStringId()
	: Id(FStringId::Null().GetId())
{
}

FStringId::FStringId(const ANSICHAR* InANSIString)
	: Id(GetTypeHash(InANSIString))
{
	if (!FStringIdRegistry::Get().StringIdRegistry.IsKeyContained(Id))
	{
		FStringIdRegistry::Get().StringIdRegistry.Add(Id, InANSIString);
	}
}

uint64 FStringId::GetId() const
{
	return Id;
}

const FANSIString& FStringId::GetString() const
{
	FANSIString* String = FStringIdRegistry::Get().StringIdRegistry.Find(Id);
	// All FStringIds should be stored in the FStringIdRegistry.
	ensure(String);
	return *String;
}

FStringId FStringId::Null()
{
	return FStringId::NullStringId;
}

bool FStringId::operator==(const FStringId& InStringId) const
{
	// Fast comparison achieved by comparing integers instead of strings.
	return Id == InStringId.Id;
}

bool FStringId::operator!=(const FStringId& InStringId) const
{
	return Id != InStringId.Id;
}
