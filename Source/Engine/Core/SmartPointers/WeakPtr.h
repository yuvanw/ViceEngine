#pragma once

#include "CoreGlobals.h"
#include "SmartPointers/ControlBlock.h"
#include "SmartPointers/SharedPtr.h"
#include "Templates/TemplateFunctionLibrary.h"
#include "AssertionMacros.h"

template <typename ObjectType> class TSharedPtr;

/**
 * Reference-counted smart pointer used for temporary ownership of a resource
 * owned by one or more TSharedPtrs. Can also be used to break TSharedPtr reference cycles.
 */
template<typename ObjectType>
class TWeakPtr final
{
public:
	TWeakPtr() = default;
	~TWeakPtr();

	// nullptr constructor.
	TWeakPtr(decltype(nullptr));
	// nullptr assignment.
	TWeakPtr& operator=(decltype(nullptr));

	// Construct from TSharedPtr.
	template<typename OtherType, typename ImplicitConversionCheck = decltype(ImplicitConv<ObjectType*>((OtherType*)nullptr))>
	TWeakPtr(const TSharedPtr<OtherType>& InSharedPtr);
	// Assign from TSharedPtr.
	template<typename OtherType, typename ImplicitConversionCheck = decltype(ImplicitConv<ObjectType*>((OtherType*)nullptr))>
	TWeakPtr& operator=(const TSharedPtr<OtherType>& InSharedPtr);

	// Copy constructors.
	TWeakPtr(const TWeakPtr<ObjectType>& InSharedPtr);
	template<typename OtherType, typename ImplicitConversionCheck = decltype(ImplicitConv<ObjectType*>((OtherType*)nullptr))>
	TWeakPtr(const TWeakPtr<OtherType>& InWeakPtr);

	// Move constructors.
	TWeakPtr(TWeakPtr&& InOther);
	template<typename OtherType, typename ImplicitConversionCheck = decltype(ImplicitConv<ObjectType*>((OtherType*)nullptr))>
	TWeakPtr(TWeakPtr<OtherType>&& InOther);

	// Copy assignment operators.
	TWeakPtr& operator=(const TWeakPtr& InOther);
	template<typename OtherType, typename ImplicitConversionCheck = decltype(ImplicitConv<ObjectType*>((OtherType*)nullptr))>
	TWeakPtr& operator=(const TWeakPtr<OtherType>& InOther);

	// Move assignment operators.
	TWeakPtr& operator=(TWeakPtr&& InOther);
	template<typename OtherType, typename ImplicitConversionCheck = decltype(ImplicitConv<ObjectType*>((OtherType*)nullptr))>
	TWeakPtr& operator=(TWeakPtr<OtherType>&& InOther);

	bool IsValid() const;
	TSharedPtr<ObjectType> Pin() const&;
	
	void Reset();

	int32 GetStrongRefCount() const
	{
		// Return -1 if the TSharedPointer doesn't hold a resource
		return ControlBlock != nullptr ? ControlBlock->GetStrongRefCount() : -1;
	}

	int32 GetWeakRefCount() const
	{
		return ControlBlock != nullptr ? ControlBlock->GetWeakRefCount() : -1;
	}

private:
	void ReleaseWeakOwnership();

	ObjectType* Object = nullptr;
	FControlBlock* ControlBlock = nullptr;

	// Gives us access to the internals of TWeakPtr of other types.
	template<typename OtherType>
	friend class TWeakPtr;

	// Allow TSharedPtr access to TWeakPtr internals.
	template<typename OtherType>
	friend class TSharedPtr;
};

template<typename ObjectType>
template<typename OtherType, typename ImplicitConversionCheck>
TWeakPtr<ObjectType>::TWeakPtr(const TSharedPtr<OtherType>& InSharedPtr)
	: Object(InSharedPtr.Object)
	, ControlBlock(InSharedPtr.ControlBlock)
{
	if (ControlBlock)
	{
		ControlBlock->IncrementWeakRef();
	}
}

template<typename ObjectType>
void TWeakPtr<ObjectType>::ReleaseWeakOwnership()
{
	if (ControlBlock)
	{
		ControlBlock->DecrementWeakRef();
		
		// Checking the weak ref count first so we can short-curcuit in the common case where weak refs still exist.
		if (ControlBlock->GetWeakRefCount() == 0 && ControlBlock->GetStrongRefCount() == 0)
		{
			delete ControlBlock;
			ControlBlock = nullptr;
		}
	}
}

template<typename ObjectType>
TWeakPtr<ObjectType>::~TWeakPtr()
{
	ReleaseWeakOwnership();
}

template<typename ObjectType>
TWeakPtr<ObjectType>::TWeakPtr(decltype(nullptr))
{
	// If we're passed a null pointer, the data members are initialized to nullptr.
}

template<typename ObjectType>
TWeakPtr<ObjectType>::TWeakPtr(const TWeakPtr<ObjectType>& InWeakPtr)
	: Object(InWeakPtr.Object)
	, ControlBlock(InWeakPtr.ControlBlock)
{
	if (ControlBlock)
	{
		ControlBlock->IncrementWeakRef();
	}
}

template<typename ObjectType>
template<typename OtherType, typename ImplicitConversionCheck>
TWeakPtr<ObjectType>::TWeakPtr(const TWeakPtr<OtherType>& InWeakPtr)
	: Object(InWeakPtr.Object)
	, ControlBlock(InWeakPtr.ControlBlock)
{
	if (ControlBlock)
	{
		ControlBlock->IncrementWeakRef();
	}
}

template<typename ObjectType>
TWeakPtr<ObjectType>::TWeakPtr(TWeakPtr<ObjectType>&& InOther)
	: Object(InOther.Object)
	, ControlBlock(InOther.ControlBlock)
{
	InOther.Object = nullptr;
	InOther.ControlBlock = nullptr;
}

template<typename ObjectType>
template<typename OtherType, typename ImplicitConversionCheck>
TWeakPtr<ObjectType>::TWeakPtr(TWeakPtr<OtherType>&& InOther)
	: Object(InOther.Object)
	, ControlBlock(InOther.ControlBlock)
{
	InOther.Object = nullptr;
	InOther.ControlBlock = nullptr;
}

template<typename ObjectType>
TWeakPtr<ObjectType>& TWeakPtr<ObjectType>::operator=(const TWeakPtr<ObjectType>& InOther)
{
	if (*this == InOther)
	{
		// For self-assignment, we're already pointing to the same object and control block,
		// and the strong reference count stays the same, so we can early return.
		return *this;
	}

	// Pointing to different objects. We can relinquish for ownership for this object.
	ReleaseWeakOwnership();
	Object = InOther.Object;
	ControlBlock = InOther.ControlBlock;

	if (ControlBlock)
	{
		ControlBlock->IncrementWeakRef();
	}

	return *this;
}

template<typename ObjectType>
template<typename OtherType, typename ImplicitConversionCheck>
TWeakPtr<ObjectType>& TWeakPtr<ObjectType>::operator=(const TWeakPtr<OtherType>& InOther)
{
	if (*this == InOther)
	{
		// For self-assignment, we're already pointing to the same object and control block,
		// and the strong reference count stays the same, so we can early return.
		return *this;
	}

	// Pointing to different objects. We can relinquish for ownership for this object.
	ReleaseWeakOwnership();
	Object = InOther.Object;
	ControlBlock = InOther.ControlBlock;

	if (ControlBlock)
	{
		ControlBlock->IncrementWeakRef();
	}

	return *this;
}

template<typename ObjectType>
TWeakPtr<ObjectType>& TWeakPtr<ObjectType>::operator=(TWeakPtr<ObjectType>&& InOther)
{
	if (this != &InOther)
	{
		// We need to check that the control block exists before decrementing the weak ref count.
		// ReleaseWeakOwnership does this for us already, so we can call it directly.
		ReleaseWeakOwnership();
		Object = InOther.Object;
		ControlBlock = InOther.ControlBlock;
		InOther.Object = nullptr;
		InOther.ControlBlock = nullptr;
	}

	return *this;
}

template<typename ObjectType>
template<typename OtherType, typename ImplicitConversionCheck>
TWeakPtr<ObjectType>& TWeakPtr<ObjectType>::operator=(TWeakPtr<OtherType>&& InOther)
{
	// We check for self-assignment here, because in order to invoke the generalized
	// move assignment operator, at least two weak pointers need to exist.
	ReleaseWeakOwnership();
	Object = InOther.Object;
	ControlBlock = InOther.ControlBlock;
	InOther.Object = nullptr;
	InOther.ControlBlock = nullptr;

	return *this;
}

template<typename ObjectType>
TWeakPtr<ObjectType>& TWeakPtr<ObjectType>::operator=(decltype(nullptr))
{
	Reset();
	return *this;
}

template<typename ObjectTypeA, typename ObjectTypeB>
bool operator==(const TWeakPtr<ObjectTypeA>& InWeakPtrA, const TWeakPtr<ObjectTypeB>& InWeakPtrB)
{
	return InWeakPtrA.Pin().Get() == InWeakPtrB.Pin().Get();
}

template<typename ObjectTypeA>
bool operator==(const TWeakPtr<ObjectTypeA>& InWeakPtrA, decltype(nullptr))
{
	return !InWeakPtrA.IsValid();
}

template<typename ObjectTypeB>
bool operator==(decltype(nullptr), const TWeakPtr<ObjectTypeB>& InWeakPtrB)
{
	return !InWeakPtrB.IsValid();
}

template<typename ObjectTypeA, typename ObjectTypeB>
bool operator!=(const TWeakPtr<ObjectTypeA>& InWeakPtrA, const TWeakPtr<ObjectTypeB>& InWeakPtrB)
{
	return InWeakPtrA.Pin().Get() != InWeakPtrB.Pin().Get();
}

template<typename ObjectTypeA>
bool operator!=(const TWeakPtr<ObjectTypeA>& InWeakPtrA, decltype(nullptr))
{
	return InWeakPtrA.IsValid();
}

template<typename ObjectTypeB>
bool operator!=(decltype(nullptr), const TWeakPtr<ObjectTypeB>& InWeakPtrB)
{
	return InWeakPtrB.IsValid();
}

template<typename ObjectType>
template<typename OtherType, typename ImplicitConversionCheck>
TWeakPtr<ObjectType>& TWeakPtr<ObjectType>::operator=(const TSharedPtr<OtherType>& InSharedPtr)
{
	ReleaseWeakOwnership();
	Object = InSharedPtr.Object;
	ControlBlock = InSharedPtr.ControlBlock;

	if (ControlBlock)
	{
		ControlBlock->IncrementWeakRef();
	}

	return *this;
}

template<typename ObjectType>
bool TWeakPtr<ObjectType>::IsValid() const
{
	return GetStrongRefCount() > 0;
}

template<typename ObjectType>
TSharedPtr<ObjectType> TWeakPtr<ObjectType>::Pin() const&
{
	return TSharedPtr<ObjectType>(*this);
}

template<typename ObjectType>
void TWeakPtr<ObjectType>::Reset()
{
	ReleaseWeakOwnership();
	Object = nullptr;
	ControlBlock = nullptr;
}
