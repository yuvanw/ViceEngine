#include "catch/catch.hpp"
#include "Containers/Array.h"

TEST_CASE("TArray default constructor.")
{
	TArray<int32> Array;
	REQUIRE(Array.GetData() == nullptr);
	REQUIRE(Array.GetSize() == 0);
	REQUIRE(Array.GetCapacity() == 0);
}

TEST_CASE("TArray constructor taking an array size.")
{
	int32 ArraySize = 10;
	TArray<int32> Array(ArraySize);
	REQUIRE(Array.GetData() != nullptr);
	REQUIRE(Array.GetSize() == 0);
	REQUIRE(Array.GetCapacity() == 10);
}

TEST_CASE("TArray::Add")
{
	TArray<int32> Array;
	Array.Add(1);

	int32* Data = Array.GetData();
	REQUIRE(Data != nullptr);
	REQUIRE(Data[0] == 1);
	REQUIRE(Array.GetSize() == 1);
	// TArray uses a growth factor of 2.
	REQUIRE(Array.GetCapacity() == 2);

	Array.Add(2);
	Array.Add(3);
	
	// Get pointer to Data array again because a reallocation has occured.
	Data = Array.GetData();
	REQUIRE(Data[0] == 1);
	REQUIRE(Data[1] == 2);
	REQUIRE(Data[2] == 3);
	REQUIRE(Array.GetSize() == 3);
	REQUIRE(Array.GetCapacity() == 4);
}

TEST_CASE("TArray::Emplace")
{
	TArray<int32> Array1;
	TArray<int32> Array2;

	SECTION("Append to an empty array.")
	{
		Array1.Add(0);
		int32* Data1 = Array1.GetData();
		REQUIRE(Data1 != nullptr);
		REQUIRE(Data1[0] == 0);
		REQUIRE(Array1.GetSize() == 1);
		REQUIRE(Array1.GetCapacity() == 2);

		Array2.Append(Array1);

		int32* Data2 = Array2.GetData();
		REQUIRE(Data2 != nullptr);
		REQUIRE(Data2[0] == 0);
		REQUIRE(Array2.GetSize() == 1);
		// Reserve exact capacity needed to perform append.
		REQUIRE(Array2.GetCapacity() == 1);

		// Array appended from is untouched.
		int32* Data3 = Array1.GetData();
		REQUIRE(Data1 == Data3);
		REQUIRE(Array1.GetSize() == 1);
		REQUIRE(Array1.GetCapacity() == 2);
	}

	SECTION("Append from an empty array.")
	{
		Array2.Add(0);
		int32* Data1 = Array2.GetData();
		REQUIRE(Data1 != nullptr);
		REQUIRE(Data1[0] == 0);
		REQUIRE(Array2.GetSize() == 1);
		REQUIRE(Array2.GetCapacity() == 2);

		Array2.Append(Array1);

		int32* Data2 = Array2.GetData();
		REQUIRE(Data1 == Data2);
		REQUIRE(Array2.GetSize() == 1);
		REQUIRE(Array2.GetCapacity() == 2);
	}

	SECTION("Append from a non-empty array.")
	{
		Array2.Add(0);
		Array1.Add(1);
		Array1.Add(2);

		Array2.Append(Array1);

		int32* Data = Array2.GetData();
		REQUIRE(Data != nullptr);
		REQUIRE(Data[0] == 0);
		REQUIRE(Data[1] == 1);
		REQUIRE(Data[2] == 2);
		REQUIRE(Array2.GetSize() == 3);
		// Reserve exact capacity needed to perform append.
		REQUIRE(Array2.GetCapacity() == 3);
	}
}

TEST_CASE("TArray::RemoveAt")
{
	TArray<int32> Array;

	SECTION("Remove element from the end of the array.")
	{
		Array.Add(1);
		Array.RemoveAt(0);

		int32* Data = Array.GetData();
		REQUIRE(Data != nullptr);
		REQUIRE(Array.GetSize() == 0);
		// Element removal does not cause capacity to shrink.
		REQUIRE(Array.GetCapacity() == 2);
	}

	SECTION("Remove element from the middle of the array.")
	{
		Array.Add(1);
		Array.Add(2);
		Array.Add(3);
		Array.Add(4);
		Array.Add(5);

		Array.RemoveAt(1);

		int32* Data = Array.GetData();
		REQUIRE(Data != nullptr);
		REQUIRE(Data[0] == 1);
		// All elements to the right of the removed element are now shifted down one slot to the left.
		REQUIRE(Data[1] == 3);
		REQUIRE(Data[2] == 4);
		REQUIRE(Data[3] == 5);
		REQUIRE(Array.GetSize() == 4);
		REQUIRE(Array.GetCapacity() == 8);
	}
}

TEST_CASE("TArray::RemoveFirst")
{
	TArray<int32> Array;

	SECTION("Element does not exist.")
	{
		Array.Add(1);

		// RemoveFirst returns false because the element was not found.
		REQUIRE(!Array.RemoveFirst(0));

		// The array remains untouched.
		int32* Data = Array.GetData();
		REQUIRE(Data != nullptr);
		REQUIRE(Data[0] == 1);
		REQUIRE(Array.GetSize() == 1);
		REQUIRE(Array.GetCapacity() == 2);
	}

	SECTION("Element exists.")
	{
		Array.Add(1);
		
		// RemoveFirst returns true because the element was found.
		REQUIRE(Array.RemoveFirst(1));

		// The requested element has been removed from the array.
		REQUIRE(Array.GetData() != nullptr);
		REQUIRE(Array.GetSize() == 0);
		REQUIRE(Array.GetCapacity() == 2);
	}

	SECTION("Duplicate elements exist.")
	{
		Array.Add(1);
		Array.Add(2);
		Array.Add(1);

		REQUIRE(Array.RemoveFirst(1));

		// Only the first instance of the requested element was removed.
		int32* Data = Array.GetData();
		REQUIRE(Data != nullptr);
		REQUIRE(Data[0] == 2);
		REQUIRE(Data[1] == 1);
		REQUIRE(Array.GetSize() == 2);
		REQUIRE(Array.GetCapacity() == 4);
	}
}

TEST_CASE("TArray::RemoveFirstByPredicate")
{
	TArray<int32> Array;

	// Predicate used for comparison.
	auto IsEqualToOne = [](int32 InElement)
	{
		return InElement == 1;
	};

	SECTION("Element does not exist.")
	{
		Array.Add(2);

		// RemoveFirst returns false because no match was found using the input predicate.
		bool bIsRemoved = Array.RemoveFirstByPredicate(IsEqualToOne);
		REQUIRE(!bIsRemoved);

		int32* Data = Array.GetData();
		REQUIRE(Data != nullptr);
		REQUIRE(Data[0] == 2);
		REQUIRE(Array.GetSize() == 1);
		REQUIRE(Array.GetCapacity() == 2);
	}

	SECTION("Element exists.")
	{
		Array.Add(1);

		// RemoveFirst returns true because a match was found using the input predicate.
		bool bIsRemoved = Array.RemoveFirstByPredicate(IsEqualToOne);
		REQUIRE(bIsRemoved);

		// The requested element has been removed from the array.
		REQUIRE(Array.GetData() != nullptr);
		REQUIRE(Array.GetSize() == 0);
		REQUIRE(Array.GetCapacity() == 2);
	}

	SECTION("Duplicate elements exist.")
	{
		Array.Add(1);
		Array.Add(2);
		Array.Add(1);

		bool bIsRemoved = Array.RemoveFirstByPredicate(IsEqualToOne);
		REQUIRE(bIsRemoved);

		// Only the first element that matched the predicate's conditions was removed.
		int32* Data = Array.GetData();
		REQUIRE(Data != nullptr);
		REQUIRE(Data[0] == 2);
		REQUIRE(Data[1] == 1);
		REQUIRE(Array.GetSize() == 2);
		REQUIRE(Array.GetCapacity() == 4);
	}
}

TEST_CASE("TArray::Reserve")
{
	TArray<int32> Array;

	SECTION("Reserving zero capacity is a no-op.")
	{
		Array.Reserve(0);
		REQUIRE(Array.GetData() == nullptr);
		REQUIRE(Array.GetSize() == 0);
		REQUIRE(Array.GetCapacity() == 0);
	}

	SECTION("Reserving capacity less than or equal to the current capacity is a no-op.")
	{
		Array.Add(0);
		Array.Add(1);
		Array.Add(2);
		Array.Add(3);

		int32* Data1 = Array.GetData();
		REQUIRE(Data1 != nullptr);
		REQUIRE(Data1[0] == 0);
		REQUIRE(Data1[1] == 1);
		REQUIRE(Data1[2] == 2);
		REQUIRE(Data1[3] == 3);
		REQUIRE(Array.GetSize() == 4);
		REQUIRE(Array.GetCapacity() == 4);

		Array.Reserve(4);

		int32* Data2 = Array.GetData();
		REQUIRE(Data1 == Data2);
		REQUIRE(Array.GetSize() == 4);
		REQUIRE(Array.GetCapacity() == 4);
	}

	SECTION("Reserving more than the current capacity.")
	{
		Array.Add(0);
		Array.Add(1);
		Array.Add(2);
		Array.Add(3);

		int32* Data1 = Array.GetData();
		REQUIRE(Data1 != nullptr);
		REQUIRE(Data1[0] == 0);
		REQUIRE(Data1[1] == 1);
		REQUIRE(Data1[2] == 2);
		REQUIRE(Data1[3] == 3);
		REQUIRE(Array.GetSize() == 4);
		REQUIRE(Array.GetCapacity() == 4);

		Array.Reserve(5);

		int32* Data2 = Array.GetData();
		REQUIRE(Data1 != Data2);
		REQUIRE(Data2 != nullptr);
		REQUIRE(Data2[0] == 0);
		REQUIRE(Data2[1] == 1);
		REQUIRE(Data2[2] == 2);
		REQUIRE(Data2[3] == 3);
		REQUIRE(Array.GetSize() == 4);
		// Reserve the requested capacity exactly.
		REQUIRE(Array.GetCapacity() == 5);
	}
}

TEST_CASE("TArray::Find")
{
	TArray<int32> Array;

	SECTION("Element does not exist.")
	{
		Array.Add(1);
		
		int32* Element = Array.Find(0);
		REQUIRE(Element == nullptr);
	}

	SECTION("Element exists.")
	{
		Array.Add(1);
	
		int32* Element = Array.Find(1);
		REQUIRE(Element != nullptr);
		REQUIRE(*Element == 1);
	}

	SECTION("Duplicate elements exist.")
	{
		Array.Add(1);
		Array.Add(1);

		// Find returns the first occurence of the requested element.
		int32* Element = &Array[0];
		int32* FoundElement = Array.Find(1);
		REQUIRE(FoundElement != nullptr);
		REQUIRE(Element == FoundElement);
	}
}

TEST_CASE("TArray::FindByPredicate")
{
	TArray<int32> Array;

	// Predicates used for comparison.
	auto IsEqualToZero = [](int32 InElement)
	{
		return InElement == 0;
	};
	auto IsEqualToOne = [](int32 InElement)
	{
		return InElement == 1;
	};

	SECTION("Element does not exist.")
	{
		Array.Add(1);

		int32* Element = Array.FindByPredicate(IsEqualToZero);
		REQUIRE(Element == nullptr);
	}

	SECTION("Element exists.")
	{
		Array.Add(1);

		int32* Element = Array.FindByPredicate(IsEqualToOne);
		REQUIRE(Element != nullptr);
		REQUIRE(*Element == 1);
	}

	SECTION("Duplicate elements exist.")
	{
		Array.Add(1);
		Array.Add(1);

		// Find returns the first occurence of the requested element.
		int32* Element = &Array[0];
		int32* FoundElement = Array.FindByPredicate(IsEqualToOne);
		REQUIRE(FoundElement != nullptr);
		REQUIRE(Element == FoundElement);
	}
}

TEST_CASE("TArray::GetIndexOf")
{
	TArray<int32> Array;

	SECTION("Element does not exist.")
	{
		Array.Add(1);

		int32 Index = Array.GetIndexOf(0);
		REQUIRE(Index == InvalidIndex);
	}

	SECTION("Element exists.")
	{
		Array.Add(1);
		Array.Add(2);
		Array.Add(3);

		int32 Index = Array.GetIndexOf(2);
		REQUIRE(Index == 1);
	}

	SECTION("Duplicate elements exist.")
	{
		Array.Add(1);
		Array.Add(2);
		Array.Add(2);

		// Find returns the first occurence of the requested element.
		int32 Index = Array.GetIndexOf(2);
		REQUIRE(Index == 1);
	}
}

TEST_CASE("TArray::GetIndexOfByPredicate")
{
	TArray<int32> Array;

	// Predicates used for comparison.
	auto IsEqualToZero = [](int32 InElement)
	{
		return InElement == 0;
	};
	auto IsEqualToTwo = [](int32 InElement)
	{
		return InElement == 2;
	};

	SECTION("Element does not exist.")
	{
		Array.Add(1);

		int32 Index = Array.GetIndexOfByPredicate(IsEqualToZero);
		REQUIRE(Index == InvalidIndex);
	}

	SECTION("Element exists.")
	{
		Array.Add(1);
		Array.Add(2);
		Array.Add(3);

		int32 Index = Array.GetIndexOfByPredicate(IsEqualToTwo);
		REQUIRE(Index == 1);
	}

	SECTION("Duplicate elements exist.")
	{
		Array.Add(1);
		Array.Add(2);
		Array.Add(2);

		// Find returns the first occurence of the requested element.
		int32 Index = Array.GetIndexOfByPredicate(IsEqualToTwo);
		REQUIRE(Index == 1);
	}
}

TEST_CASE("TArray::IsContained")
{
	TArray<int32> Array;
	Array.Add(1);

	REQUIRE(Array.IsContained(1));
	REQUIRE(!Array.IsContained(0));
}

TEST_CASE("TArray::IsContainedByPredicate")
{
	TArray<int32> Array;
	Array.Add(1);

	// Predicates used for comparison.
	auto IsEqualToZero = [](int32 InElement)
	{
		return InElement == 0;
	};
	auto IsEqualToOne = [](int32 InElement)
	{
		return InElement == 1;
	};

	REQUIRE(Array.IsContainedByPredicate(IsEqualToOne));
	REQUIRE(!Array.IsContainedByPredicate(IsEqualToZero));
}
