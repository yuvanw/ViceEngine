#pragma once

#include "Templates/TypeTraits/CallTraits.h"
#include "Hash/PrimitiveTypeHash.h"

/**
 * This is a means of comparing keys, extracting the actual key from an InElement.
 * This is meant to abstract the way types are compared and should let the container be templated on it.
 * This can also protect against types that do not have a KeyOperations::GetHashFromKey function implemented.
 */
template<typename KeyType, typename ElementType, bool bInAllowDuplicateKeys = false>
struct TKeyOperationsPolicyBase
{
	using KeyParamType = typename TCallTraits<KeyType>::ConstParamType;
	using ElementParamType = typename TCallTraits<ElementType>::ParamType;

	static uint64 GetHashFromKey(KeyParamType InKey)
	{
		return GetTypeHash(InKey);
	}

	template<typename ComparableKey>
	static uint64 GetHashFromKey(ComparableKey InComparableKey)
	{
		return GetTypeHash(InComparableKey);
	}

	static KeyParamType GetKeyFromElement(ElementParamType InElement)
	{
		return InElement;
	}

	static bool DoKeysMatch(KeyParamType InKey, KeyParamType InOtherKey)
	{
		return InKey == InOtherKey;
	}

	template<typename ComparableKey>
	static bool DoKeysMatch(KeyParamType InKey, ComparableKey InComparableKey)
	{
		return InKey == InComparableKey;
	}

	static constexpr bool bAllowDuplicateKeys = bInAllowDuplicateKeys;
};
