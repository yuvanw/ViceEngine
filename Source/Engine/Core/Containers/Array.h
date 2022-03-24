#pragma once

#include "CoreGlobals.h"
#include "AssertionMacros.h"
#include "Memory/NewDeleteAllocator.h"

// System include for placement new.
#include <new>

using FDefaultArrayAllocator = FNewDeleteAllocator;

/* Dynamically resizable array. */
template<typename ElementType>
class TArray
{
public:
	/**
	 * Default constructor. Does not allocate any memory.
	 * 
	 * @param InAllocator: Allocator use to allocate array memory.
	 */
	explicit TArray(IAllocator& InAllocator = FDefaultArrayAllocator::GetDefaultAllocator());
	
	/**
	 * Constructor that creates an array of requested capacity.
	 * Allocates enough memory for at least InCapacity elements.
	 * Array elements are not default constructed.
	 * 
	 * @param InCapacity: Requested number of elements.
	 * @param InAllocator: Allocator use to allocate array memory.
	 */
	explicit TArray(int32 InCapacity, IAllocator& InAllocator = FDefaultArrayAllocator::GetDefaultAllocator());

	// Destructor.
	~TArray();
	
	/**
	 * Copy constructor. Uses InArray's allocator.
	 * 
	 * @param InArray: Source array to copy from.
	 */
	TArray(const TArray<ElementType>& InArray);
	
	/**
	 * Copy assignment operator. Does not copy InArray's allocator.
	 * Array capacity does not shrink even if source array has a fewer
	 * number of elements than the target array.
	 *
	 * @param InArray: Source array to copy from.
	 */
	TArray<ElementType>& operator=(const TArray<ElementType>& InArray);

	/**
	 * Indexing operator.
	 * 
	 * @param InIndex: Array element index.
	 * @returns: Reference to the element at the request index.
	 */
	ElementType& operator[](int32 InIndex);
	const ElementType& operator[](int32 InIndex) const;

	/**
	 * Adds a copy of InElement to the back of the array.
	 * Can cause array reallocation.
	 * 
	 * @param InElement: Element to add.
	 * @returns: Index of the array that the element was added at.
	 */
	int32 Add(const ElementType& InElement);
	
	/**
	 * Constructs an element at the back of the array using the input arguments.
	 * Can cause array reallocation.
	 *
	 * @param InArgs: Arguments used to constructor the new element.
	 * @returns: Index of the array that the element was constructed in.
	 */
	template<typename... Args>
	int32 Emplace(Args... InArgs);

	/**
	 * Copies all elements of the source array to the back of the target array.
	 * Can cause array reallocation.
	 *
	 * @param InArray: Source array to copy elements from.
	 */
	void Append(const TArray<ElementType>& InArray);

	/**
	 * Removes the element at the specified index from the array.
	 * Remaining elements at indices greater than InIndex are shifted down
	 * to fill the empty slot. Array capacity does not shrink the fit the new size.
	 *
	 * @param InIndex: Index of the element to remove.
	 */
	void RemoveAt(int32 InIndex);

	/**
	 * Removes the first encountered instance of an element.
	 * Remaining elements at indices greater than that of the removed element
	 * are shifted down to fill the empty slot. Array capacity does not 
	 * shrink the fit the new size.
	 *
	 * @param InElement: Element to remove.
	 * @returns: true if an element was removed, false otherwise.
	 */
	bool RemoveFirst(const ElementType& InElement);

	/**
	 * Removes the first element that matches a predicate.
	 * Remaining elements at indices greater than that of the removed element
	 * are shifted down to fill the empty slot. Array capacity does not
	 * shrink the fit the new size.
	 *
	 * @param InPredicate: Predicate that specifies what element to remove.
	 * @returns: true if an element was removed, false otherwise.
	 */
	template<typename PredicateType>
	bool RemoveFirstByPredicate(const PredicateType& InPredicate);
	
	/**
	 * Removes all elements from the array. Array capacity does not shrink.
	 */
	void Empty();

	/**
	 * Checks if the array is empty.
	 * 
	 * @returns: true if the array is empty, false otherwise.
	 */
	bool IsEmpty() const 
	{ 
		return Size == 0; 
	}

	/**
	 * Reserves enough memory for InCapacity elements. Does nothing if 
	 * current array capacity is greater than or equal to InCapacity.
	 *
	 * @param InCapacity: Requested number of elements.
	 */
	void Reserve(int32 InCapacity);

	/**
	 * Finds the first instance of an element.
	 *
	 * @param InElement: The element to find.
	 * @returns: A pointer to the element if found, nullptr otherwise.
	 */
	ElementType* Find(const ElementType& InElement);
	const ElementType* Find(const ElementType& InElement) const;

	/**
	 * Finds the first element that matches a predicate.
	 *
	 * @param InPredicate: Predicate that specifies what element to find.
	 * @returns: A pointer to the element if found, nullptr otherwise.
	 */
	template<typename PredicateType>
	ElementType* FindByPredicate(const PredicateType& InPredicate);
	template<typename PredicateType>
	const ElementType* FindByPredicate(const PredicateType& InPredicate) const;

	/**
	 * Gets the index of the first instance of an element.
	 *
	 * @param InElement: The element to get the index of.
	 * @returns: The index of the element if found, InvalidIndex otherwise.
	 */
	int32 GetIndexOf(const ElementType& InElement) const;

	/**
	 * Gets the index of the first element that matches a predicate.
	 *
	 * @param InPredicate: Predicate that specifies what element to get the index of.
	 * @returns: The index of the element if found, InvalidIndex otherwise.
	 */
	template<typename PredicateType>
	int32 GetIndexOfByPredicate(const PredicateType& InPredicate) const;

	/**
	 * Checks whether the array contains a specified element.
	 *
	 * @param InElement: The target element.
	 * @returns: true if the array contains the element, false otherwise.
	 */
	bool IsContained(const ElementType& InElement) const;

	/**
	 * Checks whether the array contains an element that matches a predicate.
	 *
	 * @param InPredicate: Predicate that specifies the target element.
	 * @returns: true if the array contains an element matching the predicate, false otherwise.
	 */
	template<typename PredicateType>
	bool IsContainedByPredicate(const PredicateType& InPredicate) const;

	// Getters.
	int32 GetSize() const 
	{ 
		return Size; 
	}
	int32 GetCapacity() const 
	{ 
		return Capacity; 
	}
	ElementType* GetData() 
	{ 
		return Data; 
	}
	const ElementType* GetData() const 
	{
		return Data; 
	}
	IAllocator* GetAllocator() 
	{ 
		return Allocator; 
	}
	const IAllocator* GetAllocator() const 
	{
		return Allocator;
	}

	// Setters.
	void SetAllocator(const IAllocator* InAllocator) 
	{ 
		Allocator = InAllocator; 
	}

	// Iterators.
	ElementType* begin() 
	{ 
		return Data; 
	}
	const ElementType* begin() const 
	{ 
		return Data; 
	}
	ElementType* end() 
	{ 
		return Data + Size; 
	}
	const ElementType* end() const 
	{ 
		return Data + Size; 
	}

	/**
	 * Overloaded addition assignment operator that appends the source array
	 * to the back of the target array.
	 *
	 * @param InArray: Source array to append.
	 * @returns: Reference to the target array.
	 * @see Append
	 */
	TArray<ElementType>& operator+=(const TArray<ElementType>& InArray);

	/**
	 * Overloaded addition operator that appends the source array
	 * to the back of the target array.
	 *
	 * @param InArray: Source array to append.
	 * @returns: The source array appended to the back of a copy of the target array.
	 * @see Append
	 */
	TArray<ElementType> operator+(const TArray<ElementType>& InArray) const;

private:
	ElementType* Data;
	// Number of elements stored in the Data array.
	int32 Size;
	// Total size of the Data array.
	int32 Capacity;
	IAllocator* Allocator;

	// Growth factor used to determine the amount of space to reserve during a reallocation.
	static constexpr int32 GrowthFactor = 2;

	// Gets the capacity of the array to use during the next reallocation.
	int32 GetNextCapacity();
	// Grows the array to the requested capacity.
	void Resize(int32 InCapacity);
	// Makes this array a copy of the requested array.
	void CopyArray(const TArray<ElementType>& InArray);
};

template<typename ElementType>
inline TArray<ElementType>::TArray(IAllocator& InAllocator /* = FDefaultArrayAllocator::GetDefaultAllocator() */)
	: Size(0)
	, Capacity(0)
	, Data(nullptr)
	, Allocator(&InAllocator)
{
}

template<typename ElementType>
inline TArray<ElementType>::TArray(int32 InCapacity, IAllocator& InAllocator /* = FDefaultArrayAllocator::GetDefaultAllocator() */)
	: Size(0)
	, Capacity(InCapacity)
	, Allocator(&InAllocator)
{
	Data = (ElementType*)Allocator->Allocate(Capacity * sizeof(ElementType));
	ensure(Data);
}

template<typename ElementType>
inline TArray<ElementType>::TArray(const TArray<ElementType>& InArray)
	: Allocator(const_cast<IAllocator*>(InArray.GetAllocator()))
{
	CopyArray(InArray);
}

template<typename ElementType>
inline TArray<ElementType>::~TArray()
{
	Empty();
	if (Data)
	{
		Allocator->Deallocate(Data);
	}
}

template<typename ElementType>
inline TArray<ElementType>& TArray<ElementType>::operator=(const TArray<ElementType>& InArray)
{
	Empty();

	for (int32 Index = 0; Index < InArray.GetSize(); ++Index)
	{
		Add(InArray[Index]);
	}

	return *this;
}

template<typename ElementType>
inline ElementType& TArray<ElementType>::operator[](int32 InIndex)
{
	ensure(InIndex < Size);
	return Data[InIndex];
}

template<typename ElementType>
inline const ElementType& TArray<ElementType>::operator[](int32 InIndex) const
{
	ensure(InIndex < Size);
	return Data[InIndex];
}

template<typename ElementType>
inline int32 TArray<ElementType>::Add(const ElementType& InElement)
{
	if (Size >= Capacity)
	{
		int32 NextCapacity = GetNextCapacity();
		Resize(NextCapacity);
	}

	new (Data + Size) ElementType(InElement);
	return Size++;
}

template<typename ElementType>
template<typename... Args>
inline int32 TArray<ElementType>::Emplace(Args... InArgs)
{
	if (Size >= Capacity)
	{
		int32 NextCapacity = GetNextCapacity();
		Resize(NextCapacity);
	}

	new (Data + Size) ElementType(InArgs...);
	return Size++;
}

template<typename ElementType>
void TArray<ElementType>::Append(const TArray<ElementType>& InArray)
{
	Reserve(Size + InArray.GetSize());
	for (const ElementType& Element : InArray)
	{
		new (Data + Size) ElementType(Element);
		++Size;
	}
}

template<typename ElementType>
inline void TArray<ElementType>::RemoveAt(int32 InIndex)
{
	ensure(0 <= InIndex && InIndex < Size);

	// Shift all elements to the right of the removed element down one slot to the left.
	int32 SizeMinusOne = Size - 1;
	for (int32 Index = InIndex; Index < SizeMinusOne; ++Index)
	{
		Data[Index] = MoveTempIfPossible(Data[Index + 1]);
	}

	// Destroy what was the last element in the array before the removal. 
	Data[SizeMinusOne].~ElementType();

	--Size;
}

template<typename ElementType>
inline bool TArray<ElementType>::RemoveFirst(const ElementType& InElement)
{
	for (int32 Index = 0; Index < Size; ++Index)
	{
		if (Data[Index] == InElement)
		{
			RemoveAt(Index);
			return true;
		}
	}

	return false;
}

template<typename ElementType>
template<typename PredicateType>
inline bool TArray<ElementType>::RemoveFirstByPredicate(const PredicateType& InPredicate)
{
	for (int32 Index = 0; Index < Size; ++Index)
	{
		// Remove element we've found a match with using the predicate.
		if (InPredicate(Data[Index]))
		{
			RemoveAt(Index);
			return true;
		}
	}

	return false;
}

template<typename ElementType>
inline void TArray<ElementType>::Empty()
{
	for (int32 Index = 0; Index < Size; ++Index)
	{
		Data[Index].~ElementType();
	}

	Size = 0;
}

template<typename ElementType>
void TArray<ElementType>::Reserve(int32 InCapacity)
{
	ensure(InCapacity >= 0);

	// No-op if current capacity is sufficient.
	if ((InCapacity == 0) || (InCapacity <= Capacity))
	{
		return;
	}

	Resize(InCapacity);
}

template<typename ElementType>
ElementType* TArray<ElementType>::Find(const ElementType& InElement)
{
	return const_cast<ElementType*>(
		static_cast<const TArray&>(*this).Find(InElement)
	);
}

template<typename ElementType>
const ElementType* TArray<ElementType>::Find(const ElementType& InElement) const
{
	for (int32 Index = 0; Index < Size; ++Index)
	{
		const ElementType& Element = Data[Index];
		if (Element == InElement)
		{
			return &Element;
		}
	}

	return nullptr;
}

template<typename ElementType>
template<typename PredicateType>
ElementType* TArray<ElementType>::FindByPredicate(const PredicateType& InPredicate)
{
	return const_cast<ElementType*>(
		static_cast<const TArray&>(*this).FindByPredicate(InPredicate)
	);
}

template<typename ElementType>
template<typename PredicateType>
const ElementType* TArray<ElementType>::FindByPredicate(const PredicateType& InPredicate) const
{
	for (int32 Index = 0; Index < Size; ++Index)
	{
		const ElementType& Element = Data[Index];
		if (InPredicate(Element))
		{
			return &Element;
		}
	}

	return nullptr;
}

template<typename ElementType>
int32 TArray<ElementType>::GetIndexOf(const ElementType& InElement) const
{
	for (int32 Index = 0; Index < Size; ++Index)
	{
		if (Data[Index] == InElement)
		{
			return Index;
		}
	}

	return InvalidIndex;
}

template<typename ElementType>
template<typename PredicateType>
int32 TArray<ElementType>::GetIndexOfByPredicate(const PredicateType& InPredicate) const
{
	for (int32 Index = 0; Index < Size; ++Index)
	{
		if (InPredicate(Data[Index]))
		{
			return Index;
		}
	}

	return InvalidIndex;
}

template<typename ElementType>
inline bool TArray<ElementType>::IsContained(const ElementType& InElement) const
{
	for (int32 Index = 0; Index < Size; ++Index)
	{
		if (Data[Index] == InElement)
		{
			return true;
		}
	}
	return false;
}

template<typename ElementType>
template<typename PredicateType>
bool TArray<ElementType>::IsContainedByPredicate(const PredicateType& InPredicate) const
{
	for (int32 Index = 0; Index < Size; ++Index)
	{
		if (InPredicate(Data[Index]))
		{
			return true;
		}
	}
	return false;
}

template<typename ElementType>
TArray<ElementType>& TArray<ElementType>::operator+=(const TArray<ElementType>& InArray)
{
	Append(InArray);
	return *this;
}

template<typename ElementType>
TArray<ElementType> TArray<ElementType>::operator+(const TArray<ElementType>& InArray) const
{
	TArray<ElementType> Result = *this;
	return Result += InArray;
}

template<typename ElementType>
int32 TArray<ElementType>::GetNextCapacity()
{
	return (Capacity > 0) ? (Capacity * GrowthFactor) : GrowthFactor;
}

template<typename ElementType>
void TArray<ElementType>::Resize(int32 InCapacity)
{
	ensure(InCapacity > Capacity);

	ElementType* ResizedData = (ElementType*)Allocator->Allocate(InCapacity * sizeof(ElementType));

	if (Capacity > 0)
	{
		for (int32 Index = 0; Index < Size; ++Index)
		{
			new (ResizedData + Index) ElementType(Data[Index]);
		}

		for (int32 Index = 0; Index < Size; ++Index)
		{
			Data[Index].~ElementType();
		}

		Allocator->Deallocate(Data);
	}

	Data = ResizedData;
	Capacity = InCapacity;
}

template<typename ElementType>
inline void TArray<ElementType>::CopyArray(const TArray<ElementType>& InArray)
{
	Data = (ElementType*)Allocator->Allocate(InArray.GetCapacity() * sizeof(ElementType));
	ensure(Data);

	for (int32 Index = 0; Index < InArray.GetSize(); ++Index)
	{
		new (Data + Index) ElementType(InArray[Index]);
	}

	Size = InArray.GetSize();
	Capacity = InArray.GetCapacity();
}
