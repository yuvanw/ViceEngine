#include "Color.h"

#include "AssertionMacros.h"
#include "Math/MathUtilities.h"

// Includes needed for FColor constructors.
#include "Vector3D.h"
#include "Vector4D.h"

// Definitions for class constants.
const FColor FColor::Black(0.0f, 0.0f, 0.0f);
const FColor FColor::White(1.0f, 1.0f, 1.0f);
const FColor FColor::Red(1.0f, 0.0f, 0.0f);
const FColor FColor::Green(0.0f, 1.0f, 0.0f);
const FColor FColor::Blue(0.0f, 0.0f, 1.0f);

// Constructors.
FColor::FColor(float InR, float InG, float InB)
	: R(FMath::Clamp(InR, 0.0f, 1.0f))
	, G(FMath::Clamp(InG, 0.0f, 1.0f))
	, B(FMath::Clamp(InB, 0.0f, 1.0f))
	, A(1.0f)
{
}

FColor::FColor(float InR, float InG, float InB, float InA)
	: R(FMath::Clamp(InR, 0.0f, 1.0f))
	, G(FMath::Clamp(InG, 0.0f, 1.0f))
	, B(FMath::Clamp(InB, 0.0f, 1.0f))
	, A(FMath::Clamp(InA, 0.0f, 1.0f))
{
}

FColor::FColor(const FVector3D& InVector)
	: R(FMath::Clamp(InVector.X, 0.0f, 1.0f))
	, G(FMath::Clamp(InVector.Y, 0.0f, 1.0f))
	, B(FMath::Clamp(InVector.Z, 0.0f, 1.0f))
	, A(1.0f)
{
}

FColor::FColor(const FVector4D& InVector)
	: R(FMath::Clamp(InVector.X, 0.0f, 1.0f))
	, G(FMath::Clamp(InVector.Y, 0.0f, 1.0f))
	, B(FMath::Clamp(InVector.Z, 0.0f, 1.0f))
	, A(FMath::Clamp(InVector.W, 0.0f, 1.0f))
{
}

// Accessors
float& FColor::operator[](int32 InIndex)
{
	// We are assuming that data members are laid out in R, G, B, A order.
	ensure(InIndex >= 0 && InIndex < 3);
	return (&R)[InIndex];
}

const float& FColor::operator[](int32 InIndex) const
{
	ensure(InIndex >= 0 && InIndex < 3);
	return (&R)[InIndex];
}

// Equality operators
bool FColor::operator==(const FColor& InColor) const
{
	return FMath::IsApproximatelyEqual(R, InColor.R)
		&& FMath::IsApproximatelyEqual(G, InColor.G)
		&& FMath::IsApproximatelyEqual(B, InColor.B);
}

bool FColor::operator!=(const FColor& InColor) const
{
	return !(*this == InColor);
}
