#pragma once

#include "CoreGlobals.h"
#include "Math/MathUtilities.h"
#include "Memory/MemoryManager.h"
#include "Hash/Crc64.h"

// System includes (for memcpy, memcmp, and related functionality).
#include <cstring>

/**
 * Returns the length of the string, disregarding the null-terminating character.
 */
template<typename CharType>
static int32 StrLen(const CharType* InPtr)
{
	const CharType* Ptr = InPtr;

	// Both ANSI and Unicode strings are null-terminated.
	while (*Ptr)
	{
		++Ptr;
	}
	
	return (int32)(Ptr - InPtr);
}

template<typename CharType>
class TBasicString
{
public:
	// Constructors.
	TBasicString() = default;
	TBasicString(const CharType* InCharTypeString);

	// Destructor.
	~TBasicString();

	// Copy operations.
	TBasicString(const TBasicString<CharType>& InOther);
	TBasicString& operator=(const TBasicString& InOther);

	// Move operations.
	TBasicString(TBasicString<CharType>&& InOther);
	TBasicString& operator=(TBasicString&& InOther);

	TBasicString& operator=(const CharType* InCharTypeString);

	// Returns the substring from [InStartIndex, InEndIndex).
	TBasicString<CharType> Substring(int32 InStartIndex, int32 InEndIndex) const;

	// InCharacterCount disregards the null-terminating character.
	// Space for an extra character will be reserved to account for the null-terminating character.
	void Reserve(int32 InCharacterCount);
	
	bool IsEmpty() const
	{
		return BasicString.empty();
	}

	// Getters.
	const CharType* GetData() const
	{
		return Data;
	}
	CharType* GetData()
	{
		return Data;
	}
	int32 GetSize() const
	{
		return Size;
	}
	int32 GetCapacity() const
	{
		return Capacity;
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

private:
	CharType* Data = nullptr;
	// Size represents the number of characters in the string, disregarding the null-terminating character.
	int32 Size = 0;
	// Capacity represents the size of the Data array.
	int32 Capacity = 0;
	// If you encountered an Allocator in an invalid state (i.e. nullptr), 
	// you're most likely are using a string that has been moved from.
	IAllocator* Allocator = &FMemoryManager::Get().GetArenaAllocator();
};

template<typename CharType>
TBasicString<CharType>::~TBasicString()
{
	if (Data && Allocator)
	{
		Allocator->Deallocate(Data);
	}
};

template<typename CharType>
TBasicString<CharType>::TBasicString(const CharType * InCharTypeString)
{
	// Construction from null or empty strings is a no-op.
	if (InCharTypeString && *InCharTypeString)
	{
		Size = StrLen(InCharTypeString);
		Capacity = Size + 1;
		Data = static_cast<CharType*>(Allocator->Allocate(Capacity * sizeof(CharType)));
		memcpy(Data, InCharTypeString, Capacity);
	}
}

template<typename CharType>
TBasicString<CharType>::TBasicString(const TBasicString<CharType>&InOther)
{
	if (InOther.Data)
	{
		Data = static_cast<CharType*>(Allocator->Allocate(InOther.Capacity * sizeof(CharType)));
		Size = InOther.Size;
		Capacity = InOther.Capacity;
		memcpy(Data, InOther.Data, Size + 1);
	}
};

template<typename CharType>
TBasicString<CharType>::TBasicString(TBasicString<CharType> && InOther)
	: Data(InOther.Data)
	, Size(InOther.Size)
	, Capacity(InOther.Capacity)
	, Allocator(InOther.Allocator)
{
};

template<typename CharType>
TBasicString<CharType>& TBasicString<CharType>::operator=(const TBasicString<CharType>& InOther)
{
	// Self-assignment is a no-op.
	if (this == &InOther)
	{
		return *this;
	}

	if (!Data)
	{
		// Assigning to an empty string.
		Data = static_cast<CharType*>(Allocator->Allocate(InOther.Capacity * sizeof(CharType)));
		Size = InOther.Size;
		Capacity = InOther.Capacity;
		memcpy(Data, InOther.Data, Size + 1);
	}
	else if (Capacity >= InOther.Capacity)
	{
		// Reuse the Data array if it's large enough to store the input string.
		Size = InOther.Size;
		memcpy(Data, InOther.Data, Size + 1);
	}
	else
	{
		// The Data array isn't large enough to store the input string, so we need to allocate more space.
		Allocator->Deallocate(Data);
		Data = static_cast<CharType*>(Allocator->Allocate(InOther.Capacity * sizeof(CharType)));
		Size = InOther.Size;
		Capacity = InOther.Capacity;
		memcpy(Data, InOther.Data, Size + 1);
	}

	return *this;
};

template<typename CharType>
TBasicString<CharType>& TBasicString<CharType>::operator=(TBasicString<CharType>&& InOther)
{
	// Self-assignment is a no-op.
	if (this == &InOther)
	{
		return *this;
	}

	if (Data)
	{
		Allocator->Deallocate(Data);
	}

	Data = InOther.Data;
	Size = InOther.Size;
	Capacity = InOther.Capacity;
	Allocator = InOther.Allocator;

	InOther.Data = nullptr;
	InOther.Size = 0;
	InOther.Capacity = 0;
	InOther.Allocator = nullptr;

	return *this;
};

template<typename CharType>
TBasicString<CharType>& TBasicString<CharType>::operator=(const CharType* InCharTypeString)
{
	Size = StrLen(InCharTypeString);

	if (!Data)
	{
		// Assigning to an empty string.
		Capacity = Size + 1;
		Data = static_cast<CharType*>(Allocator->Allocate(Capacity * sizeof(CharType)));
		memcpy(Data, InCharTypeString, Capacity);
	}
	else if (Capacity >= Size + 1)
	{
		// Reuse the Data array if it's large enough to store the input string.
		memcpy(Data, InCharTypeString, Size + 1);
	}
	else
	{
		// The Data array isn't large enough to store the input string, so we need to allocate more space.
		Capacity = Size + 1;
		Allocator->Deallocate(Data);
		Data = static_cast<CharType*>(Allocator->Allocate(Capacity * sizeof(CharType)));
		memcpy(Data, InCharTypeString, Capacity);
	}

	return *this;
}

// Returns the substring from [InStartIndex, InEndIndex).
template<typename CharType>
TBasicString<CharType> TBasicString<CharType>::Substring(int32 InStartIndex, int32 InEndIndex) const
{
	if (InStartIndex >= InEndIndex)
	{
		return TBasicString();
	}

	int32 Start = FMath::Clamp(InStartIndex, 0, Size);
	int32 End = FMath::Clamp(InEndIndex, 0, Size);
	int32 CharacterCount = End - Start;

	TBasicString Substring;
	Substring.Size = CharacterCount;
	Substring.Capacity = CharacterCount + 1;
	Substring.Data = static_cast<CharType*>(Allocator->Allocate(Substring.Capacity * sizeof(CharType)));
	memcpy(Substring.Data, this->GetData() + Start, Substring.Size);
	Substring.Data[Substring.Size] = CharType('\0');

	return Substring;
}

// InCharacterCount disregards the null-terminating character.
// Space for an extra character will be reserved to account for the null-terminating character.
template<typename CharType>
void TBasicString<CharType>::Reserve(int32 InCharacterCount)
{
	if (InCharacterCount <= 0)
	{
		return;
	}

	int32 NewCapacity = InCharacterCount + 1;

	if (!Data)
	{
		Data = static_cast<CharType*>(Allocator->Allocate(NewCapacity * sizeof(CharType)));
		Capacity = NewCapacity;
		return;
	}

	// Similar to Unreal's FString, we only reserve more space for the 
	// string when the requested capacity is greater than the current capacity.
	if (NewCapacity > Capacity)
	{
		CharType* OldData = Data;
		Data = static_cast<CharType*>(Allocator->Allocate(NewCapacity * sizeof(CharType)));
		memcpy(Data, OldData, Size + 1);
		Allocator->Deallocate(OldData);
		Capacity = NewCapacity;
	}
}

// Global operators
template<typename CharType>
inline bool operator==(const TBasicString<CharType>& InString1, const TBasicString<CharType>& InString2)
{
	return (InString1.GetSize() == InString2.GetSize()) && (memcmp(InString1.GetData(), InString2.GetData(), InString1.GetSize() * sizeof(CharType)) == 0);
}

template<typename CharType>
inline bool operator==(const CharType* InCharTypeString, const TBasicString<CharType>& InString)
{
	return (StrLen(InCharTypeString) == InString.GetSize()) && (memcmp(InCharTypeString, InString.GetData(), InString.GetSize() * sizeof(CharType)) == 0);
}

template<typename CharType>
inline bool operator==(const TBasicString<CharType>& InString, const CharType* InCharTypeString)
{
	return InCharTypeString == InString;
}

template<typename CharType>
inline bool operator!=(const TBasicString<CharType>& InString1, const TBasicString<CharType>& InString2)
{
	return !(InString1 == InString2);
}

template<typename CharType>
inline bool operator!=(const CharType* InCharTypeString, const TBasicString<CharType>& InString)
{
	return !(InCharTypeString == InString);
}

template<typename CharType>
inline bool operator!=(const TBasicString<CharType>& InString, const CharType* InCharTypeString)
{
	return !(InString == InCharTypeString);
}

// Hashing support for strings.
inline uint64 GetTypeHash(const TBasicString<ANSICHAR>& InBasicString)
{
	int32 SizeBytes = InBasicString.GetSize() * sizeof(ANSICHAR);
	return FCrc64::GetTypeHash(InBasicString.GetData(), SizeBytes);
}

inline uint64 GetTypeHash(const TBasicString<U8CHAR>& InBasicString)
{
	int32 SizeBytes = InBasicString.GetSize() * sizeof(U8CHAR);
	return FCrc64::GetTypeHash(InBasicString.GetData(), SizeBytes);
}

inline uint64 GetTypeHash(const ANSICHAR* InANSIString)
{
	int32 SizeBytes = StrLen(InANSIString) * sizeof(ANSICHAR);
	return FCrc64::GetTypeHash(InANSIString, SizeBytes);
}

// char8_t was only introduced in C++20, so we're using uint8 instead.
using FU8String = TBasicString<U8CHAR>;
using FANSIString = TBasicString<ANSICHAR>;
