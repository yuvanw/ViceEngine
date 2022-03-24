#include "catch/catch.hpp"

#include "SmartPointers/SharedPtr.h"

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

TEST_CASE("MakeShared")
{
	SECTION("Built-in type")
	{
		TSharedPtr<int> IntPtr = MakeShared<int>();
		REQUIRE(IntPtr.GetStrongRefCount() == 1);
		REQUIRE(IntPtr.GetWeakRefCount() == 0);
		REQUIRE(IntPtr.IsValid());

		*IntPtr = 5;
		REQUIRE(*IntPtr == 5);
	}

	SECTION("User-defined type")
	{
		TSharedPtr<Base> BasePtr = MakeShared<Base>(5);
		REQUIRE(BasePtr.GetStrongRefCount() == 1);
		REQUIRE(BasePtr.GetWeakRefCount() == 0);
		REQUIRE(BasePtr.IsValid());
		REQUIRE(BasePtr->x == 5);
	}

	SECTION("Polymorphism")
	{
		TSharedPtr<Base> BasePtr = MakeShared<Derived>(5);
		REQUIRE(BasePtr.GetStrongRefCount() == 1);
		REQUIRE(BasePtr.GetWeakRefCount() == 0);
		REQUIRE(BasePtr.IsValid());
		REQUIRE(BasePtr->x == 5);
	}
}

TEST_CASE("TSharedPtr Constructors")
{
	SECTION("Default constructor.")
	{
		TSharedPtr<int32> IntPtr;
		REQUIRE(IntPtr.GetStrongRefCount() == -1);
		REQUIRE(IntPtr.GetWeakRefCount() == -1);
		REQUIRE(IntPtr.Get() == nullptr);
	}

	SECTION("Constuct with a raw pointer.")
	{
		TSharedPtr<int32> IntPtr = TSharedPtr<int32>(new int32(5));
		REQUIRE(IntPtr.GetStrongRefCount() == 1);
		REQUIRE(IntPtr.GetWeakRefCount() == 0);
		REQUIRE(IntPtr.IsValid());
		REQUIRE(*IntPtr == 5);
	}
}

TEST_CASE("Copy Constructor")
{
	TSharedPtr<int32> IntPtr = TSharedPtr<int32>(new int32(5));
	REQUIRE(IntPtr.GetStrongRefCount() == 1);
	REQUIRE(IntPtr.GetWeakRefCount() == 0);
	REQUIRE(IntPtr.IsValid());
	REQUIRE(*IntPtr == 5);

	TSharedPtr<int32> OtherIntPtr = IntPtr;
	REQUIRE(IntPtr.GetStrongRefCount() == 2);
	REQUIRE(IntPtr.GetWeakRefCount() == 0);
	REQUIRE(IntPtr.IsValid());
	REQUIRE(*IntPtr == 5);
}

TEST_CASE("Move Constructor")
{
	int32* RawIntPtr = new int32(5);
	TSharedPtr<int32> IntPtr = TSharedPtr<int32>(RawIntPtr);
	REQUIRE(IntPtr.GetStrongRefCount() == 1);
	REQUIRE(IntPtr.GetWeakRefCount() == 0);
	REQUIRE(IntPtr.IsValid());
	REQUIRE(*IntPtr == 5);

	TSharedPtr<int32> OtherIntPtr = MoveTemp(IntPtr);
	REQUIRE(IntPtr.Get() == nullptr);
	REQUIRE(IntPtr.GetStrongRefCount() == -1);
	REQUIRE(IntPtr.GetWeakRefCount() == -1);

	REQUIRE(OtherIntPtr.GetStrongRefCount() == 1);
	REQUIRE(OtherIntPtr.GetWeakRefCount() == 0);
	REQUIRE(OtherIntPtr.IsValid());
	REQUIRE(*OtherIntPtr == 5);

	REQUIRE(OtherIntPtr.Get() == RawIntPtr);
}

TEST_CASE("Generalized Move Constructor")
{
	Derived* RawDerivedPtr = new Derived;
	TSharedPtr<Derived> DerivedPtr = TSharedPtr<Derived>(RawDerivedPtr);
	REQUIRE(DerivedPtr.GetStrongRefCount() == 1);
	REQUIRE(DerivedPtr.GetWeakRefCount() == 0);
	REQUIRE(DerivedPtr.IsValid());

	TSharedPtr<Base> BasePtr = MoveTemp(DerivedPtr);
	REQUIRE(DerivedPtr.Get() == nullptr);
	REQUIRE(DerivedPtr.GetStrongRefCount() == -1);
	REQUIRE(DerivedPtr.GetWeakRefCount() == -1);

	REQUIRE(BasePtr.GetStrongRefCount() == 1);
	REQUIRE(BasePtr.GetWeakRefCount() == 0);
	REQUIRE(BasePtr.IsValid());

	REQUIRE(BasePtr.Get() == RawDerivedPtr);
}

TEST_CASE("Generalized Constructor Taking a TUniquePtr.")
{
	Derived* RawDerivedPtr = new Derived;
	TUniquePtr<Derived> DerivedPtr(RawDerivedPtr);
	REQUIRE(DerivedPtr.IsValid());

	TSharedPtr<Base> BasePtr = MoveTemp(DerivedPtr);

	REQUIRE(!DerivedPtr.IsValid());
	
	REQUIRE(BasePtr.IsValid());
	REQUIRE(BasePtr.Get() == RawDerivedPtr);
	REQUIRE(BasePtr.GetStrongRefCount() == 1);
	REQUIRE(BasePtr.GetWeakRefCount() == 0);

}

TEST_CASE("Generalized Copy Constructor")
{
	TSharedPtr<Derived> DerivedPtr = TSharedPtr<Derived>(new Derived);
	REQUIRE(DerivedPtr.GetStrongRefCount() == 1);
	REQUIRE(DerivedPtr.GetWeakRefCount() == 0);
	REQUIRE(DerivedPtr.IsValid());

	TSharedPtr<Base> BasePtr = DerivedPtr;
	REQUIRE(BasePtr.GetStrongRefCount() == 2);
	REQUIRE(BasePtr.GetWeakRefCount() == 0);
	REQUIRE(BasePtr.IsValid());
}

TEST_CASE("Copy Assignment Operator")
{
	SECTION("Default Copy Assignment")
	{
		TSharedPtr<int32> IntPtr;
		IntPtr = TSharedPtr<int32>(new int32(5));
		REQUIRE(IntPtr.GetStrongRefCount() == 1);
		REQUIRE(IntPtr.GetWeakRefCount() == 0);
		REQUIRE(IntPtr.IsValid());
		REQUIRE(*IntPtr == 5);

		TSharedPtr<int32> OtherIntPtr;
		OtherIntPtr = IntPtr;
		REQUIRE(OtherIntPtr.GetStrongRefCount() == 2);
		REQUIRE(OtherIntPtr.GetWeakRefCount() == 0);
		REQUIRE(OtherIntPtr.IsValid());
		REQUIRE(*OtherIntPtr == 5);

		TSharedPtr<int32> LastIntPtr = TSharedPtr<int32>(new int32(1));
		IntPtr = LastIntPtr;
		REQUIRE(IntPtr.GetStrongRefCount() == 2);
		REQUIRE(IntPtr.GetWeakRefCount() == 0);
		REQUIRE(IntPtr.IsValid());
		REQUIRE(*IntPtr == 1);

		REQUIRE(OtherIntPtr.GetStrongRefCount() == 1);
		REQUIRE(OtherIntPtr.GetWeakRefCount() == 0);
		REQUIRE(OtherIntPtr.IsValid());
		REQUIRE(*OtherIntPtr == 5);
	}

	SECTION("Default Copy Assignment: assign a default constructed TSharedPtr")
	{
		TSharedPtr<int32> IntPtr = TSharedPtr<int32>(new int32(5));
		REQUIRE(IntPtr.GetStrongRefCount() == 1);
		REQUIRE(IntPtr.GetWeakRefCount() == 0);
		REQUIRE(IntPtr.IsValid());
		REQUIRE(*IntPtr == 5);

		TSharedPtr<int32> OtherIntPtr;
		IntPtr = OtherIntPtr;
		REQUIRE(IntPtr.Get() == nullptr);
	}

	SECTION("Generalized Copy Assignment")
	{
		TSharedPtr<Derived> DerivedPtr;
		DerivedPtr = TSharedPtr<Derived>(new Derived);
		REQUIRE(DerivedPtr.GetStrongRefCount() == 1);
		REQUIRE(DerivedPtr.GetWeakRefCount() == 0);
		REQUIRE(DerivedPtr.IsValid());

		TSharedPtr<Base> BasePtr;
		BasePtr = DerivedPtr;
		REQUIRE(DerivedPtr.GetStrongRefCount() == 2);
		REQUIRE(DerivedPtr.GetWeakRefCount() == 0);
		REQUIRE(DerivedPtr.IsValid());
	}

	SECTION("Copy Assignment Operator: passed a nullptr")
	{
		TSharedPtr<int32> IntPtr = TSharedPtr<int32>(new int32(5));
		REQUIRE(IntPtr.GetStrongRefCount() == 1);
		REQUIRE(IntPtr.GetWeakRefCount() == 0);
		REQUIRE(IntPtr.IsValid());
		REQUIRE(*IntPtr == 5);

		IntPtr = nullptr;
		REQUIRE(IntPtr.Get() == nullptr);
	}

	SECTION("Copy Assignment Operator: self-assignment")
	{
		TSharedPtr<int32> IntPtr1 = TSharedPtr<int32>(new int32(5));
		REQUIRE(IntPtr1.GetStrongRefCount() == 1);
		REQUIRE(IntPtr1.GetWeakRefCount() == 0);
		REQUIRE(IntPtr1.IsValid());
		REQUIRE(*IntPtr1 == 5);

		TSharedPtr<int32> IntPtr2 = IntPtr1;
		REQUIRE(IntPtr2.GetStrongRefCount() == 2);
		REQUIRE(IntPtr2.GetWeakRefCount() == 0);
		REQUIRE(IntPtr2.IsValid());
		REQUIRE(*IntPtr2 == 5);

		TSharedPtr<int32> IntPtr3 = IntPtr1;
		IntPtr3 = IntPtr2;
		REQUIRE(IntPtr3.GetStrongRefCount() == 3);
		REQUIRE(IntPtr3.GetWeakRefCount() == 0);
		REQUIRE(IntPtr3.IsValid());
		REQUIRE(*IntPtr3 == 5);
	}
}

TEST_CASE("Move Assignment Operator")
{
	SECTION("Default Move Assignment")
	{
		TSharedPtr<int32> IntPtr = TSharedPtr<int32>(new int32(5));
		REQUIRE(IntPtr.GetStrongRefCount() == 1);
		REQUIRE(IntPtr.GetWeakRefCount() == 0);
		REQUIRE(IntPtr.IsValid());
		REQUIRE(*IntPtr == 5);

		TSharedPtr<int32> OtherIntPtr;
		OtherIntPtr = MoveTemp(IntPtr);
		REQUIRE(IntPtr.Get() == nullptr);
		REQUIRE(IntPtr.GetStrongRefCount() == -1);
		REQUIRE(IntPtr.GetWeakRefCount() == -1);

		REQUIRE(OtherIntPtr.GetStrongRefCount() == 1);
		REQUIRE(OtherIntPtr.GetWeakRefCount() == 0);
		REQUIRE(OtherIntPtr.IsValid());
		REQUIRE(*OtherIntPtr == 5);
	}

	SECTION("Default Move Assignment: Self-Assignment")
	{
		TSharedPtr<int32> IntPtr1 = TSharedPtr<int32>(new int32(5));
		REQUIRE(IntPtr1.GetStrongRefCount() == 1);
		REQUIRE(IntPtr1.GetWeakRefCount() == 0);
		REQUIRE(IntPtr1.IsValid());
		REQUIRE(*IntPtr1 == 5);

		TSharedPtr<int32> IntPtr2 = IntPtr1;
		REQUIRE(IntPtr2.GetStrongRefCount() == 2);
		REQUIRE(IntPtr2.GetWeakRefCount() == 0);
		REQUIRE(IntPtr2.IsValid());
		REQUIRE(*IntPtr2 == 5);

		TSharedPtr<int32> IntPtr3 = IntPtr1; 
		IntPtr3 = MoveTemp(IntPtr2);
		REQUIRE(IntPtr3.GetStrongRefCount() == 2);
		REQUIRE(IntPtr3.GetWeakRefCount() == 0);
		REQUIRE(IntPtr3.IsValid());
		REQUIRE(*IntPtr3 == 5);
	}

	SECTION("Generalized Move Assignement")
	{
		TSharedPtr<Derived> DerivedPtr = TSharedPtr<Derived>(new Derived);
		REQUIRE(DerivedPtr.GetStrongRefCount() == 1);
		REQUIRE(DerivedPtr.GetWeakRefCount() == 0);
		REQUIRE(DerivedPtr.IsValid());

		TSharedPtr<Base> BasePtr;
		BasePtr = MoveTemp(DerivedPtr);
		REQUIRE(DerivedPtr.Get() == nullptr);
		REQUIRE(DerivedPtr.GetStrongRefCount() == -1);
		REQUIRE(DerivedPtr.GetWeakRefCount() == -1);

		REQUIRE(BasePtr.GetStrongRefCount() == 1);
		REQUIRE(BasePtr.GetWeakRefCount() == 0);
		REQUIRE(BasePtr.IsValid());
	}

	SECTION("Generalized Move Assignment: Self-Assignment")
	{
		TSharedPtr<Derived> DerivedPtr1 = TSharedPtr<Derived>(new Derived);
		REQUIRE(DerivedPtr1.GetStrongRefCount() == 1);
		REQUIRE(DerivedPtr1.GetWeakRefCount() == 0);
		REQUIRE(DerivedPtr1.IsValid());

		TSharedPtr<Derived> DerivedPtr2 = DerivedPtr1;
		REQUIRE(DerivedPtr2.GetStrongRefCount() == 2);
		REQUIRE(DerivedPtr2.GetWeakRefCount() == 0);
		REQUIRE(DerivedPtr2.IsValid());

		TSharedPtr<Base> BasePtr = DerivedPtr1;
		BasePtr = MoveTemp(DerivedPtr2);
		REQUIRE(BasePtr.GetStrongRefCount() == 2);
		REQUIRE(BasePtr.GetWeakRefCount() == 0);
		REQUIRE(BasePtr.IsValid());
	}
}

TEST_CASE("Destructor")
{
	TSharedPtr<int32> IntPtr = TSharedPtr<int32>(new int32(5));
	REQUIRE(IntPtr.GetStrongRefCount() == 1);
	REQUIRE(IntPtr.GetWeakRefCount() == 0);
	REQUIRE(IntPtr.IsValid());
	REQUIRE(*IntPtr == 5);

	{
		TSharedPtr<int32> OtherIntPtr = IntPtr;
		REQUIRE(OtherIntPtr.GetStrongRefCount() == 2);
		REQUIRE(OtherIntPtr.GetWeakRefCount() == 0);
		REQUIRE(OtherIntPtr.IsValid());
		REQUIRE(*OtherIntPtr == 5);
	}

	REQUIRE(IntPtr.GetStrongRefCount() == 1);
	REQUIRE(IntPtr.GetWeakRefCount() == 0);
	REQUIRE(IntPtr.IsValid());
	REQUIRE(*IntPtr == 5);
}

TEST_CASE("Overloaded operators")
{
	SECTION("TSharedPtr::operator*")
	{
		TSharedPtr<int32> IntPtr = TSharedPtr<int32>(new int32(5));
		REQUIRE(*IntPtr == 5);

		TSharedPtr<Base> MyStructPtr = TSharedPtr<Base>(new Base(5));
		REQUIRE((*MyStructPtr).x == 5);
	}

	SECTION("TSharedPtr::operator->")
	{
		TSharedPtr<Base> MyStructPtr = TSharedPtr<Base>(new Base(5));
		REQUIRE(MyStructPtr->x == 5);
	}

	SECTION("TSharedPtr::operator bool")
	{
		TSharedPtr<int32> IntPtr = TSharedPtr<int32>(new int32(5));
		REQUIRE(IntPtr);
	}

	SECTION("TSharedPtr::operator!")
	{
		TSharedPtr<int32> IntPtr;
		REQUIRE(!IntPtr);
	}

	SECTION("TSharedPtr::operator==")
	{
		TSharedPtr<int32> IntPtr;
		TSharedPtr<int32> OtherIntPtr;
		REQUIRE(IntPtr == OtherIntPtr);

		int32* RawIntPtr = new int32(5);
		IntPtr = TSharedPtr<int32>(RawIntPtr);
		OtherIntPtr = IntPtr;

		REQUIRE(IntPtr == OtherIntPtr);
		REQUIRE((IntPtr == nullptr) == false);
		REQUIRE((nullptr == OtherIntPtr) == false);
	}

	SECTION("TSharedPtr::operator!=")
	{
		TSharedPtr<int32> IntPtr;
		TSharedPtr<int32> OtherIntPtr;
		REQUIRE((IntPtr != OtherIntPtr) == false);

		IntPtr = TSharedPtr<int32>(new int32(5));
		OtherIntPtr = TSharedPtr<int32>(new int32(5));
		REQUIRE(IntPtr != OtherIntPtr);
		REQUIRE(IntPtr != nullptr);
		REQUIRE(nullptr != OtherIntPtr);
	}
}

TEST_CASE("Reset")
{
	TSharedPtr<int32> IntPtr = TSharedPtr<int32>(new int32(5));
	REQUIRE(IntPtr.GetStrongRefCount() == 1);
	REQUIRE(IntPtr.GetWeakRefCount() == 0);
	REQUIRE(IntPtr.IsValid());
	REQUIRE(*IntPtr == 5);

	IntPtr.Reset();
	REQUIRE(IntPtr.Get() == nullptr);
}

TEST_CASE("TSharedPtr::Get")
{
	int32* RawIntPtr = new int32(5);
	TSharedPtr<int32> ConstIntPtr = TSharedPtr<int32>(RawIntPtr);
	REQUIRE(ConstIntPtr.Get() == RawIntPtr);
}
