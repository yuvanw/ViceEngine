#include "catch/catch.hpp"

#include "Strings/StringId.h"
#include "Strings/String.h"

TEST_CASE("FStringId default constructor.")
{
	FStringId StringId;
	REQUIRE(StringId.GetId() == FStringId::Null().GetId());
}

TEST_CASE("FStringId taking a C-style string.")
{
	ANSICHAR* String = "Hello";
	uint64 StringHash = GetTypeHash(String);
	FStringId StringId = FStringId(String);
	REQUIRE(StringId.GetString() == String);
	REQUIRE(StringId.GetId() == StringHash);
}

TEST_CASE("FStringId Copy Constructor")
{
	FStringId StringId1 = FStringId("Hello");
	FStringId StringId2 = StringId1;
	REQUIRE(StringId1.GetString() == StringId2.GetString());
	REQUIRE(StringId1.GetId() == StringId2.GetId());
}

TEST_CASE("FStringId Copy Assignment Operator")
{
	FStringId StringId1 = FStringId("Hello");
	FStringId StringId2;
	StringId2 = StringId1;
	REQUIRE(StringId1.GetString() == StringId2.GetString());
	REQUIRE(StringId1.GetId() == StringId2.GetId());
}

TEST_CASE("FStringId::operator==")
{
	FStringId StringId1 = FStringId("Hello");
	FStringId StringId2 = FStringId("Hello");
	FStringId StringId3 = FStringId("World");

	bool IsEqual = (StringId1 == StringId2);
	REQUIRE(IsEqual);

	bool IsNotEqual = !(StringId1 == StringId3);
	REQUIRE(IsNotEqual);
}

TEST_CASE("FStringId::operator!=")
{
	FStringId StringId1 = FStringId("Hello");
	FStringId StringId2 = FStringId("Hello");
	FStringId StringId3 = FStringId("World");

	bool IsEqual = !(StringId1 != StringId2);
	REQUIRE(IsEqual);

	bool IsNotEqual = (StringId1 != StringId3);
	REQUIRE(IsNotEqual);
}
