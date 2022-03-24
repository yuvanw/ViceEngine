#pragma once

#include "CoreGlobals.h"

// Forward declarations needed for FVector4D constructors.
class FVector2D;
class FVector3D;

/**
 * 4D homogeneous vector of floating point values.
 */
class FVector4D
{
public:
	// Zero vector (0, 0, 0, 0)
	static const FVector4D Zero;
	// One vector (1, 1, 1, 1)
	static const FVector4D One;
	// Unit X axis vector (1, 0, 0, 0)
	static const FVector4D XAxis;
	// Unit Y axis vector (0, 1, 0, 0)
	static const FVector4D YAxis;
	// Unit Z axis vector (0, 0, 1, 0)
	static const FVector4D ZAxis;
	// Unit Z axis vector (0, 0, 0, 1)
	static const FVector4D UnitW;

	// Constructors.
	FVector4D() = default;
	FVector4D(float InX, float InY, float InZ, float InW);
	// Construct using the X and Y members of an FVector2D.
	explicit FVector4D(const FVector2D& InVector, float InZ = 0.0f, float InW = 0.0f);
	// Construct using the X, Y, and Z members of an FVector3D.
	explicit FVector4D(const FVector3D& InVector, float InW = 0.0f);

	// Copy operations.
	FVector4D(const FVector4D& InVector) = default;
	FVector4D& operator=(const FVector4D& InVector) = default;

	// Vector math.
	float GetLength() const;
	float GetLengthSquared() const;
	FVector4D& Normalize();
	static float DotProduct(const FVector4D& InVector1, const FVector4D& InVector2);
	// Calculates the cross product using X, Y, and Z. W is set to zero.
	static FVector4D CrossProduct(const FVector4D& InVector1, const FVector4D& InVector2);

	// Accessors.
	float& operator[](int32 InIndex);
	const float& operator[](int32 InIndex) const;

	// Component-wise operations.
	FVector4D& operator+=(const FVector4D& InVector);
	FVector4D& operator-=(const FVector4D& InVector);
	FVector4D& operator*= (const FVector4D& InVector);
	FVector4D& operator/=(const FVector4D& InVector);
	FVector4D operator+(const FVector4D& InVector) const;
	FVector4D operator-(const FVector4D& InVector) const;
	FVector4D operator*(const FVector4D& InVector) const;
	FVector4D operator/(const FVector4D& InVector) const;

	// Scalar operations.
	FVector4D& operator+=(float InValue);
	FVector4D& operator-=(float InValue);
	FVector4D& operator*=(float InValue);
	FVector4D& operator/=(float InValue);
	FVector4D operator+(float InValue) const;
	FVector4D operator-(float InValue) const;
	FVector4D operator*(float InValue) const;
	FVector4D operator/(float InValue) const;

	// Unary minus operator (creates a negated copy of the FVector4D).
	FVector4D operator-() const;

	// Equality operators.
	bool operator==(const FVector4D& InVector) const;
	bool operator!=(const FVector4D& InVector) const;

	float X = 0.0f;
	float Y = 0.0f;
	float Z = 0.0f;
	float W = 0.0f;
};

// Scalar multiplication.
FVector4D operator*(float InValue, const FVector4D& InVector);
