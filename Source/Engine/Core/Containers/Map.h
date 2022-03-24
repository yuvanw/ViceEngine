#pragma once

#include "CoreGlobals.h"
#include "Containers/Set.h"
#include "Templates/TypeTraits/CallTraits.h"
#include "Templates/TypeTraits/IsDefaultConstructable.h"
#include "Templates/TypeTraits/TRValueToLValueReference.h"
#include "Templates/TemplateFunctionLibrary.h"

template <typename KeyType, typename ValueType>
struct TPair
{
	// Default constructor only meant for use in container classes.
	TPair()
	{
		if (TIsDefaultConstructable<KeyType>::Value)
		{
			Key = KeyType();
		}

		if (TIsDefaultConstructable<ValueType>::Value)
		{
			Value = ValueType();
		}
	};

	TPair(const TPair& InPair) = default;
	TPair& operator=(const TPair& InPair) = default;
	TPair(TPair&& InPair) = default;
	TPair& operator=(TPair&& InPair) = default;

	TPair(const KeyType& InKey, const ValueType& InValue)
		: Key(InKey)
		, Value(InValue)
	{

	}
	TPair(const KeyType& InKey, ValueType&& InValue)
		: Key(InKey)
		, Value(MoveTemp(InValue))
	{

	}

	TPair(KeyType&& InKey, const ValueType& InValue)
		: Key(MoveTemp(InKey))
		, Value(InValue)
	{

	}

	TPair(KeyType&& InKey, ValueType&& InValue)
		: Key(MoveTemp(InKey))
		, Value(MoveTemp(InValue))
	{

	}

	KeyType Key;
	ValueType Value;
};

template <typename KeyParamType, typename ValueParamType>
struct TPairInitializer
{
	typename TRValueToLValueReference<KeyParamType>::Type Key;
	typename TRValueToLValueReference<ValueParamType>::Type Value;

	TPairInitializer(KeyParamType InKey, ValueParamType InValue)
		: Key(InKey)
		, Value(InValue)
	{
	}

	template <typename KeyType, typename ValueType>
	TPairInitializer(const TPair<KeyType, ValueType>& InPair)
		: Key(InPair.Key)
		, Value(InPair.Value)
	{
	}

	template <typename KeyType, typename ValueType>
	operator TPair<KeyType, ValueType>() const
	{
		return TPair<KeyType, ValueType>(static_cast<KeyParamType>(Key), static_cast<ValueParamType>(Value));
	}
};

template<typename KeyType, typename ValueType>
struct TDefaultMapKeyOperationsPolicy : public TKeyOperationsPolicyBase<KeyType, TPair<KeyType, ValueType>>
{
	// Redefining aliases declared in base class TKeyOperationsPolicyBase.
	// These redefined typedefs apply to the base class functions that we are inheriting.
	using KeyParamType = typename TCallTraits<KeyType>::ConstParamType;
	using ElementParamType = const TPairInitializer<typename TCallTraits<KeyType>::ParamType, typename TCallTraits<ValueType>::ParamType>&;

	// Overloading GetKeyFromElement so that we return the pair's key instead of the pair itself.
	static KeyParamType GetKeyFromElement(ElementParamType Element)
	{
		return Element.Key;
	}
};

template<typename KeyType, typename ValueType>
struct TDefaultMapHashableKeyOperations : TDefaultMapKeyOperationsPolicy<KeyType, ValueType>
{
	// This line fails to compile if GetTypeHash hasn't been overloaded for KeyType.
	using HashabilityCheck = decltype(GetTypeHash(DeclVal<const KeyType>()));
};

/**
 * A hash map implementation that does not allow for duplicate keys.
 */
template<typename KeyType, typename ValueType, typename KeyOperations = TDefaultMapKeyOperationsPolicy<KeyType, ValueType>>
class TMap
{
	using KeyConstParamType = typename TCallTraits<KeyType>::ConstParamType;
	using KeyParamType = typename TCallTraits<KeyType>::ParamType;
	using ValueParamType = typename TCallTraits<ValueType>::ParamType;

public:
	TMap() = default;
	~TMap() = default;

	void Add(const KeyType& InKey, const ValueType& InValue)
	{
		Set.Add(TPair<KeyType, ValueType>(InKey, InValue));
	}

	void Add(const KeyType& InKey, ValueType&& InValue)
	{
		Set.Add(TPair<KeyType, ValueType>(InKey, MoveTempIfPossible(InValue)));
	}

	void Add(KeyType&& InKey, const ValueType& InValue)
	{
		Set.Add(TPair<KeyType, ValueType>(MoveTempIfPossible(InKey), InValue));
	}

	void Add(KeyType&& InKey, ValueType&& InValue)
	{
		Set.Add(TPair<KeyType, ValueType>(MoveTempIfPossible(InKey), MoveTempIfPossible(InValue)));
	}

	void Remove(KeyConstParamType InKey, bool bShouldDeleteKey = true)
	{
		Set.Remove(InKey);
	}

	const ValueType* Find(KeyConstParamType InKey) const
	{
		return const_cast<ValueType*>(
			static_cast<const TMap&>(*this).Find(InKey)
		);
	}

	ValueType* Find(KeyConstParamType InKey)
	{
		if (TPair<KeyType, ValueType>* Pair = Set.Find(InKey))
		{
			return &Pair->Value;
		}

		return nullptr;
	}

	template<typename ComparableKeyType>
	const KeyType* FindByHash(uint64 InHash, const ComparableKeyType& InKey) const
	{
		return Set.FindIndexByHash(InHash, InKey);
	}

	template<typename ComparableKeyType>
	KeyType* FindByHash(uint64 InHash, const ComparableKeyType& InKey)
	{
		return Set.FindIndexByHash(InHash, InKey);
	}

	int32 FindIndexByKey(KeyConstParamType InKey) const
	{
		return Set.FindIndexByKey(InKey);
	}

	template<typename ComparableKeyType>
	int32 FindIndexByHash(uint64 InHash, const ComparableKeyType& InKey) const
	{
		Set.FindIndexByHash(InHash, InKey);
	}

	void Empty()
	{
		Set.Empty();
	}

	bool IsEmpty() const
	{
		return Set.IsEmpty();
	}
	bool IsKeyContained(KeyConstParamType InKey) const
	{
		return Set.IsKeyContained(InKey);
	}
	bool IsHashContained(uint64 InHash) const
	{
		return Set.IsHashContained(InHash);
	}

	// Getters.
	int32 GetSize() const
	{
		return Set.GetSize();
	}
	int32 GetCapacity() const
	{
		return Set.GetCapacity();
	}
	float GetLoadFactor() const
	{
		return Set.GetLoadFactor();
	}
	float GetRehashLoadFactor() const
	{
		return Set.GetRehashLoadFactor();
	}
	IAllocator* GetAllocator()
	{
		return Set.GetAllocator();
	}

	// Setters.
	void SetAllocator(const IAllocator& InAllocator)
	{
		Set.SetAllocator(InAllocator);
	}

private:
	TSet<TPair<KeyType, ValueType>, KeyOperations> Set;
};
