#include "catch/catch.hpp"

#include "Containers/Map.h"
#include "Math/MathUtilities.h"

TEST_CASE("TMap")
{
	TMap<int, int> TestMap;

	REQUIRE(TestMap.IsEmpty());
	REQUIRE(TestMap.GetSize() == 0);

	SECTION("Add and remove one element.")
	{
		TestMap.Add(1, -1);

		REQUIRE(!TestMap.IsEmpty());
		REQUIRE(TestMap.GetSize() == 1);
		REQUIRE(TestMap.Find(1) != nullptr);
		REQUIRE(*TestMap.Find(1) == -1);

		TestMap.Remove(1, -1);

		REQUIRE(TestMap.IsEmpty());
		REQUIRE(TestMap.GetSize() == 0);
		REQUIRE(TestMap.Find(1) == nullptr);
	}

	SECTION("Add duplicate element.")
	{
		TestMap.Add(1, -1);

		REQUIRE(!TestMap.IsEmpty());
		REQUIRE(TestMap.GetSize() == 1);
		REQUIRE(TestMap.Find(1) != nullptr);
		REQUIRE(*TestMap.Find(1) == -1);

		TestMap.Add(1, -1);

		// Duplicates aren't allowed by default
		REQUIRE(!TestMap.IsEmpty());
		REQUIRE(TestMap.GetSize() == 1);
		REQUIRE(TestMap.Find(1) != nullptr);
		REQUIRE(*TestMap.Find(1) == -1);
	}

	SECTION("Remove a non-existent element.")
	{
		TestMap.Add(1, -1);

		REQUIRE(!TestMap.IsEmpty());
		REQUIRE(TestMap.GetSize() == 1);
		REQUIRE(TestMap.Find(1) != nullptr);
		REQUIRE(*TestMap.Find(1) == -1);

		TestMap.Remove(0);

		REQUIRE(!TestMap.IsEmpty());
		REQUIRE(TestMap.GetSize() == 1);
		REQUIRE(TestMap.Find(1) != nullptr);
		REQUIRE(*TestMap.Find(1) == -1);
	}

	SECTION("Add enough elements to trigger a rehash.")
	{
		REQUIRE(TestMap.GetCapacity() == 8);

		int NumElementsForRehash = FMath::Ceil(TestMap.GetRehashLoadFactor() * TestMap.GetCapacity()) + 1;
		for (int32 Index = 0; Index < NumElementsForRehash; ++Index)
		{
			TestMap.Add(Index, -Index);
		}

		REQUIRE(!TestMap.IsEmpty());
		REQUIRE(TestMap.GetSize() == 8);
		REQUIRE(TestMap.GetCapacity() == 16);
		for (int32 Index = 0; Index < NumElementsForRehash; ++Index)
		{
			int* Element = TestMap.Find(Index);
			REQUIRE(Element != nullptr);
			REQUIRE(*Element == -Index);
		}
	}
}
