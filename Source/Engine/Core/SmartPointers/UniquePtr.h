#pragma once

#include "Templates/TemplateFunctionLibrary.h"

/**
 * Smart pointer used for exclusive ownership of a resource.
 */
template <typename ObjectType>
class TUniquePtr final
{
public:
	TUniquePtr() = default;
	~TUniquePtr();

	// Non-copyable to ensure exclusive ownership.
	TUniquePtr(const TUniquePtr<ObjectType>&InUniquePtr) = delete;
	TUniquePtr& operator=(const TUniquePtr& InOther) = delete;

	// nullptr constructor.
	TUniquePtr(decltype(nullptr));
	// nullptr assignment.
	TUniquePtr& operator=(decltype(nullptr));

	// Construct from a raw pointer.
	template<typename OtherType, typename ImplicitConversionCheck = decltype(ImplicitConv<ObjectType*>((OtherType*)nullptr))>
	explicit TUniquePtr(OtherType* InObject);

	// Move constructors.
	TUniquePtr(TUniquePtr && InOther);
	template<typename OtherType, typename ImplicitConversionCheck = decltype(ImplicitConv<ObjectType*>((OtherType*)nullptr))>
	TUniquePtr(TUniquePtr<OtherType> && InOther);

	// Move assignment operators.
	TUniquePtr& operator=(TUniquePtr && InOther);
	template<typename OtherType, typename ImplicitConversionCheck = decltype(ImplicitConv<ObjectType*>((OtherType*)nullptr))>
	TUniquePtr& operator=(TUniquePtr<OtherType> && InOther);

	// Overloaded operators to emulate pointer behavior.
	ObjectType& operator*() const;
	ObjectType* operator->() const;
	// Overloaded bool operator to perform if checks.
	explicit operator bool() const;
	bool operator!() const;

	// Return type not declared const because clients should
	// declare ObjectType to be a const type if they want const behavior.
	ObjectType* Get() const;
	bool IsValid() const;
	void Reset(ObjectType* InObject = nullptr);

private:
	ObjectType* Object = nullptr;

	// Gives us access to the internals of TUniquePtr of other types.
	template <typename OtherType>
	friend class TUniquePtr;

	// Allow TSharedPtr access to TUniquePtr internals.
	template <typename OtherType>
	friend class TSharedPtr;
};

// Factory function for creating TUniquePtr.
template<typename ObjectType, typename... ArgTypes>
TUniquePtr<ObjectType> MakeUnique(ArgTypes&&... InArgs)
{
	return TUniquePtr<ObjectType>(new ObjectType(Forward<ArgTypes>(InArgs)...));
}

template <typename ObjectType>
TUniquePtr<ObjectType>::~TUniquePtr()
{
	Reset();
}

template <typename ObjectType>
TUniquePtr<ObjectType>::TUniquePtr(decltype(nullptr))
{
	// Object is set to nullptr via in-class member initialization.
}

template <typename ObjectType>
TUniquePtr<ObjectType>& TUniquePtr<ObjectType>::operator=(decltype(nullptr))
{
	Reset();
	return *this;
}

template <typename ObjectType>
template <typename OtherType, typename ImplicitConversionCheck>
TUniquePtr<ObjectType>::TUniquePtr(OtherType* InObject)
	: Object(InObject)
{
}

template <typename ObjectType>
TUniquePtr<ObjectType>::TUniquePtr(TUniquePtr&& InOther)
	: Object(InOther.Object)
{
	InOther.Object = nullptr;
}

template <typename ObjectType>
template <typename OtherType, typename ImplicitConversionCheck>
TUniquePtr<ObjectType>::TUniquePtr(TUniquePtr<OtherType>&& InOther)
	: Object(InOther.Object)
{
	InOther.Object = nullptr;
}

template <typename ObjectType>
TUniquePtr<ObjectType>& TUniquePtr<ObjectType>::operator=(TUniquePtr<ObjectType>&& InOther)
{
	Reset(InOther.Object);
	InOther.Object = nullptr;
	return *this;
}

template <typename ObjectType>
template <typename OtherType, typename ImplicitConversionCheck>
TUniquePtr<ObjectType>& TUniquePtr<ObjectType>::operator=(TUniquePtr<OtherType>&& InOther)
{
	Reset(InOther.Object);
	InOther.Object = nullptr;
	return *this;
}

template <typename ObjectType>
ObjectType& TUniquePtr<ObjectType>::operator*() const
{
	return *Object;
}

template <typename ObjectType>
ObjectType* TUniquePtr<ObjectType>::operator->() const
{
	return Object;
}

template <typename ObjectType>
TUniquePtr<ObjectType>::operator bool() const
{
	return IsValid();
}

template <typename ObjectType>
bool TUniquePtr<ObjectType>::operator!() const
{
	return !IsValid();
}

template <typename ObjectType>
ObjectType* TUniquePtr<ObjectType>::Get() const
{
	return Object;
}

template <typename ObjectType>
bool TUniquePtr<ObjectType>::IsValid() const
{
	return Object != nullptr;
}

template <typename ObjectType>
void TUniquePtr<ObjectType>::Reset(ObjectType* InObject /* = nullptr */)
{
	if (Object != InObject)
	{
		ObjectType* OldObject = Object;
		Object = InObject;
		if (OldObject)
		{
			delete OldObject;
		}
	}
}

// Equality functions.
template<typename ObjectTypeA, typename ObjectTypeB>
bool operator==(const TUniquePtr<ObjectTypeA>& InUniquePtrA, const TUniquePtr<ObjectTypeB>& InUniquePtrB)
{
	return InUniquePtrA.Get() == InUniquePtrB.Get();
}

template<typename ObjectTypeA>
bool operator==(const TUniquePtr<ObjectTypeA>& InUniquePtrA, decltype(nullptr))
{
	return !InUniquePtrA.IsValid();
}

template<typename ObjectTypeB>
bool operator==(decltype(nullptr), const TUniquePtr<ObjectTypeB>& InUniquePtrB)
{
	return !InUniquePtrB.IsValid();
}

template<typename ObjectTypeA, typename ObjectTypeB>
bool operator!=(const TUniquePtr<ObjectTypeA>& InUniquePtrA, const TUniquePtr<ObjectTypeB>& InUniquePtrB)
{
	return InUniquePtrA.Get() != InUniquePtrB.Get();
}

template<typename ObjectTypeA>
bool operator!=(const TUniquePtr<ObjectTypeA>& InUniquePtrA, decltype(nullptr))
{
	return InUniquePtrA.IsValid();
}

template<typename ObjectTypeB>
bool operator!=(decltype(nullptr), const TUniquePtr<ObjectTypeB>& InUniquePtrB)
{
	return InUniquePtrB.IsValid();
}
