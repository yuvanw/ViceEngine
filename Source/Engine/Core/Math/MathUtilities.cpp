#include "MathUtilities.h"

// System includes (for cosf, sinf, etc.)
#include <cmath>

float FMath::Cos(float InRadians)
{
	return std::cosf(InRadians);
}

float FMath::Sin(float InRadians)
{
	return std::sinf(InRadians);
}

float FMath::Tan(float InRadians)
{
	return std::tanf(InRadians);
}

float FMath::Acos(float InRatio)
{
	return std::acosf(InRatio);

}
float FMath::Asin(float InRatio)
{
	return std::asinf(InRatio);
}

float FMath::Atan(float InRatio)
{
	return std::atanf(InRatio);
}

float FMath::Atan2(float InY, float InX)
{
	return std::atan2f(InY, InX);
}

bool FMath::IsApproximatelyZero(float InValue, float InErrorTolerance /* = Epsilon */)
{
	return Abs(InValue) < InErrorTolerance;
}

bool FMath::IsApproximatelyEqual(float InValue1, float InValue2, float InErrorTolerance /* = Epsilon */)
{
	return IsApproximatelyZero(InValue1 - InValue2, InErrorTolerance);
}

float FMath::DegreesToRadians(float InDegrees)
{
	return InDegrees * (FMath::Pi / 180.0f);
}

float FMath::RadiansToDegrees(float InRadians)
{
	return InRadians * (180.0f / FMath::Pi);
}

float FMath::Pow(float InBase, float InExponent)
{
	return std::powf(InBase, InExponent);
}

float FMath::Sqrt(float InValue)
{
	return std::sqrtf(InValue);
}

float FMath::InvSqrt(float InValue)
{
	return 1.0f / Sqrt(InValue);
}

float FMath::Ceil(float InValue)
{
	return std::ceilf(InValue);
}

float FMath::Floor(float InValue)
{
	return std::floorf(InValue);
}

float FMath::Trunc(float InValue) 
{
	return std::truncf(InValue);
}

float FMath::Round(float InValue) 
{
	return std::roundf(InValue);
}

int32 FMath::RoundUpToNearestPowerOfTwo(int32 InValue)
{
	--InValue;
	InValue |= InValue >> 1;
	InValue |= InValue >> 2;
	InValue |= InValue >> 4;
	InValue |= InValue >> 8;
	InValue |= InValue >> 16;
	++InValue;
	return InValue;
}

float FMath::Abs(float InValue)
{
	return std::fabsf(InValue);
}

float FMath::Clamp(float InValue, float InMin, float InMax)
{
	return Min(InMax, Max(InValue, InMin));
}

bool FMath::IsPowerOfTwo(int32 InValue)
{
	return (InValue > 0) && ((InValue & (InValue - 1)) == 0);
}
