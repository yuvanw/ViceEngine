#include "catch/catch.hpp"

#include "SmartPointers/WeakPtr.h"

// Struct used for testing purposes
struct Base
{
	Base()
		: x(0)
	{
	}

	explicit Base(int32 InX)
		: x(InX)
	{
	}

	void MyVirtualFunc() {};

	int32 x;
};

struct Derived : public Base
{
	Derived()
		: Base()
	{
	}

	explicit Derived(int32 InX)
		: Base(InX)
	{
	}
};

TEST_CASE("TWeakPtr - Constructors")
{
	SECTION("Construct using TSharedPtr with no underlying resource.")
	{
		TSharedPtr<int32> MySharedPtr;
		TWeakPtr<int32> MyWeakPtr = MySharedPtr;
		REQUIRE(MyWeakPtr.GetStrongRefCount() == -1);
		REQUIRE(MyWeakPtr.GetWeakRefCount() == -1);
		REQUIRE(MySharedPtr.Get() == nullptr);
	}

	SECTION("Construct using TSharedPtr that is managing a resource.")
	{
		TSharedPtr<int32> MySharedPtr = TSharedPtr<int32>(new int32(5));
		TWeakPtr<int32> MyWeakPtr = MySharedPtr;
		REQUIRE(MyWeakPtr.GetStrongRefCount() == 1);
		REQUIRE(MyWeakPtr.GetWeakRefCount() == 1);
		REQUIRE(MySharedPtr.IsValid());
		REQUIRE(*MySharedPtr == 5);
	}

	SECTION("Construct using nullptr")
	{
		TWeakPtr<int32> MyWeakPtr = nullptr;
	}
}

TEST_CASE("TWeakPtr - Copy Constructor")
{
	SECTION("Copy construct with invalid weak pointer.")
	{
		TWeakPtr<int32> WeakPtr1;
		TWeakPtr<int32> WeakPtr2 = WeakPtr1;
		REQUIRE(WeakPtr2.GetStrongRefCount() == -1);
		REQUIRE(WeakPtr2.GetWeakRefCount() == -1);
		REQUIRE(!WeakPtr2.IsValid());
	}

	SECTION("Copy construct with valid weak pointer.")
	{
		TSharedPtr<int32> SharedPtr = TSharedPtr<int32>(new int32(5));
		TWeakPtr<int32> WeakPtr1 = SharedPtr;
		TWeakPtr<int32> WeakPtr2 = WeakPtr1;
		REQUIRE(WeakPtr2.GetStrongRefCount() == 1);
		REQUIRE(WeakPtr2.GetWeakRefCount() == 2);
		REQUIRE(WeakPtr2.IsValid());
	}
}

TEST_CASE("TWeakPtr - Generalized Copy Constructor")
{
	TSharedPtr<Derived> SharedPtr = TSharedPtr<Derived>(new Derived);
	TWeakPtr<Derived> WeakPtr1 = SharedPtr;
	TWeakPtr<Base> WeakPtr2 = WeakPtr1;
	REQUIRE(WeakPtr2.GetStrongRefCount() == 1);
	REQUIRE(WeakPtr2.GetWeakRefCount() == 2);
	REQUIRE(WeakPtr2.IsValid());
}

TEST_CASE("TWeakPtr - Move Constructor")
{
	TSharedPtr<int32> SharedPtr = TSharedPtr<int32>(new int32(5));
	TWeakPtr<int32> WeakPtr1 = SharedPtr;
	TWeakPtr<int32> WeakPtr2 = MoveTemp(WeakPtr1);

	REQUIRE(WeakPtr1.GetStrongRefCount() == -1);
	REQUIRE(WeakPtr1.GetWeakRefCount() == -1);
	REQUIRE(!WeakPtr1.IsValid());

	REQUIRE(WeakPtr2.GetStrongRefCount() == 1);
	REQUIRE(WeakPtr2.GetWeakRefCount() == 1);
	REQUIRE(WeakPtr2.IsValid());
}

TEST_CASE("TWeakPtr - Generalized Move Constructor")
{
	TSharedPtr<Derived> SharedPtr = TSharedPtr<Derived>(new Derived);
	TWeakPtr<Derived> WeakPtr1 = SharedPtr;
	TWeakPtr<Base> WeakPtr2 = MoveTemp(WeakPtr1);

	REQUIRE(WeakPtr1.GetStrongRefCount() == -1);
	REQUIRE(WeakPtr1.GetWeakRefCount() == -1);
	REQUIRE(!WeakPtr1.IsValid());

	REQUIRE(WeakPtr2.GetStrongRefCount() == 1);
	REQUIRE(WeakPtr2.GetWeakRefCount() == 1);
	REQUIRE(WeakPtr2.IsValid());
}

TEST_CASE("TWeakPtr - Copy Assignment Operator")
{
	SECTION("Default Copy Assignment")
	{
		TSharedPtr<int32> SharedPtr1 = TSharedPtr<int32>(new int32(5));
		TWeakPtr<int32> WeakPtr1 = SharedPtr1;
		TWeakPtr<int32> WeakPtr2;
		WeakPtr2 = WeakPtr1;

		REQUIRE(WeakPtr2.GetStrongRefCount() == 1);
		REQUIRE(WeakPtr2.GetWeakRefCount() == 2);
		REQUIRE(WeakPtr2.IsValid());

		TSharedPtr<int32> SharedPtr2 = TSharedPtr<int32>(new int32(1));
		TWeakPtr<int32> WeakPtr3 = SharedPtr2;
		WeakPtr2 = WeakPtr3;

		REQUIRE(WeakPtr1.GetStrongRefCount() == 1);
		REQUIRE(WeakPtr1.GetWeakRefCount() == 1);
		REQUIRE(WeakPtr1.IsValid());

		REQUIRE(WeakPtr2.GetStrongRefCount() == 1);
		REQUIRE(WeakPtr2.GetWeakRefCount() == 2);
		REQUIRE(WeakPtr2.IsValid());
	}

	SECTION("Default Copy Assignment: assign a default constructed TWeakPtr")
	{
		TSharedPtr<int32> SharedPtr1 = TSharedPtr<int32>(new int32(5));
		TWeakPtr<int32> WeakPtr1 = SharedPtr1;
		REQUIRE(WeakPtr1.GetStrongRefCount() == 1);
		REQUIRE(WeakPtr1.GetWeakRefCount() == 1);
		REQUIRE(WeakPtr1.IsValid());

		TWeakPtr<int32> WeakPtr2;
		WeakPtr1 = WeakPtr2;
		REQUIRE(WeakPtr1.GetStrongRefCount() == -1);
		REQUIRE(WeakPtr1.GetWeakRefCount() == -1);
		REQUIRE(!WeakPtr1.IsValid());
	}

	SECTION("Copy Assignment Operator: self-assignment")
	{
		TSharedPtr<int32> SharedPtr1 = TSharedPtr<int32>(new int32(5));
		TWeakPtr<int32> WeakPtr1 = SharedPtr1;
		TWeakPtr<int32> WeakPtr2 = WeakPtr1;
		TWeakPtr<int32> WeakPtr3 = WeakPtr1;
		REQUIRE(WeakPtr3.GetStrongRefCount() == 1);
		REQUIRE(WeakPtr3.GetWeakRefCount() == 3);
		REQUIRE(WeakPtr3.IsValid());

		WeakPtr3 = WeakPtr1;
		REQUIRE(WeakPtr3.GetStrongRefCount() == 1);
		REQUIRE(WeakPtr3.GetWeakRefCount() == 3);
		REQUIRE(WeakPtr3.IsValid());
	}

	SECTION("Copy Assignment Operator: passed a nullptr")
	{
		TSharedPtr<int32> SharedPtr1 = TSharedPtr<int32>(new int32(5));
		TWeakPtr<int32> WeakPtr = SharedPtr1;
		REQUIRE(WeakPtr.GetStrongRefCount() == 1);
		REQUIRE(WeakPtr.GetWeakRefCount() == 1);
		REQUIRE(WeakPtr.IsValid());

		WeakPtr = nullptr;
		REQUIRE(!WeakPtr.IsValid());
	}

	SECTION("Generalized Copy Assignment")
	{
		TSharedPtr<Derived> SharedPtr = TSharedPtr<Derived>(new Derived);
		TWeakPtr<Derived> WeakPtr1 = SharedPtr;
		TWeakPtr<Base> WeakPtr2;
		WeakPtr2 = WeakPtr1;

		REQUIRE(WeakPtr2.GetStrongRefCount() == 1);
		REQUIRE(WeakPtr2.GetWeakRefCount() == 2);
		REQUIRE(WeakPtr2.IsValid());
	}
}

TEST_CASE("TWeakPtr - Move Assignment Operator")
{
	SECTION("Default Move Assignment")
	{
		TSharedPtr<int32> SharedPtr = TSharedPtr<int32>(new int32(5));
		TWeakPtr<int32> WeakPtr1 = SharedPtr;
		REQUIRE(WeakPtr1.GetStrongRefCount() == 1);
		REQUIRE(WeakPtr1.GetWeakRefCount() == 1);
		REQUIRE(WeakPtr1.IsValid());

		TWeakPtr<int32> WeakPtr2;
		WeakPtr2 = MoveTemp(WeakPtr1);
		REQUIRE(WeakPtr1.GetStrongRefCount() == -1);
		REQUIRE(WeakPtr1.GetWeakRefCount() == -1);
		REQUIRE(!WeakPtr1.IsValid());

		REQUIRE(WeakPtr2.GetStrongRefCount() == 1);
		REQUIRE(WeakPtr2.GetWeakRefCount() == 1);
		REQUIRE(WeakPtr2.IsValid());
	}

	SECTION("Default Move Assignment: Self-Assignment")
	{
		TSharedPtr<int32> SharedPtr = TSharedPtr<int32>(new int32(5));
		TWeakPtr<int32> WeakPtr1 = SharedPtr;
		TWeakPtr<int32> WeakPtr2 = WeakPtr1;
		TWeakPtr<int32> WeakPtr3 = WeakPtr1;
		REQUIRE(WeakPtr3.GetStrongRefCount() == 1);
		REQUIRE(WeakPtr3.GetWeakRefCount() == 3);
		REQUIRE(WeakPtr3.IsValid());

		WeakPtr3 = MoveTemp(WeakPtr2);
		REQUIRE(WeakPtr2.GetStrongRefCount() == -1);
		REQUIRE(WeakPtr2.GetWeakRefCount() == -1);
		REQUIRE(!WeakPtr2.IsValid());

		REQUIRE(WeakPtr3.GetStrongRefCount() == 1);
		REQUIRE(WeakPtr3.GetWeakRefCount() == 2);
		REQUIRE(WeakPtr3.IsValid());
	}

	SECTION("Default Move Assignment: Self-Assignment (Same Object)")
	{
		TSharedPtr<int32> SharedPtr = TSharedPtr<int32>(new int32(5));
		TWeakPtr<int32> WeakPtr = SharedPtr;
		REQUIRE(WeakPtr.GetStrongRefCount() == 1);
		REQUIRE(WeakPtr.GetWeakRefCount() == 1);
		REQUIRE(WeakPtr.IsValid());
		
		WeakPtr = MoveTemp(WeakPtr);
		REQUIRE(WeakPtr.GetStrongRefCount() == 1);
		REQUIRE(WeakPtr.GetWeakRefCount() == 1);
		REQUIRE(WeakPtr.IsValid());
	}

	SECTION("Generalized Move Assignement")
	{
		TSharedPtr<Derived> SharedPtr = TSharedPtr<Derived>(new Derived);
		TWeakPtr<Derived> WeakPtr1 = SharedPtr;
		REQUIRE(WeakPtr1.GetStrongRefCount() == 1);
		REQUIRE(WeakPtr1.GetWeakRefCount() == 1);
		REQUIRE(WeakPtr1.IsValid());

		TWeakPtr<Base> WeakPtr2;
		WeakPtr2 = MoveTemp(WeakPtr1);
		REQUIRE(WeakPtr1.GetStrongRefCount() == -1);
		REQUIRE(WeakPtr1.GetWeakRefCount() == -1);
		REQUIRE(!WeakPtr1.IsValid());

		REQUIRE(WeakPtr2.GetStrongRefCount() == 1);
		REQUIRE(WeakPtr2.GetWeakRefCount() == 1);
		REQUIRE(WeakPtr2.IsValid());
	}

	SECTION("Generalized Move Assignment: Self-Assignment")
	{
		TSharedPtr<Derived> SharedPtr = TSharedPtr<Derived>(new Derived);
		TWeakPtr<Derived> WeakPtr1 = SharedPtr;
		TWeakPtr<Derived> WeakPtr2 = WeakPtr1;
		TWeakPtr<Derived> WeakPtr3 = WeakPtr1;
		REQUIRE(WeakPtr3.GetStrongRefCount() == 1);
		REQUIRE(WeakPtr3.GetWeakRefCount() == 3);
		REQUIRE(WeakPtr3.IsValid());

		WeakPtr3 = MoveTemp(WeakPtr2);
		REQUIRE(WeakPtr2.GetStrongRefCount() == -1);
		REQUIRE(WeakPtr2.GetWeakRefCount() == -1);
		REQUIRE(!WeakPtr2.IsValid());

		REQUIRE(WeakPtr3.GetStrongRefCount() == 1);
		REQUIRE(WeakPtr3.GetWeakRefCount() == 2);
		REQUIRE(WeakPtr3.IsValid());
	}
}

TEST_CASE("TWeakPtr - Destructor")
{
	TSharedPtr<int32> SharedPtr1 = TSharedPtr<int32>(new int32(5));
	{
		TWeakPtr<int32> WeakPtr1 = SharedPtr1;
		REQUIRE(WeakPtr1.GetStrongRefCount() == 1);
		REQUIRE(WeakPtr1.GetWeakRefCount() == 1);
		REQUIRE(WeakPtr1.IsValid());

		{
			TWeakPtr<int32> WeakPtr2 = SharedPtr1;
			REQUIRE(WeakPtr2.GetStrongRefCount() == 1);
			REQUIRE(WeakPtr2.GetWeakRefCount() == 2);
			REQUIRE(WeakPtr2.IsValid());
		}

		REQUIRE(WeakPtr1.GetStrongRefCount() == 1);
		REQUIRE(WeakPtr1.GetWeakRefCount() == 1);
		REQUIRE(WeakPtr1.IsValid());
	}

	REQUIRE(SharedPtr1.GetStrongRefCount() == 1);
	REQUIRE(SharedPtr1.GetWeakRefCount() == 0);
	REQUIRE(SharedPtr1.IsValid());
}

TEST_CASE("TWeakPtr - Pin")
{
	SECTION("Call Pin() on a TWeakPtr without an underlying resource.")
	{
		TWeakPtr<int32> MyWeakPtr;
		TSharedPtr<int32>  MySharedPtr = MyWeakPtr.Pin();
		REQUIRE(MyWeakPtr.GetStrongRefCount() == -1);
		REQUIRE(MyWeakPtr.GetWeakRefCount() == -1);
		REQUIRE(!MySharedPtr.IsValid());
	}

	SECTION("Call Pin() on a TWeakPtr with an underlying resource.")
	{
		TSharedPtr<int32> SharedPtr1 = TSharedPtr<int32>(new int32(5));
		TWeakPtr<int32> WeakPtr = SharedPtr1;
		TSharedPtr<int32> SharedPtr2 = WeakPtr.Pin();
		REQUIRE(SharedPtr2.GetStrongRefCount() == 2);
		REQUIRE(SharedPtr2.GetWeakRefCount() == 1);
		REQUIRE(SharedPtr2.IsValid());
	}
}

TEST_CASE("TWeakPtr - Assign from TSharedPtr")
{
	TSharedPtr<int32> SharedPtr1 = TSharedPtr<int32>(new int32(5));
	TSharedPtr<int32> SharedPtr2 = TSharedPtr<int32>(new int32(4));

	TWeakPtr<int32> WeakPtr = SharedPtr1;
	REQUIRE(WeakPtr.GetStrongRefCount() == 1);
	REQUIRE(WeakPtr.GetWeakRefCount() == 1);
	REQUIRE(WeakPtr.IsValid());

	REQUIRE(SharedPtr2.GetStrongRefCount() == 1);
	REQUIRE(SharedPtr2.GetWeakRefCount() == 0);
	REQUIRE(SharedPtr2.IsValid());

	WeakPtr = SharedPtr2;
	REQUIRE(WeakPtr.GetStrongRefCount() == 1);
	REQUIRE(WeakPtr.GetWeakRefCount() == 1);
	REQUIRE(WeakPtr.IsValid());

	REQUIRE(SharedPtr1.GetStrongRefCount() == 1);
	REQUIRE(SharedPtr1.GetWeakRefCount() == 0);
	REQUIRE(SharedPtr1.IsValid());
}

TEST_CASE("TWeakPtr - IsValid")
{
	SECTION("Valid weak pointer.")
	{
		TSharedPtr<int32> SharedPtr = TSharedPtr<int32>(new int32(5));
		TWeakPtr<int32> WeakPtr = SharedPtr;
		REQUIRE(WeakPtr.IsValid());
	}

	SECTION("Default constructed TWeakPtr is invalid.")
	{
		TWeakPtr<int32> WeakPtr;
		REQUIRE(!WeakPtr.IsValid());
	}

	SECTION("TWeakPtr pointing to a destroyed object is invalid.")
	{
		TWeakPtr<int32> WeakPtr;
		{
			TSharedPtr<int32> SharedPtr = TSharedPtr<int32>(new int32(5));
			WeakPtr = SharedPtr;
		}
		REQUIRE(!WeakPtr.IsValid());
	}
}
