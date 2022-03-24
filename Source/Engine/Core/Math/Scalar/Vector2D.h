#pragma once

#include "CoreGlobals.h"

// Forward declarations needed for FVector2D constructors.
class FVector3D;
class FVector4D;

/**
 * 2D direction vector of floating point values.
 */
class FVector2D
{
public:
	// Zero vector (0, 0)
	static const FVector2D Zero;
	// One vector (1, 1)
	static const FVector2D One;
	// Unit X axis vector (1, 0)
	static const FVector2D XAxis;
	// Unit Y axis vector (0, 1)
	static const FVector2D YAxis;

	// Constructors.
	FVector2D() = default;
	FVector2D(float InX, float InY);
	// Construct using the X and Y members of an FVector3D.
	explicit FVector2D(const FVector3D& InVector);
	// Construct using the X and Y members of an FVector4D.
	explicit FVector2D(const FVector4D& InVector);

	// Copy operations.
	FVector2D(const FVector2D& InVector) = default;
	FVector2D& operator=(const FVector2D& InVector) = default;

	// Vector math.
	float GetLength() const;
	float GetLengthSquared() const;
	FVector2D& Normalize();
	static float DotProduct(const FVector2D& InVector1, const FVector2D& InVector2);

	// Accessors.
	float& operator[](int32 InIndex);
	const float& operator[](int32 InIndex) const;

	// Component-wise operations.
	FVector2D& operator+=(const FVector2D& InVector);
	FVector2D& operator-=(const FVector2D& InVector);
	FVector2D& operator*= (const FVector2D& InVector);
	FVector2D& operator/=(const FVector2D& InVector);
	FVector2D operator+(const FVector2D& InVector) const;
	FVector2D operator-(const FVector2D& InVector) const;
	FVector2D operator*(const FVector2D& InVector) const;
	FVector2D operator/(const FVector2D& InVector) const;

	// Scalar operations.
	FVector2D& operator+=(float InValue);
	FVector2D& operator-=(float InValue);
	FVector2D& operator*=(float InValue);
	FVector2D& operator/=(float InValue);
	FVector2D operator+(float InValue) const;
	FVector2D operator-(float InValue) const;
	FVector2D operator*(float InValue) const;
	FVector2D operator/(float InValue) const;

	// Unary minus operator (creates a negated copy of the FVector2D).
	FVector2D operator-() const;

	// Equality operators.
	bool operator==(const FVector2D& InVector) const;
	bool operator!=(const FVector2D& InVector) const;

	float X = 0.0f;
	float Y = 0.0f;
};

// Scalar multiplication.
FVector2D operator*(float InValue, const FVector2D& InVector);
