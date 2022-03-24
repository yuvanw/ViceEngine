#pragma once

#include "CoreGlobals.h"

// Forward declarations needed for FVector3D constructors.
class FVector2D;
class FVector4D;
class FColor;

/**
 * 3D direction vector of floating point values.
 */
class FVector3D
{
public:
	// Zero vector (0, 0, 0)
	static const FVector3D Zero;
	// One vector (1, 1, 1)
	static const FVector3D One;
	// Unit X axis vector (1, 0, 0)
	static const FVector3D XAxis;
	// Unit Y axis vector (0, 1, 0)
	static const FVector3D YAxis;
	// Unit Z axis vector (0, 0, 1)
	static const FVector3D ZAxis;
	
	// Constructors.
	FVector3D() = default;
	FVector3D(float InX, float InY, float InZ);
	// Construct using the R, G, and B members of an FColor.
	FVector3D(const FColor& InColor);
	// Construct using the X and Y members of an FVector2D.
	explicit FVector3D(const FVector2D& InVector, float InZ = 0.0f);
	// Construct using the X, Y, and Z members of an FVector4D.
	explicit FVector3D(const FVector4D& InVector);

	// Copy operations.
	FVector3D(const FVector3D& InVector) = default;
	FVector3D& operator=(const FVector3D& InVector) = default;

	// Vector math.
	float GetLength() const;
	float GetLengthSquared() const;
	FVector3D& Normalize();

	static float Length(const FVector3D& InVector1, const FVector3D& InVector2);
	static FVector3D Normalize(const FVector3D& InVector);
	static float DotProduct(const FVector3D& InVector1, const FVector3D& InVector2);
	static FVector3D CrossProduct(const FVector3D& InVector1, const FVector3D& InVector2);

	// Accessors.
	float& operator[](int32 InIndex);
	const float& operator[](int32 InIndex) const;

	// Component-wise operations.
	FVector3D& operator+=(const FVector3D& InVector);
	FVector3D& operator-=(const FVector3D& InVector);
	FVector3D& operator*= (const FVector3D& InVector);
	FVector3D& operator/=(const FVector3D& InVector);
	FVector3D operator+(const FVector3D& InVector) const;
	FVector3D operator-(const FVector3D& InVector) const;
	FVector3D operator*(const FVector3D& InVector) const;
	FVector3D operator/(const FVector3D& InVector) const;

	// Scalar operations.
	FVector3D& operator+=(float InValue);
	FVector3D& operator-=(float InValue);
	FVector3D& operator*=(float InValue);
	FVector3D& operator/=(float InValue);
	FVector3D operator+(float InValue) const;
	FVector3D operator-(float InValue) const;
	FVector3D operator*(float InValue) const;
	FVector3D operator/(float InValue) const;

	// Unary minus operator (creates a negated copy of the FVector3D).
	FVector3D operator-() const;

	// Equality operators.
	bool operator==(const FVector3D& InVector) const;
	bool operator!=(const FVector3D& InVector) const;

	float X = 0.0f;
	float Y = 0.0f;
	float Z = 0.0f;
};

// Scalar multiplication.
FVector3D operator*(float InValue, const FVector3D& InVector);
