#include "catch/catch.hpp"

#include "CoreGlobals.h"
#include "SmartPointers/UniquePtr.h"

// Structs used for testing generalized functions.
struct Base
{
	Base() : X(0) {}
	explicit Base(int32 InX) : X(InX) {}
	void MyVirtualFunc() {};
	int32 X;
};

struct Derived : public Base
{
	Derived() : Base() {}
	explicit Derived(int32 InX) : Base(InX) {}
};

TEST_CASE("TUniquePtr default constructor.")
{
	TUniquePtr<int32> IntPtr;
	REQUIRE(IntPtr.Get() == nullptr);
}

TEST_CASE("TUniquePtr constructor taking a nullptr.")
{
	TUniquePtr<int32> IntPtr = nullptr;
	REQUIRE(IntPtr.Get() == nullptr);
}

TEST_CASE("TUniquePtr assignment operator taking a nullptr.")
{
	TUniquePtr<int32> IntPtr(new int32(5));
	REQUIRE(IntPtr.Get() != nullptr);
	REQUIRE(*IntPtr.Get() == 5);

	IntPtr = nullptr;
	REQUIRE(IntPtr.Get() == nullptr);
}

TEST_CASE("TUniquePtr constructor taking a raw pointer.")
{
	SECTION("Built-in type.")
	{
		TUniquePtr<int32> IntPtr(new int32(5));
		REQUIRE(IntPtr.Get() != nullptr);
		REQUIRE(*IntPtr.Get() == 5);
	}

	SECTION("User-defined type.")
	{
		TUniquePtr<Base> BasePtr(new Base(5));
		REQUIRE(BasePtr.Get() != nullptr);
		REQUIRE(BasePtr.Get()->X == 5);
	}

	SECTION("Polymorphism.")
	{
		TUniquePtr<Base> BasePtr(new Derived(5));
		REQUIRE(BasePtr.Get() != nullptr);
		REQUIRE(BasePtr.Get()->X == 5);
	}
}

TEST_CASE("MakeUnique")
{
	SECTION("Built-in type.")
	{
		TUniquePtr<int32> IntPtr = MakeUnique<int32>(5);
		REQUIRE(IntPtr.Get() != nullptr);
		REQUIRE(*IntPtr.Get() == 5);
	}

	SECTION("User-defined type.")
	{
		TUniquePtr<Base> BasePtr = MakeUnique<Base>(5);
		REQUIRE(BasePtr.Get() != nullptr);
		REQUIRE(BasePtr.Get()->X == 5);
	}

	SECTION("Polymorphism.")
	{
		TUniquePtr<Base> BasePtr = MakeUnique<Derived>(5);
		REQUIRE(BasePtr.Get() != nullptr);
		REQUIRE(BasePtr.Get()->X == 5);
	}
}

TEST_CASE("TUniquePtr move constructor.")
{
	TUniquePtr<int32> IntPtr1(new int32(5));
	TUniquePtr<int32> IntPtr2(MoveTemp(IntPtr1));
	REQUIRE(IntPtr1.Get() == nullptr);
	REQUIRE(IntPtr2.Get() != nullptr);
	REQUIRE(*IntPtr2.Get() == 5);
}

TEST_CASE("TUniquePtr generalized move constructor.")
{
	TUniquePtr<Derived> DerivedPtr(new Derived(5));
	TUniquePtr<Base> BasePtr(MoveTemp(DerivedPtr));
	REQUIRE(DerivedPtr.Get() == nullptr);
	REQUIRE(BasePtr.Get() != nullptr);
	REQUIRE(BasePtr.Get()->X == 5);
}

TEST_CASE("TUniquePtr move assignment operator.")
{
	TUniquePtr<int32> IntPtr1(new int32(5));
	TUniquePtr<int32> IntPtr2;
	
	IntPtr2 = MoveTemp(IntPtr1);
	
	REQUIRE(IntPtr1.Get() == nullptr);
	REQUIRE(IntPtr2.Get() != nullptr);
	REQUIRE(*IntPtr2.Get() == 5);
}

TEST_CASE("TUniquePtr generalized move assignment operator.")
{
	TUniquePtr<Derived> DerivedPtr(new Derived(5));
	TUniquePtr<Base> BasePtr;

	BasePtr = MoveTemp(DerivedPtr);

	REQUIRE(DerivedPtr.Get() == nullptr);
	REQUIRE(BasePtr.Get() != nullptr);
	REQUIRE(BasePtr.Get()->X == 5);
}

TEST_CASE("TUniquePtr::operator*")
{
	TUniquePtr<int32> IntPtr(new int32(5));
	REQUIRE(*IntPtr == 5);

	TUniquePtr<Base> MyStructPtr(new Base(5));
	REQUIRE((*MyStructPtr).X == 5);
}

TEST_CASE("TUniquePtr::operator->")
{
	TUniquePtr<Base> MyStructPtr(new Base(5));
	REQUIRE(MyStructPtr->X == 5);
}

TEST_CASE("TUniquePtr::operator bool")
{
	TUniquePtr<int32> IntPtr(new int32(5));
	REQUIRE(IntPtr);
}

TEST_CASE("TUniquePtr::operator!")
{
	TUniquePtr<int32> IntPtr;
	REQUIRE(!IntPtr);
}

TEST_CASE("TUniquePtr::Get")
{
	int32* Object = new int32(5);
	TUniquePtr<int32> IntPtr(Object);
	REQUIRE(IntPtr.Get() == Object);
}

TEST_CASE("TUniquePtr::IsValid")
{
	TUniquePtr<int32> NonNullPtr(new int32(5));
	TUniquePtr<int32> NullPtr(nullptr);
	REQUIRE(NonNullPtr.IsValid());
	REQUIRE(!NullPtr.IsValid());
}

TEST_CASE("TUniquePtr::Reset")
{
	SECTION("Reset a nullptr with a nullptr.")
	{
		TUniquePtr<int32> IntPtr;
		REQUIRE(IntPtr.Get() == nullptr);

		IntPtr.Reset();
		REQUIRE(IntPtr.Get() == nullptr);
	}

	SECTION("Reset a nullptr with a non-null pointer.")
	{
		TUniquePtr<int32> IntPtr;
		REQUIRE(IntPtr.Get() == nullptr);

		IntPtr.Reset(new int32(5));
		REQUIRE(IntPtr.Get() != nullptr);
		REQUIRE(*IntPtr.Get() == 5);
	}

	SECTION("Reset a non-null pointer with a nullptr.")
	{
		TUniquePtr<int32> IntPtr(new int32(5));
		REQUIRE(IntPtr.Get() != nullptr);
		REQUIRE(*IntPtr.Get() == 5);

		IntPtr.Reset();
		REQUIRE(IntPtr.Get() == nullptr);
	}

	SECTION("Reset a non-null pointer with a different non-null pointer.")
	{
		TUniquePtr<int32> IntPtr(new int32(5));
		REQUIRE(IntPtr.Get() != nullptr);
		REQUIRE(*IntPtr.Get() == 5);

		IntPtr.Reset(new int32(4));
		REQUIRE(IntPtr.Get() != nullptr);
		REQUIRE(*IntPtr.Get() == 4);
	}

	/**
	 * Self-assignment is not possible unless the API is being misused.
	 * (i.e. creating multiple TUniquePtrs from the same raw pointer).
	 */
	SECTION("Self-assignment.")
	{
		TUniquePtr<int32> IntPtr(new int32(5));
		REQUIRE(IntPtr.Get() != nullptr);
		REQUIRE(*IntPtr.Get() == 5);

		IntPtr.Reset(new int32(4));
		REQUIRE(IntPtr.Get() != nullptr);
		REQUIRE(*IntPtr.Get() == 4);
	}
}

TEST_CASE("TUniquePtr::operator==")
{
	TUniquePtr<int32> IntPtr1;
	TUniquePtr<int32> IntPtr2;
	REQUIRE(IntPtr1 == IntPtr2);
	REQUIRE(IntPtr1 == nullptr);
	REQUIRE(nullptr == IntPtr1);

	IntPtr1.Reset(new int32(5));
	IntPtr2.Reset(new int32(5));

	// Comparing pointers, not the data they point to.
	REQUIRE(!(IntPtr1 == IntPtr2));
	REQUIRE(!(IntPtr1 == nullptr));
	REQUIRE(!(nullptr == IntPtr1));
}

TEST_CASE("TUniquePtr::operator!=")
{
	TUniquePtr<int32> IntPtr1;
	TUniquePtr<int32> IntPtr2;
	REQUIRE(!(IntPtr1 != IntPtr2));
	REQUIRE(!(IntPtr1 != nullptr));
	REQUIRE(!(nullptr != IntPtr1));

	IntPtr1.Reset(new int32(5));
	IntPtr2.Reset(new int32(5));

	// Comparing pointers, not the data they point to.
	REQUIRE(IntPtr1 != IntPtr2);
	REQUIRE(IntPtr1 != nullptr);
	REQUIRE(nullptr != IntPtr1);
}
