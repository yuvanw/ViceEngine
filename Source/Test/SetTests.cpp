#include "catch/catch.hpp"

#include "Containers/Set.h"
#include "Math/MathUtilities.h"

TEST_CASE("TSet") 
{
	TSet<int> TestSet;

	REQUIRE(TestSet.IsEmpty());
	REQUIRE(TestSet.GetSize() == 0);

    SECTION("Add and remove one element.")
    {
		TestSet.Add(1);

		REQUIRE(!TestSet.IsEmpty());
		REQUIRE(TestSet.GetSize() == 1);
		REQUIRE(TestSet.Find(1) != nullptr);
		REQUIRE(*TestSet.Find(1) == 1);

		TestSet.Remove(1);

		REQUIRE(TestSet.IsEmpty());
		REQUIRE(TestSet.GetSize() == 0);
		REQUIRE(TestSet.Find(1) == nullptr);
    }

	SECTION("Add duplicate element.")
	{
		TestSet.Add(1);

		REQUIRE(!TestSet.IsEmpty());
		REQUIRE(TestSet.GetSize() == 1);
		REQUIRE(TestSet.Find(1) != nullptr);
		REQUIRE(*TestSet.Find(1) == 1);

		TestSet.Add(1);

		// Duplicates aren't allowed by default
		REQUIRE(!TestSet.IsEmpty());
		REQUIRE(TestSet.GetSize() == 1);
		REQUIRE(TestSet.Find(1) != nullptr);
		REQUIRE(*TestSet.Find(1) == 1);
	}

	SECTION("Remove a non-existent element.")
	{
		TestSet.Add(1);

		REQUIRE(!TestSet.IsEmpty());
		REQUIRE(TestSet.GetSize() == 1);
		REQUIRE(TestSet.Find(1) != nullptr);
		REQUIRE(*TestSet.Find(1) == 1);

		TestSet.Remove(0);

		REQUIRE(!TestSet.IsEmpty());
		REQUIRE(TestSet.GetSize() == 1);
		REQUIRE(TestSet.Find(1) != nullptr);
		REQUIRE(*TestSet.Find(1) == 1);
	}

	SECTION("Add enough elements to trigger a rehash.")
	{
		REQUIRE(TestSet.GetCapacity() == 8);

		int NumElementsForRehash = FMath::Ceil(TestSet.GetRehashLoadFactor() * TestSet.GetCapacity()) + 1;
		for (int32 Index = 0; Index < NumElementsForRehash; ++Index)
		{
			TestSet.Add(Index);
		}

		REQUIRE(!TestSet.IsEmpty());
		REQUIRE(TestSet.GetCapacity() == 16);
		REQUIRE(TestSet.GetSize() == 8);
		for (int32 Index = 0; Index < NumElementsForRehash; ++Index)
		{
			int* Element = TestSet.Find(Index);
			REQUIRE(Element != nullptr);
			REQUIRE(*Element == Index);
		}
	}
}
