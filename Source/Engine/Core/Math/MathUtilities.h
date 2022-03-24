#pragma once

#include "CoreGlobals.h"

struct FMath
{
	// Trigonometric constants.
	static constexpr float Pi = 3.1415926535897932384626433832795f;
	static constexpr float TwoPi = 2.0f * Pi;
	static constexpr float PiOverTwo = Pi / 2.0f;
	static constexpr float PiOverFour = Pi / 4.0f;

	// Default value used to determine whether two floating-point numbers are close enough to be considered equal.
	static constexpr float Epsilon = 1.e-6f;

	// Trigonometric functions.
	static float Cos(float InRadians);
	static float Sin(float InRadians);
	static float Tan(float InRadians);
	static float Acos(float InRatio);
	static float Asin(float InRatio);
	static float Atan(float InRatio);
	static float Atan2(float InY, float InX);

	// Floating point comparison functions.
	static bool IsApproximatelyZero(float InValue, float InErrorTolerance = Epsilon);
	static bool IsApproximatelyEqual(float InValue1, float InValue2, float InErrorTolerance = Epsilon);

	// Conversion functions between degrees and radians.
	static float DegreesToRadians(float InDegrees);
	static float RadiansToDegrees(float InRadians);

	// Power functions.
	static float Pow(float InBase, float InExponent);
	static float Sqrt(float InValue);
	static float InvSqrt(float InValue);

	// Rounds up to the closest integer.
	static float Ceil(float InValue);
	// Rounds down to the closest integer.
	static float Floor(float InValue);
	// Sets the fractional part of the number to zero.
	// In other words, it acts like Floor for positive numbers and like Ceil for negative numbers.
	static float Trunc(float InValue);
	// Rounds to the closest integer, with the halfway point (0.5) rounding away from zero.
	static float Round(float InValue);
	// Rounds an integer up to its closest power of 2, with values that are already a power of 2 staying the same.
	static int32 RoundUpToNearestPowerOfTwo(int32 InValue);

	// Minimum and maximum functions.
	template<typename T>
	static T Max(T InValue1, T InValue2)
	{
		return (InValue1 >= InValue2) ? InValue1 : InValue2;
	}

	template<typename T>
	static T Min(T InValue1, T InValue2)
	{
		return (InValue1 <= InValue2) ? InValue1 : InValue2;
	}

	// Misc.
	static float Abs(float InValue);
	static float Clamp(float InValue, float InMin, float InMax);
	static bool IsPowerOfTwo(int32 InValue);
};
