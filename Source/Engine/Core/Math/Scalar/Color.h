#pragma once

#include "CoreGlobals.h"

// Forward declarations needed for FColor constructors.
class FVector3D;
class FVector4D;

/**
 * RGBA color with 32 bits per component.
 */
class FColor
{
public:
	// Constants.
	static const FColor Black;
	static const FColor White;
	static const FColor Red;
	static const FColor Green;
	static const FColor Blue;

	// Constructors
	FColor() = default;
	// Construct using input R, B, and B values, with A set to 1.0f.
	FColor(float InR, float InG, float InB);
	FColor(float InR, float InG, float InB, float InA);
	// Construct using the X, Y, and Z members of an FVector3D.
	FColor(const FVector3D& InVector);
	// Construct using the X, Y, Z, and W members of an FVector4D.
	explicit FColor(const FVector4D& InVector);

	// Copy operations.
	FColor(const FColor& InVector) = default;
	FColor& operator=(const FColor& InVector) = default;

	// Accessors.
	float& operator[](int32 InIndex);
	const float& operator[](int32 InIndex) const;

	// Equality operators.
	bool operator==(const FColor& InVector) const;
	bool operator!=(const FColor& InVector) const;

	float R = 0.0f;
	float G = 0.0f;
	float B = 0.0f;
	float A = 0.0f;
};
