#include "catch/catch.hpp"
#include "Strings/String.h"
#include "Templates/TemplateFunctionLibrary.h"

TEST_CASE("FANSIString StrLen.")
{
	// StrLen disregards the null-terminating character.
	REQUIRE(StrLen("") == 0);
	REQUIRE(StrLen("Hello") == 5);
}

TEST_CASE("FANSIString default constructor.")
{
	FANSIString String;
	REQUIRE(String.GetData() == nullptr);
	REQUIRE(String.GetSize() == 0);
	REQUIRE(String.GetCapacity() == 0);
}

TEST_CASE("FANSIString constructor taking a C-style string.")
{
	SECTION("Constructing from a nullptr is a no-op.")
	{
		FANSIString String = nullptr;
		REQUIRE(String.GetData() == nullptr);
		REQUIRE(String.GetSize() == 0);
		REQUIRE(String.GetCapacity() == 0);
	}

	SECTION("Constructing from an empty string is a no-op.")
	{
		FANSIString String = "";
		REQUIRE(String.GetData() == nullptr);
		REQUIRE(String.GetSize() == 0);
		REQUIRE(String.GetCapacity() == 0);
	}

	SECTION("Constructing from a non-empty string.")
	{
		FANSIString String = "Hello";
		REQUIRE(String.GetData() != nullptr);
		REQUIRE(String.GetSize() == 5);
		REQUIRE(String.GetCapacity() == 6);
		REQUIRE(String == "Hello");
	}
}

TEST_CASE("FANSIString copy constructor.")
{
	SECTION("Copying from an empty string is a no-op.")
	{
		FANSIString String1;
		FANSIString String2 = String1;
		REQUIRE(String2.GetData() == nullptr);
		REQUIRE(String2.GetSize() == 0);
		REQUIRE(String2.GetCapacity() == 0);
	}

	SECTION("Copying from a non-empty string.")
	{
		FANSIString String1 = "Hello";
		FANSIString String2 = String1;
		REQUIRE(String2.GetData() != nullptr);
		REQUIRE(String1.GetSize() == String2.GetSize());
		REQUIRE(String1.GetCapacity() == String2.GetCapacity());
		REQUIRE(String1 == String2);
	}
}

TEST_CASE("FANSIString move constructor.")
{
	FANSIString String1 = "Hello";
	char* Data = String1.GetData();
	int32 Size = String1.GetSize();
	int32 Capacity = String1.GetCapacity();
	IAllocator* Allocator = String1.GetAllocator();

	FANSIString String2 = MoveTemp(String1);
	REQUIRE(String2.GetData() == Data);
	REQUIRE(String2.GetSize() == Size);
	REQUIRE(String2.GetCapacity() == Capacity);
	REQUIRE(String2.GetAllocator() == Allocator);
}

TEST_CASE("FANSIString copy assignment operator.")
{
	SECTION("Self-assingment is a no-op")
	{
		FANSIString String = "Hello";
		String = String;
		REQUIRE(String.GetData() != nullptr);
		REQUIRE(String.GetSize() == 5);
		REQUIRE(String.GetCapacity() == 6);
	}

	SECTION("Assigning to an empty string.")
	{
		FANSIString String1;
		FANSIString String2 = "Hello";
		String1 = String2;
		REQUIRE(String1.GetData() != nullptr);
		REQUIRE(String1.GetSize() == 5);
		REQUIRE(String1.GetCapacity() == 6);
	}

	SECTION("Assigning to a non-empty string.")
	{
		FANSIString String1 = "Hello";

		// Reuse the Data array if it's large enough to store the input String1.
		FANSIString String2 = "Hi";
		String1 = String2;
		REQUIRE(String1.GetData() != nullptr);
		REQUIRE(String1.GetSize() == 2);
		REQUIRE(String1.GetCapacity() == 6);

		// Allocate more space if the Data array isn't large enough to store the input String1.
		FANSIString String3 = "Hello, world!";
		String1 = String3;
		REQUIRE(String1.GetData() != nullptr);
		REQUIRE(String1.GetSize() == 13);
		REQUIRE(String1.GetCapacity() == 14);
	}
}

TEST_CASE("FANSIString assignment operator taking a C-style string.")
{
	SECTION("Assigning to an empty string.")
	{
		FANSIString String;
		String = "Hello";
		REQUIRE(String.GetData() != nullptr);
		REQUIRE(String.GetSize() == 5);
		REQUIRE(String.GetCapacity() == 6);
	}

	SECTION("Assigning to a non-empty string.")
	{
		FANSIString String = "Hello";

		// Reuse the Data array if it's large enough to store the input string.
		String = "Hi";
		REQUIRE(String.GetData() != nullptr);
		REQUIRE(String.GetSize() == 2);
		REQUIRE(String.GetCapacity() == 6);

		// Allocate more space if the Data array isn't large enough to store the input string.
		String = "Hello, world!";
		REQUIRE(String.GetData() != nullptr);
		REQUIRE(String.GetSize() == 13);
		REQUIRE(String.GetCapacity() == 14);
	}
}

TEST_CASE("FANSIString move assignment operator.")
{
	SECTION("Self-assignment is a no-op.")
	{
		FANSIString String = "Hello";
		String = MoveTemp(String);
		REQUIRE(String.GetData() != nullptr);
		REQUIRE(String.GetSize() == 5);
		REQUIRE(String.GetCapacity() == 6);
		REQUIRE(String.GetAllocator() != nullptr);
	}

	SECTION("Assigning to an empty string.")
	{
		FANSIString String1 = "Hello";
		char* Data = String1.GetData();
		int32 Size = String1.GetSize();
		int32 Capacity = String1.GetCapacity();
		IAllocator* Allocator = String1.GetAllocator();

		FANSIString String2;
		String2 = MoveTemp(String1);

		REQUIRE(String2.GetData() == Data);
		REQUIRE(String2.GetSize() == Size);
		REQUIRE(String2.GetCapacity() == Capacity);
		REQUIRE(String2.GetAllocator() == Allocator);

		REQUIRE(String1.GetData() == nullptr);
		REQUIRE(String1.GetSize() == 0);
		REQUIRE(String1.GetCapacity() == 0);
		REQUIRE(String1.GetAllocator() == nullptr);
	}

	SECTION("Assigning to a non-empty string.")
	{
		FANSIString String1 = "Hello";
		char* Data = String1.GetData();
		int32 Size = String1.GetSize();
		int32 Capacity = String1.GetCapacity();
		IAllocator* Allocator = String1.GetAllocator();

		FANSIString String2 = "World";
		String2 = MoveTemp(String1);
		REQUIRE(String2.GetData() == Data);
		REQUIRE(String2.GetSize() == Size);
		REQUIRE(String2.GetCapacity() == Capacity);
		REQUIRE(String2.GetAllocator() == Allocator);

		REQUIRE(String1.GetData() == nullptr);
		REQUIRE(String1.GetSize() == 0);
		REQUIRE(String1.GetCapacity() == 0);
		REQUIRE(String1.GetAllocator() == nullptr);
	}
}

TEST_CASE("FANSIString::Substring")
{
	FANSIString String = "Hello";

	SECTION("Start index is out of bounds.")
	{
		FANSIString Substring = String.Substring(-1, 5);
		REQUIRE(Substring.GetData() != nullptr);
		REQUIRE(Substring.GetSize() == 5);
		REQUIRE(Substring.GetCapacity() == 6);
		REQUIRE(Substring == String);
	}

	SECTION("End index is out of bounds.")
	{
		FANSIString Substring = String.Substring(0, 6);
		REQUIRE(Substring.GetData() != nullptr);
		REQUIRE(Substring.GetSize() == 5);
		REQUIRE(Substring.GetCapacity() == 6);
		REQUIRE(Substring == String);
	}

	SECTION("Start and end indices are swapped.")
	{
		FANSIString Substring = String.Substring(5, 0);
		REQUIRE(Substring.GetData() == nullptr);
		REQUIRE(Substring.GetSize() == 0);
		REQUIRE(Substring.GetCapacity() == 0);
	}

	SECTION("Start and end indices are out of bounds.")
	{
		FANSIString Substring = String.Substring(-1, 6);
		REQUIRE(Substring.GetData() != nullptr);
		REQUIRE(Substring.GetSize() == 5);
		REQUIRE(Substring.GetCapacity() == 6);
		REQUIRE(Substring == String);
	}

	SECTION("Start and end indices are the same.")
	{
		FANSIString Substring = String.Substring(1, 1);
		REQUIRE(Substring.GetData() == nullptr);
		REQUIRE(Substring.GetSize() == 0);
		REQUIRE(Substring.GetCapacity() == 0);
	}

	SECTION("Start and end indices are within the bounds of the string.")
	{
		FANSIString Substring = String.Substring(1, 3);
		REQUIRE(Substring.GetData() != nullptr);
		REQUIRE(Substring.GetSize() == 2);
		REQUIRE(Substring.GetCapacity() == 3);
		REQUIRE(Substring == "el");
	}
}

TEST_CASE("FANSIString::Reserve")
{

	SECTION("Reserving 0 or less characters is a no-op.")
	{
		FANSIString String;
	
		String.Reserve(0);
		REQUIRE(String.GetData() == nullptr);
		REQUIRE(String.GetSize() == 0);
		REQUIRE(String.GetCapacity() == 0);

		String.Reserve(-1);
		REQUIRE(String.GetData() == nullptr);
		REQUIRE(String.GetSize() == 0);
		REQUIRE(String.GetCapacity() == 0);
	}

	SECTION("Reserving space for an empty string.")
	{
		FANSIString String;
		
		String.Reserve(5);
		REQUIRE(String.GetData() != nullptr);
		REQUIRE(String.GetSize() == 0);
		REQUIRE(String.GetCapacity() == 6);
	}

	SECTION("Reserving space for a non-empty string.")
	{
		FANSIString String = "Hello";
		REQUIRE(String.GetData() != nullptr);
		REQUIRE(String.GetSize() == 5);
		REQUIRE(String.GetCapacity() == 6);

		// Requested capacity (InCharacterCount + 1) is less than the current capacity: no-op.
		String.Reserve(4);
		REQUIRE(String.GetData() != nullptr);
		REQUIRE(String.GetSize() == 5);
		REQUIRE(String.GetCapacity() == 6);

		// Requested capacity (InCharacterCount + 1) is greater than the current capacity: reserve additional space.
		String.Reserve(6);
		REQUIRE(String.GetData() != nullptr);
		REQUIRE(String.GetSize() == 5);
		REQUIRE(String.GetCapacity() == 7);
	}
}
