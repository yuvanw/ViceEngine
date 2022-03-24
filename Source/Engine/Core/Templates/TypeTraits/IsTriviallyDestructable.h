#pragma once

namespace NTriviallyDestructablePrivate
{
	// This is used so that a full declaration of the enum type is unnecessary.
	template<typename T, bool bIsEnum = __is_enum(T)>
	struct TIsTriviallyDestructableImpl
	{
		static constexpr bool Value = true;
	};

	// If it's not an enum, we rely on the compiler to tell us if the object is trivially destructable.
	template<typename T>
	struct TIsTriviallyDestructableImpl<T, false>
	{
		static constexpr bool Value = __has_trivial_destructor(T);
	};
}

// Traits to determine if a type is trivially destructable.
template<typename T>
struct TIsTriviallyDestructable
{
	static constexpr bool Value = NTriviallyDestructablePrivate::TIsTriviallyDestructableImpl<T>::Value;
};
