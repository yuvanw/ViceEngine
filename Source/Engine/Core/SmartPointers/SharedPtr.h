#pragma once

#include "SmartPointers/ControlBlock.h"
#include "SmartPointers/WeakPtr.h"
#include "SmartPointers/UniquePtr.h"
#include "Templates/TemplateFunctionLibrary.h"
#include "AssertionMacros.h"

template <typename ObjectType> class TWeakPtr;

/**
 * Reference-counted smart pointer used for shared ownership of a resource.
 */
template<typename ObjectType>
class TSharedPtr final
{
public:
	TSharedPtr() = default;
	~TSharedPtr();

	// nullptr constructor.
	TSharedPtr(decltype(nullptr));
	// nullptr assignment.
	TSharedPtr& operator=(decltype(nullptr));

	template<typename OtherType, typename ImplicitConversionCheck = decltype(ImplicitConv<ObjectType*>((OtherType*)nullptr))>
	explicit TSharedPtr(OtherType* InObject);

	// Move construct from TUniquePtr convert from exclusive ownership to shared ownership of a resource.
	template<typename OtherType, typename ImplicitConversionCheck = decltype(ImplicitConv<ObjectType*>((OtherType*)nullptr))>
	TSharedPtr(TUniquePtr<OtherType>&& InUniquePtr);
	
	// Copy constructors.
	TSharedPtr(const TSharedPtr<ObjectType>& InSharedPtr);
	template<typename OtherType, typename ImplicitConversionCheck = decltype(ImplicitConv<ObjectType*>((OtherType*)nullptr))>
	TSharedPtr(const TSharedPtr<OtherType>& InSharedPtr);

	// Move constructors.
	TSharedPtr(TSharedPtr&& InOther);
	template<typename OtherType, typename ImplicitConversionCheck = decltype(ImplicitConv<ObjectType*>((OtherType*)nullptr))>
	TSharedPtr(TSharedPtr<OtherType>&& InOther);
	
	// Copy assignment operators.
	TSharedPtr& operator=(const TSharedPtr& InOther);
	template<typename OtherType, typename ImplicitConversionCheck = decltype(ImplicitConv<ObjectType*>((OtherType*)nullptr))>
	TSharedPtr& operator=(const TSharedPtr<OtherType>& InOther);

	// Move assignment operators.
	TSharedPtr& operator=(TSharedPtr&& InOther);
	template<typename OtherType, typename ImplicitConversionCheck = decltype(ImplicitConv<ObjectType*>((OtherType*)nullptr))>
	TSharedPtr& operator=(TSharedPtr<OtherType>&& InOther);
	
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
	void Reset();

	int32 GetStrongRefCount() const
	{
		// Return -1 if the TSharedPointer doesn't hold a resource.
		return ControlBlock != nullptr ? ControlBlock->GetStrongRefCount() : -1;
	}

	int32 GetWeakRefCount() const
	{
		return ControlBlock != nullptr ? ControlBlock->GetWeakRefCount() : -1;
	}

private:
	explicit TSharedPtr(const TWeakPtr<ObjectType>& InWeakPtr);
	void ReleaseStrongOwnership();

	ObjectType* Object = nullptr;
	FControlBlock* ControlBlock = nullptr;

	// Gives us access to the internals of TSharedPtr of other types.
	template<typename OtherType>
	friend class TSharedPtr;

	// Allow TWeakPtr access to TSharedPtr internals.
	template<typename OtherType>
	friend class TWeakPtr;
};

// Factory function for creating TSharedPtrs.
template<typename ObjectType, typename... ArgTypes>
TSharedPtr<ObjectType> MakeShared(ArgTypes&&... InArgs)
{
	return TSharedPtr<ObjectType>(new ObjectType(Forward<ArgTypes>(InArgs)...));
}

template<typename ObjectType>
void TSharedPtr<ObjectType>::ReleaseStrongOwnership()
{
	if (ControlBlock)
	{
		ControlBlock->DecrementStrongRef();
		if (ControlBlock->GetStrongRefCount() == 0 && Object)
		{
			delete Object;
			Object = nullptr;

			if (ControlBlock->GetWeakRefCount() == 0)
			{
				delete ControlBlock;
				ControlBlock = nullptr;
			}
		}
	}
}

template<typename ObjectType>
TSharedPtr<ObjectType>::~TSharedPtr()
{
	ReleaseStrongOwnership();
}

template<typename ObjectType>
template<typename OtherType, typename ImplicitConversionCheck>
TSharedPtr<ObjectType>::TSharedPtr(OtherType* InObject)
	: Object(InObject)
	, ControlBlock(FControlBlock::Create()) 
{
	ControlBlock->IncrementStrongRef();
}

template<typename ObjectType>
template<typename OtherType, typename ImplicitConversionCheck>
TSharedPtr<ObjectType>::TSharedPtr(TUniquePtr<OtherType>&& InUniquePtr)
	: Object(InUniquePtr.Get())
	, ControlBlock(FControlBlock::Create())
{
	ControlBlock->IncrementStrongRef();
	InUniquePtr.Object = nullptr;
}

template<typename ObjectType>
TSharedPtr<ObjectType>::TSharedPtr(decltype(nullptr))
{
	// If we're passed a null pointer, the data members are initialized to nullptr.
}

template<typename ObjectType>
TSharedPtr<ObjectType>::TSharedPtr(const TSharedPtr<ObjectType>& InSharedPtr)
	: Object(InSharedPtr.Object)
	, ControlBlock(InSharedPtr.ControlBlock)
{
	if (ControlBlock)
	{
		ControlBlock->IncrementStrongRef();
	}
}

template<typename ObjectType>
template<typename OtherType, typename ImplicitConversionCheck>
TSharedPtr<ObjectType>::TSharedPtr(const TSharedPtr<OtherType>& InSharedPtr)
	: Object(InSharedPtr.Object)
	, ControlBlock(InSharedPtr.ControlBlock)
{
	if (ControlBlock)
	{
		ControlBlock->IncrementStrongRef();
	}
}

template<typename ObjectType>
TSharedPtr<ObjectType>::TSharedPtr(TSharedPtr<ObjectType>&& InOther)
	: Object(InOther.Object)
	, ControlBlock(InOther.ControlBlock)
{
	InOther.Object = nullptr;
	InOther.ControlBlock = nullptr;
}

template<typename ObjectType>
template<typename OtherType, typename ImplicitConversionCheck>
TSharedPtr<ObjectType>::TSharedPtr(TSharedPtr<OtherType>&& InOther)
	: Object(InOther.Object)
	, ControlBlock(InOther.ControlBlock)
{
	InOther.Object = nullptr;
	InOther.ControlBlock = nullptr;
}

template<typename ObjectType>
TSharedPtr<ObjectType>::TSharedPtr(const TWeakPtr<ObjectType>& InWeakPtr)
	: Object(InWeakPtr.Object)
	, ControlBlock(InWeakPtr.ControlBlock)
{
	if (ControlBlock)
	{
		ControlBlock->IncrementStrongRef();
	}
}

template<typename ObjectType>
TSharedPtr<ObjectType>& TSharedPtr<ObjectType>::operator=(const TSharedPtr<ObjectType>& InOther)
{
	if (*this == InOther)
	{
		// For self-assignment, we're already pointing to the same object and control block,
		// and the strong reference count stays the same, so we can early return.
		return *this;
	}

	// Pointing to different objects. We can relinquish for ownership for this object.
	ReleaseStrongOwnership();
	Object = InOther.Object;
	ControlBlock = InOther.ControlBlock;

	if (ControlBlock)
	{
		ControlBlock->IncrementStrongRef();
	}

	return *this;
}

template<typename ObjectType>
template<typename OtherType, typename ImplicitConversionCheck>
TSharedPtr<ObjectType>& TSharedPtr<ObjectType>::operator=(const TSharedPtr<OtherType>& InOther)
{
	if (*this == InOther)
	{
		// For self-assignment, we're already pointing to the same object and control block,
		// and the strong reference count stays the same, so we can early return.
		return *this;
	}

	// Pointing to different objects. We can relinquish for ownership for this object.
	ReleaseStrongOwnership();
	Object = InOther.Object;
	ControlBlock = InOther.ControlBlock;

	if (ControlBlock)
	{
		ControlBlock->IncrementStrongRef();
	}

	return *this;
}

template<typename ObjectType>
TSharedPtr<ObjectType>& TSharedPtr<ObjectType>::operator=(TSharedPtr<ObjectType>&& InOther)
{
	if (IsValid())
	{
		ReleaseStrongOwnership();
	}

	Object = InOther.Object;
	ControlBlock = InOther.ControlBlock;
	InOther.Object = nullptr;
	InOther.ControlBlock = nullptr;
	return *this;
}

template<typename ObjectType>
template<typename OtherType, typename ImplicitConversionCheck>
TSharedPtr<ObjectType>& TSharedPtr<ObjectType>::operator=(TSharedPtr<OtherType>&& InOther)
{
	if (IsValid())
	{
		ReleaseStrongOwnership();
	}

	Object = InOther.Object;
	ControlBlock = InOther.ControlBlock;
	InOther.Object = nullptr;
	InOther.ControlBlock = nullptr;
	return *this;
}

template<typename ObjectType>
TSharedPtr<ObjectType>& TSharedPtr<ObjectType>::operator=(decltype(nullptr))
{
	Reset();
	return *this;
}

template<typename ObjectType>
ObjectType& TSharedPtr<ObjectType>::operator*() const
{
	return *Object;
}

template<typename ObjectType>
ObjectType* TSharedPtr<ObjectType>::operator->() const
{
	return Object;
}

template<typename ObjectType>
ObjectType* TSharedPtr<ObjectType>::Get() const
{
	return Object;
}

template<typename ObjectType>
TSharedPtr<ObjectType>::operator bool() const
{
	return Object != nullptr;
}

template<typename ObjectType>
bool TSharedPtr<ObjectType>::operator!() const
{
	return Object == nullptr;
}

template<typename ObjectTypeA, typename ObjectTypeB>
bool operator==(const TSharedPtr<ObjectTypeA>& InSharedPtrA, const TSharedPtr<ObjectTypeB>& InSharedPtrB)
{
	return InSharedPtrA.Get() == InSharedPtrB.Get();
}

template<typename ObjectTypeA>
bool operator==(const TSharedPtr<ObjectTypeA>& InSharedPtrA, decltype(nullptr))
{
	return !InSharedPtrA.IsValid();
}

template<typename ObjectTypeB>
bool operator==(decltype(nullptr), const TSharedPtr<ObjectTypeB>& InSharedPtrB)
{
	return !InSharedPtrB.IsValid();
}

template<typename ObjectTypeA, typename ObjectTypeB>
bool operator!=(const TSharedPtr<ObjectTypeA>& InSharedPtrA, const TSharedPtr<ObjectTypeB>& InSharedPtrB)
{
	return InSharedPtrA.Get() != InSharedPtrB.Get();
}

template<typename ObjectTypeA>
bool operator!=(const TSharedPtr<ObjectTypeA>& InSharedPtrA, decltype(nullptr))
{
	return InSharedPtrA.IsValid();
}

template<typename ObjectTypeB>
bool operator!=(decltype(nullptr), const TSharedPtr<ObjectTypeB>& InSharedPtrB)
{
	return InSharedPtrB.IsValid();
}

template<typename ObjectType>
bool TSharedPtr<ObjectType>::IsValid() const
{
	// We don't check that Object != nullptr here since the Object
	// could be pointing to garbage once it's deleted. Instead, we
	// check that the strong ref counts exist.
	return GetStrongRefCount() > 0;
}

template<typename ObjectType>
void TSharedPtr<ObjectType>::Reset()
{
	ReleaseStrongOwnership();
	Object = nullptr;
	ControlBlock = nullptr;
}
