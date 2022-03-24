#pragma once
#include "TypeTraits/RemoveReference.h"
#include "TypeTraits/Identity.h"
#include "TypeTraits/IsLValueReferenceType.h"
#include "TypeTraits/AreTypesEqual.h"
#include "TypeTraits/IsConstType.h"


// Follows Unreal's Implicit Conv.
// This is safer than your standard C-style Cast or static_cast as it guards against downcasting.
template <typename T>
inline T ImplicitConv(typename TIdentity<T>::Type Obj)
{
	return Obj;
}

// Follows Unreal's implementation.
// This guards against passing in r-values and const l-value references.
// Former is redundant, latter is a mistake.
template <typename T>
inline typename TRemoveReference<T>::Type&& MoveTemp(T&& InObject)
{
	typedef typename TRemoveReference<T>::Type CastType;
	static_assert(TIsLValueReference<T>::Value, "Calling MoveTemp on a r-value reference. Redundant.");
	static_assert(!TIsConstType<T>::Value, "Calling MoveTemp on a const object.");
	return (CastType&&)InObject;
}

// Follow's Unreal's implementation.
// Same behaviour as std::move().
template <typename T>
inline typename TRemoveReference<T>::Type&& MoveTempIfPossible(T&& InObject)
{
	typedef typename TRemoveReference<T>::Type CastType;
	return (CastType&&) InObject;
}

// Follows Unreal's implementation of Forward.
// Casts the passed in object to an r-value reference.
template<typename T>
inline T&& Forward(typename TRemoveReference<T>::Type& InObject)
{
	return (T&&)InObject;
}

template<typename T>
inline T&& Forward(typename TRemoveReference<T>::Type&& InObject)
{
	return (T&&)InObject;
}

template <typename T>
T&& DeclVal();
