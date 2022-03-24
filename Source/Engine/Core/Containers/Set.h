#pragma once

#include "CoreGlobals.h"
#include "AssertionMacros.h"
#include "Memory/Alignment.h"
#include "Containers/KeyOperationsPolicyBase.h"
#include "Templates/TypeTraits/CallTraits.h"
#include "Templates/TypeTraits/IsDefaultConstructable.h"
#include "Templates/TypeTraits/IsTriviallyDestructable.h"
#include "Templates/TemplateFunctionLibrary.h"
#include "Memory/MemoryManager.h"
#include "Math/MathUtilities.h"

// System include for placement new.
#include <new>

template<typename ElementType>
struct TDefaultSetKeyOperationsPolicy : public TKeyOperationsPolicyBase<ElementType, ElementType>
{
};

template<typename ElementType>
struct TDefaultSetHashableKeyOperations : TDefaultSetKeyOperationsPolicy<ElementType>
{
	// This line fails to compile if GetTypeHash hasn't been overloaded for ElementType.
	using HashabilityCheck = decltype(GetTypeHash(DeclVal<const ElementType>()));
};

using MetadataType = int8;

// We break with convention here to allow for easier integer comparisons.
namespace EMetadataState
{
	// The following values are from the Abseil implementation and are optimizations.
	// Note that any Metadata byte with a 0 in the MSB means that it's full and contains a valid element in the Data array.
	enum Type : MetadataType
	{
		Empty = -128, // 0b1000000
		Deleted = -2, // 0b11111110
		Sentinel = -1 // 0b11111111
	};

	static bool IsEmpty(MetadataType InMetadata)
	{
		return InMetadata == EMetadataState::Empty;
	}

	static bool IsDeleted(MetadataType InMetadata)
	{
		return InMetadata == EMetadataState::Deleted;
	}

	static bool IsSentinel(MetadataType InMetadata)
	{
		return InMetadata == EMetadataState::Sentinel;
	}

	static bool IsFull(MetadataType InMetadata)
	{
		// All other values of metadata state are negative.
		return InMetadata >= 0;
	}
}

// This isolates the 7 bits that make up the metadata.
static MetadataType GetMetadataFromHash(uint64 InHash)
{
	return InHash & 0x7f;
}

// This isolates 57 bits which will represent the index into the Data array.
// This is what you take the modulus of to determine the position.
static int64 GetIndexFromHash(uint64 InHash)
{
	return InHash >> 7;
}

/**
 * Flat hash set based on the Google Abseil implementation 
 * and the following CppCon Talk: https://youtu.be/ncHmEUmJZf4
 * 
 * ElementType must support a default constructor if it is a user-defined type.
 * Growth policy is just powers of 2 for now. No SSE or intrinsics are used yet.
 * Optimization of groups isn't being done yet either.
 */
template<typename ElementType, typename KeyOperations = TDefaultSetKeyOperationsPolicy<ElementType>>
class TSet
{
	static constexpr int32 MinimumSetSize = 8;
	static constexpr float MaxLoadFactor = 0.8f;

	// Allows us to access private variables of TSets of polymorphic type.
	template <typename OtherElementType, typename OtherKeyOperations>
	friend class TSet;

	using KeyParamType = typename KeyOperations::KeyParamType;
	using ElementParamType = typename KeyOperations::ElementParamType;

public:
	/**
	 * Default constructor. Allocates memory and default constructs 
	 * elements if ElementType is a user-defined type.
	 */
	TSet();

	/**
	 * Constructor that creates a set of requested capacity.
	 * Allocates enough memory for at least InCapacity elements.
	 * 
	 * @param InCapacity: Requested number of elements.
	 */
	TSet(int32 InCapacity);

	// Destructor.
	~TSet();

	/**
	 * Copy constructor. Uses InOther's allocator.
	 *
	 * @param InOther: Source set to copy from.
	 */
	TSet(const TSet<ElementType>& InOther);

	/**
	 * Copy assignment operator. Copies InOther's elements into
	 * memory allocated by InOther's allocator.
	 *
	 * @param InOther: Source set to copy from.
	 */
	TSet<ElementType>& operator=(const TSet<ElementType>& InOther);

	/**
	 * Move constructor.
	 *
	 * @param InOther: Source set to move from.
	 */
	TSet(TSet<ElementType>&& InOther);

	/**
	 * Move assignment operator.
	 *
	 * @param InOther: Source set to move from.
	 */
	TSet<ElementType>& operator=(TSet<ElementType>&& InOther);

	/**
	 * Adds a copy of InElement to the set.
	 *
	 * @param InElement: Element to add.
	 */
	void Add(const ElementType& InElement);

	/**
	 * Moves an element into the set.
	 *
	 * @param InElement: Element to move.
	 */
	void Add(ElementType&& InElement);

	/**
	 * Removes an element from the set.
	 *
	 * @param InElement: Element to remove.
	 * @param bShouldDeleteKey: Whether or not InElement's destructor should be called upon removal.
	 */
	void Remove(KeyParamType InElement, bool bShouldDeleteKey = true);

	/**
	 * Finds an element in the set.
	 *
	 * @param InElement: Element to find.
	 * @returns: A pointer to the element if found, nullptr otherwise.
	 */
	const ElementType* Find(KeyParamType InElement) const;
	ElementType* Find(KeyParamType InElement);

	/**
	 * Finds an element using a hash and a comparable element.
	 * ComparableKeyType is for heterogeneous lookups, so that we can support polymorphism for keys.
	 * 
	 * @param InHash: Hash used for lookup.
	 * @param InElement: Element to compare with.
	 * @returns: A pointer to the element if found, nullptr otherwise.
	 */
	template<typename ComparableKeyType>
	const ElementType* FindByHash(uint64 InHash, const ComparableKeyType& InElement) const;
	template<typename ComparableKeyType>
	ElementType* FindByHash(uint64 InHash, const ComparableKeyType& InElement);

	/**
	 * Finds the index of an element using its key.
	 *
	 * @param InElement: The key of the target element.
	 * @returns: The index of the element if found, InvalidIndex otherwise.
	 */
	int32 FindIndexByKey(KeyParamType InElement) const;

	/**
	 * Finds the index of an element using a hash and a key.
	 * ComparableKeyType is for heterogeneous lookups, so that we can support polymorphism for keys.
	 *
	 * @param InHash: Hash used for lookup.
	 * @param InElement: The key of the target element.
	 * @returns: The index of the element if found, InvalidIndex otherwise.
	 */
	int32 FindIndexByHash(uint64 InHash, KeyParamType InElement) const;
	template<typename ComparableKeyType>
	int32 FindIndexByHash(uint64 InHash, const ComparableKeyType& InElement) const;

	/**
	 * Removes all elements from the set. Set capacity does not shrink.
	 */
	void Empty();

	bool IsKeyContained(KeyParamType InElement) const
	{
		return Find(InElement) != nullptr;
	}
	bool IsHashContained(uint64 InHash) const
	{
		return FindByHash(InElement) != nullptr;
	}
	bool IsEmpty() const
	{
		return Size == 0;
	}

	// Getters.
	int32 GetSize() const
	{
		return Size;
	}
	int32 GetCapacity() const
	{
		return Capacity;
	}
	float GetLoadFactor() const
	{
		return static_cast<float>(Size) / static_cast<float>(Capacity);
	}
	float GetRehashLoadFactor() const
	{
		return MaxLoadFactor;
	}
	IAllocator* GetAllocator()
	{
		return Allocator;
	}

	// Setters.
	void SetAllocator(const IAllocator& InAllocator)
	{
		Allocator = const_cast<IAllocator*>(&InAllocator);
	}

private:
	// A hash set should never be full. This is primarily used for assertions.
	bool IsFull() const;
	void Rehash();

	IAllocator* Allocator = nullptr;
	// The metadata for the data with all of the hash codes.
	// We use raw pointers instead of TArrays to cut down on the memory footprint.
	MetadataType* Metadata = nullptr;
	ElementType* Data = nullptr;
	int32 Size = 0;
	int32 Capacity = 0;
};

template <typename ElementType, typename KeyOperations>
TSet<ElementType, KeyOperations>::TSet()
{
	Capacity = TSet::MinimumSetSize;
	SetAllocator(FMemoryManager::Get().GetArenaAllocator());
	Metadata = (MetadataType*)Allocator->Allocate(Capacity * sizeof(MetadataType));
	Data = (ElementType*)Allocator->Allocate(Capacity * sizeof(ElementType));

	for (int32 Index = 0; Index < Capacity; ++Index)
	{
		Metadata[Index] = EMetadataState::Empty;
	}

	// Default construct every element in the Data array if ElementType is a user-defined type.
	if (TIsDefaultConstructable<ElementType>::Value)
	{
		for (int32 Index = 0; Index < Capacity; ++Index)
		{
			new (Data + Index) ElementType();
		}
	}
}


template <typename ElementType, typename KeyOperations>
TSet<ElementType, KeyOperations>::TSet(int32 InCapacity)
{
	if (InCapacity < TSet::MinimumSetSize)
	{
		Capacity = TSet::MinimumSetSize;
	}
	else
	{
		Capacity = FMath::RoundUpToNearestPowerOfTwo(InCapacity);
	}
	SetAllocator(FMemoryManager::Get().GetArenaAllocator());
	Metadata = (MetadataType*)Allocator->Allocate(Capacity * sizeof(MetadataType));
	Data = (ElementType*)Allocator->Allocate(Capacity * sizeof(ElementType));
}

template <typename ElementType, typename KeyOperations>
TSet<ElementType, KeyOperations>::TSet(const TSet<ElementType>& InOther)
{
	Capacity = InOther.Capacity;
	ensure(FMath::IsPowerOfTwo(Capacity));
	Size = InOther.Size;
	ensure(Size < Capacity);
	Allocator = InOther.Allocator;
	// Allocate the data arrays with the new allocator.
	Metadata = Allocator->Allocate(Capacity * sizeof(MetadataType));
	Data = Allocator->Allocate(Capacity * sizeof(ElementType));
	// Loop through the other set and copy everything over.
	// We have 2 separate loops to keep the cache hot.
	for (int32 Index = 0; Index < Capacity; ++Index)
	{
		Metadata[Index] = InOther.Metadata[Index];
	}
	for (int32 Index = 0; Index < Capacity; ++Index)
	{
		Data[Index] = InOther.Data[Index];
	}
}

template <typename ElementType, typename KeyOperations>
TSet<ElementType, KeyOperations>::TSet(TSet<ElementType>&& InOther)
{
	Allocator = InOther.Allocator;
	Metadata = InOther.Metadata;
	Data = InOther.Data;
	Size = InOther.Size;
	Capacity = InOther.Capacity;

	InOther.Allocator = nullptr;
	InOther.Metadata = nullptr;
	InOther.Data = nullptr;
	InOther.Size = 0;
	InOther.Capacity = nullptr;
}

template <typename ElementType, typename KeyOperations>
TSet<ElementType, KeyOperations>::~TSet()
{
	// Capacity MUST always be a power of 2 due to growth factor.
	ensure(FMath::IsPowerOfTwo(Capacity));
	// We must ALWAYS have an allocator.
	ensure(Allocator);;
	Empty();
	// Clean up the allocated data.
	Allocator->Deallocate(Metadata);
	Allocator->Deallocate(Data);
}

template <typename ElementType, typename KeyOperations>
TSet<ElementType>& TSet<ElementType, KeyOperations>::operator=(const TSet<ElementType>& InOther)
{
	ensure(FMath::IsPowerOfTwo(InOther.Capacity));
	ensure(InOther.Size < InOther.Capacity);
	Empty();
	// Deallocate current data.
	Allocator->Deallocate(Metadata);
	Allocator->Deallocate(Data);
	Capacity = 0;
	Capacity = InOther.Capacity;
	Size = InOther.Size;
	Allocator = InOther.Allocator;
	// Allocate the data arrays with the new allocator.
	Metadata = Allocator->Allocate(Capacity * sizeof(MetadataType));
	Data = Allocator->Allocate(Capacity * sizeof(ElementType));
	// Loop through the other set and copy everything over.
	// We have 2 separate loops to keep the cache hot.
	for (int32 Index = 0; Index < Capacity; ++Index)
	{
		Metadata[Index] = InOther.Metadata[Index];
	}
	for (int32 Index = 0; Index < Capacity; ++Index)
	{
		Data[Index] = InOther.Data[Index];
	}
	return *this;
}

template <typename ElementType, typename KeyOperations>
TSet<ElementType>& TSet<ElementType, KeyOperations>::operator=(TSet<ElementType>&& InOther)
{
	Allocator = InOther.Allocator;
	Metadata = InOther.Metadata;
	Data = InOther.Data;
	Size = InOther.Size;
	Capacity = InOther.Capacity;

	InOther.Allocator = nullptr;
	InOther.Metadata = nullptr;
	InOther.Data = nullptr;
	InOther.Size = 0;
	InOther.Capacity = nullptr;
	return *this;
}

template <typename ElementType, typename KeyOperations>
void TSet<ElementType, KeyOperations>::Add(const ElementType& InElement)
{
	ensure(Capacity >= TSet::MinimumSetSize);
	ensure(FMath::IsPowerOfTwo(Capacity));
	// Rehashing is also expensive so we'll also need to have a look at size.
	if (GetLoadFactor() >= MaxLoadFactor)
	{
		Rehash();
	}
	KeyOperations::KeyParamType Key = KeyOperations::GetKeyFromElement(InElement);
	int64 Hash = KeyOperations::GetHashFromKey(Key);
	// Think of this as GetIndexFromHash(Hash) % static_cast<int64>(Capacity);
	// We're just using bit manipualtion as an optimization.
	int64 Index = GetIndexFromHash(Hash) & (static_cast<uint64>(Capacity) - 1);
	while (true)
	{
		// Check for duplicate keys if we allow them.
		if (!KeyOperations::bAllowDuplicateKeys && EMetadataState::IsFull(Metadata[Index]) && KeyOperations::DoKeysMatch(Data[Index], InElement))
		{
			// Early return if duplicate is found.
			return;
		}
		if (EMetadataState::IsEmpty(Metadata[Index]))
		{
			// Empty slot, we can just pop the key in.
			Metadata[Index] = GetMetadataFromHash(Hash);
			Data[Index] = InElement;
			++Size;
			return;
		}
		// If we reach here, that means there's a collision in the Index hash. So we need to probe to find an empty spot.
		++Index;
		// Think of this as Index %= Capacity.
		// Just an optimization as capacity is always a power of 2.
		Index &= (Capacity - 1);
	}
}

template <typename ElementType, typename KeyOperations>
void TSet<ElementType, KeyOperations>::Add(ElementType&& InElement)
{
	ensure(FMath::IsPowerOfTwo(Capacity));
	// Rehashing is also expensive so we'll also need to have a look at size.
	if (GetLoadFactor() >= MaxLoadFactor)
	{
		Rehash();
	}
	KeyOperations::KeyParamType Key = KeyOperations::GetKeyFromElement(InElement);
	int64 Hash = KeyOperations::GetHashFromKey(Key);
	int64 Index = GetIndexFromHash(Hash) & (static_cast<uint64>(Capacity) - 1);
	while (true)
	{
		// Check for duplicate keys if we allow them.
		if (!KeyOperations::bAllowDuplicateKeys && EMetadataState::IsFull(Metadata[Index])
			&& KeyOperations::DoKeysMatch(KeyOperations::GetKeyFromElement(Data[Index]), KeyOperations::GetKeyFromElement(InElement)))
		{
			// Early return if duplicate is found.
			return;
		}
		if (EMetadataState::IsEmpty(Metadata[Index]))
		{
			// Empty slot, we can just pop the key in.
			Metadata[Index] = GetMetadataFromHash(Hash);
			Data[Index] = MoveTempIfPossible(InElement);
			++Size;
			return;
		}
		// If we reach here, that means there's a collision in the Index hash. So we need to probe to find an empty spot.
		++Index;
		// Think of this as Index %= Capacity.
		// Just an optimization as capacity is always a power of 2.
		Index &= (Capacity - 1);
	}
}

template <typename ElementType, typename KeyOperations>
void TSet<ElementType, KeyOperations>::Remove(KeyParamType InElement, bool bShouldDeleteKey = true)
{
	int32 FoundIndex = FindIndexByKey(InElement);
	if (FoundIndex != InvalidIndex)
	{
		Metadata[FoundIndex] = EMetadataState::Deleted;
		if (bShouldDeleteKey && TIsTriviallyDestructable<ElementType>::Value)
		{
			Data[FoundIndex].~ElementType();
		}
		--Size;
	}
}

template <typename ElementType, typename KeyOperations>
const ElementType* TSet<ElementType, KeyOperations>::Find(KeyParamType InElement) const
{
	int32 FoundIndex = FindIndexByKey(InElement);
	if (FoundIndex != InvalidIndex)
	{
		return &Data[FoundIndex];
	}
	return nullptr;
}

template <typename ElementType, typename KeyOperations>
ElementType* TSet<ElementType, KeyOperations>::Find(KeyParamType InElement)
{
	/**
	 * Call the const version of Find and cast away the constness of the return type.
	 * This way, we avoid code duplication between const and non-const versions of Find.
	 * Note that we shouldn't do the opposite (i.e. call the non-const version of Find
	 * from the const version), because const member functions promise not to modify
	 * the object, and non-const member functions don't make that promise.
	 */
	return const_cast<ElementType*>(
		static_cast<const TSet&>(*this).Find(InElement)
		);
}

template <typename ElementType, typename KeyOperations>
template<typename ComparableKeyType>
const ElementType* TSet<ElementType, KeyOperations>::FindByHash(uint64 InHash, const ComparableKeyType& InElement) const
{
	int32 FoundIndex = FindIndexByHash(InHash, InElement);
	if (FoundIndex != InvalidIndex)
	{
		return &Data[FoundIndex];
	}
	return nullptr;
}

// ComparableKeyType is for heterogeneous lookups, so that we can support polymorphism for keys.
template <typename ElementType, typename KeyOperations>
template<typename ComparableKeyType>
ElementType* TSet<ElementType, KeyOperations>::FindByHash(uint64 InHash, const ComparableKeyType& InElement)
{
	const_cast<ElementType*>(
		static_cast<const TSet&>(*this).FindByHash(InElement)
		);
}

template <typename ElementType, typename KeyOperations>
int32 TSet<ElementType, KeyOperations>::FindIndexByKey(KeyParamType InElement) const
{
	ensure(FMath::IsPowerOfTwo(Capacity));
	uint64 Hash = KeyOperations::GetHashFromKey(InElement);
	return FindIndexByHash(Hash, InElement);
}

template <typename ElementType, typename KeyOperations>
int32 TSet<ElementType, KeyOperations>::FindIndexByHash(uint64 InHash, KeyParamType InElement) const
{
	ensure(FMath::IsPowerOfTwo(Capacity));
	uint64 Index = GetIndexFromHash(InHash) & (static_cast<uint64>(Capacity) - 1);
	while (true)
	{
		// if the hashed index is empty, nothing could possibly be there.
		if (EMetadataState::IsEmpty(Metadata[Index]))
		{
			return InvalidIndex;
		}
		if ((GetMetadataFromHash(InHash) == Metadata[Index]) && KeyOperations::DoKeysMatch(KeyOperations::GetKeyFromElement(Data[Index]), InElement))
		{
			return Index;
		}
		// This can occur if we have the same Metadata hash for multiple elements and causes a collision
		// We will have to probe in this case.
		++Index;
		// Think of this as Index %= Capacity.
		// Optimization as capacity should always be a power of 2.
		Index &= (Capacity - 1);
	}
}

template <typename ElementType, typename KeyOperations>
template<typename ComparableKeyType>
int32 TSet<ElementType, KeyOperations>::FindIndexByHash(uint64 InHash, const ComparableKeyType& InElement) const
{
	ensure(FMath::IsPowerOfTwo(Capacity));
	uint64 Index = GetIndexFromHash(InHash) & (static_cast<uint64>(Capacity) - 1);
	while (true)
	{
		// If the hashed index is empty, nothing could possibly be there.
		if (EMetadataState::IsEmpty(Metadata[Index]))
		{
			return InvalidIndex;
		}
		if ((GetMetadataFromHash(InHash) == Metadata[Index]) && KeyOperations::DoKeysMatch(KeyOperations::GetKeyFromElement(Data[Index]), KeyOperations::GetKeyFromElement(InElement)))
		{
			return Index;
		}
		// This can occur if we have the same Metadata hash for multiple elements and causes a collision
		// We will have to probe in this case.
		++Index;
		// Think of this as Index %= Capacity.
		// Optimization as capacity should always be a power of 2.
		Index &= (Capacity - 1);
	}
}

template <typename ElementType, typename KeyOperations>
void TSet<ElementType, KeyOperations>::Empty()
{
	for (int32 Index = 0; Index < Capacity; ++Index)
	{
		if (EMetadataState::IsFull(Metadata[Index]) || EMetadataState::IsDeleted(Metadata[Index]))
		{
			// Destroy the element.
			if (TIsTriviallyDestructable<ElementType>::Value)
			{
				Data[Index].~ElementType();
			}
			Metadata[Index] = EMetadataState::Empty;
		}
	}
	Size = 0;
}

template <typename ElementType, typename KeyOperations>
void TSet<ElementType, KeyOperations>::Rehash()
{
	MetadataType* OldMetadata = Metadata;
	ElementType* OldData = Data;
	// Capacity should ALWAYS be a power of 2.
	// If this assertion triggers, something is very wrong with the algorithm.
	ensure(FMath::IsPowerOfTwo(Capacity));
	int32 OldCapacity = Capacity;
	Capacity = FMath::RoundUpToNearestPowerOfTwo(++Capacity);

	// Allocate new arrays.
	Metadata = static_cast<MetadataType*>(Allocator->Allocate(sizeof(MetadataType) * Capacity));
	Data = static_cast<ElementType*>(Allocator->Allocate(sizeof(ElementType) * Capacity));
	// Initialize Metadata array.
	for (int32 Index = 0; Index < Capacity; ++Index)
	{
		Metadata[Index] = EMetadataState::Empty;
	}
	// Initialize Data array with default constructed elements if ElementType is a user-defined type.
	if (TIsDefaultConstructable<ElementType>::Value)
	{
		for (int32 Index = 0; Index < Capacity; ++Index)
		{
			// each element in the Data array without using placement new,
			// so that we can get rid of the dependency on the <new> header.
			new (Data + Index) ElementType();
		}
	}
	for (int32 Index = 0; Index < OldCapacity; ++Index)
	{
		if (EMetadataState::IsFull(OldMetadata[Index]))
		{
			uint64 Hash = KeyOperations::GetHashFromKey(KeyOperations::GetKeyFromElement(OldData[Index]));
			// Think of this as GetIndexFromHash() % Capacity.
			// We're just using bit shifting as an optimization since Capacity will always be power of 2.
			int32 NewIndex = GetIndexFromHash(Hash) & (Capacity - 1);

			if (EMetadataState::IsFull(Metadata[NewIndex]))
			{
				while (true)
				{
					// If we reach here, that means there's a collision in the Index hash. So we need to probe to find an empty spot.
					++NewIndex;
					// Think of this as Index %= Capacity.
					// Just an optimization as capacity is always a power of 2.
					NewIndex &= (Capacity - 1);
					// We loop until we find an empty slot, then break.
					if (EMetadataState::IsEmpty(Metadata[NewIndex]))
					{
						break;
					}
				}
			}

			// No need to recompute metadata as it will always be the same.
			Metadata[NewIndex] = OldMetadata[Index];
			// We move so that we don't have to copy the elements and then destroy the old elements.
			Data[NewIndex] = MoveTempIfPossible(OldData[Index]);
		}
		// Elements marked deleted are still alive, we will need to clean that up.
		if (EMetadataState::IsDeleted(OldMetadata[Index]) && TIsTriviallyDestructable<ElementType>::Value)
		{
			OldData[Index].~ElementType();
		}
	}

	// Delete old data.
	Allocator->Deallocate(OldMetadata);
	Allocator->Deallocate(OldData);
}
