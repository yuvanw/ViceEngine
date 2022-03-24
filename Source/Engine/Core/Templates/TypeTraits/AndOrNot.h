#pragma once

// Performs a boolean AND on the ::Value static member of each type, and short-circuits if any ::Value is false.
template <typename... Types>
struct TAnd;

template <bool LHSValue, typename... RHS>
struct TAndValue
{
	static constexpr bool Value = TAnd<RHS...>::Value;
};

template <typename... RHS>
struct TAndValue<false, RHS...>
{
	static constexpr bool Value = false;
};

template <typename LHS, typename... RHS>
struct TAnd<LHS, RHS...> : TAndValue<LHS::Value, RHS...>
{
};

template <>
struct TAnd<>
{
	static constexpr bool Value = true;
};

// Performs a boolean OR on the ::Value static member of each type, and short-circuits if any ::Value is true.
template <typename... Types>
struct TOr;

template <bool LHSValue, typename... RHS>
struct TOrValue
{
	static constexpr bool Value = TOr<RHS...>::Value;
};

template <typename... RHS>
struct TOrValue<true, RHS...>
{
	static constexpr bool Value = true;
};

template <typename LHS, typename... RHS>
struct TOr<LHS, RHS...> : TOrValue<LHS::Value, RHS...>
{
};

template <>
struct TOr<>
{
	static constexpr bool Value = false;
};

// Performs a boolean NOT on the ::Value static member of the type.
template <typename Type>
struct TNot
{
	static constexpr bool Value = !Type::Value;
};
