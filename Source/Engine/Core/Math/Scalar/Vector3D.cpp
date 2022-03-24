#include "Vector3D.h"

#include "AssertionMacros.h"
#include "Math/MathUtilities.h"

// Includes needed for FVector3D constructors.
#include "Vector2D.h"
#include "Vector4D.h"
#include "Color.h"

// Definitions for class constants.
const FVector3D FVector3D::Zero(0.0f, 0.0f, 0.0f);
const FVector3D FVector3D::One(1.0f, 1.0f, 1.0f);
const FVector3D FVector3D::XAxis(1.0f, 0.0f, 0.0f);
const FVector3D FVector3D::YAxis(0.0f, 1.0f, 0.0f);
const FVector3D FVector3D::ZAxis(0.0f, 0.0f, 1.0f);

// Constructors.
FVector3D::FVector3D(float InX, float InY, float InZ)
	: X(InX)
	, Y(InY)
	, Z(InZ)
{
}

FVector3D::FVector3D(const FVector2D& InVector, float InZ /* = 0.0f */)
	: X(InVector.X)
	, Y(InVector.Y)
	, Z(InZ)
{
}

FVector3D::FVector3D(const FVector4D& InVector)
	: X(InVector.X)
	, Y(InVector.Y)
	, Z(InVector.Z)
{
}

FVector3D::FVector3D(const FColor& InColor)
	: X(InColor.R)
	, Y(InColor.G)
	, Z(InColor.B)
{
}

// Vector math.
float FVector3D::GetLength() const
{
	return FMath::Sqrt(X*X + Y*Y + Z*Z);
}

float FVector3D::GetLengthSquared() const
{
	return X*X + Y*Y + Z*Z;
}

FVector3D& FVector3D::Normalize()
{
	float LengthSquared = GetLengthSquared();
	if (FMath::IsApproximatelyZero(LengthSquared))
	{
		// If the length of the vector is 0, we can't normalize,
		// since division by 0 is not possible.
		return *this;
	}

	float OneOverLength = FMath::InvSqrt(LengthSquared);
	X *= OneOverLength;
	Y *= OneOverLength;
	Z *= OneOverLength;
	return *this;
}

/*static*/ float FVector3D::Length(const FVector3D& InVector1, const FVector3D& InVector2)
{
	FVector3D Difference = InVector2 - InVector1;
	return Difference.GetLength();
}

/*static*/ FVector3D FVector3D::Normalize(const FVector3D& InVector)
{
	FVector3D Vector = InVector;
	return Vector.Normalize();
}

/*static*/ float FVector3D::DotProduct(const FVector3D& InVector1, const FVector3D& InVector2)
{
	return InVector1.X * InVector2.X + InVector1.Y * InVector2.Y + InVector1.Z * InVector2.Z;
}

/*static*/ FVector3D FVector3D::CrossProduct(const FVector3D& InVector1, const FVector3D& InVector2)
{
	return FVector3D(
		InVector1.Y * InVector2.Z - InVector1.Z * InVector2.Y,
		InVector1.Z * InVector2.X - InVector1.X * InVector2.Z,
		InVector1.X * InVector2.Y - InVector1.Y * InVector2.X
	);
}

// Accessors.
float& FVector3D::operator[](int32 InIndex)
{
	// We are assuming that data members are laid out in X, Y, Z order.
	ensure(InIndex >= 0 && InIndex < 3);
	return (&X)[InIndex];
}

const float& FVector3D::operator[](int32 InIndex) const
{
	ensure(InIndex >= 0 && InIndex < 3);
	return (&X)[InIndex];
}

// Component-wise operations.
FVector3D& FVector3D::operator+=(const FVector3D& InVector)
{
	X += InVector.X;
	Y += InVector.Y;
	Z += InVector.Z;
	return *this;
}

FVector3D& FVector3D::operator-=(const FVector3D& InVector)
{
	X -= InVector.X;
	Y -= InVector.Y;
	Z -= InVector.Z;
	return *this;
}

FVector3D& FVector3D::operator*=(const FVector3D& InVector)
{
	X *= InVector.X;
	Y *= InVector.Y;
	Z *= InVector.Z;
	return *this;
}

FVector3D& FVector3D::operator/=(const FVector3D& InVector)
{
	X /= InVector.X;
	Y /= InVector.Y;
	Z /= InVector.Z;
	return *this;
}

FVector3D FVector3D::operator+(const FVector3D& InVector) const
{
	FVector3D Result = *this;
	return Result += InVector;
}

FVector3D FVector3D::operator-(const FVector3D& InVector) const
{
	FVector3D Result = *this;
	return Result -= InVector;
}

FVector3D FVector3D::operator*(const FVector3D& InVector) const
{
	FVector3D Result = *this;
	return Result *= InVector;
}

FVector3D FVector3D::operator/(const FVector3D& InVector) const
{
	FVector3D Result = *this;
	return Result /= InVector;
}

// Scalar operations.
FVector3D& FVector3D::operator+=(float InValue)
{
	X += InValue;
	Y += InValue;
	Z += InValue;
	return *this;
}

FVector3D& FVector3D::operator-=(float InValue)
{
	X -= InValue;
	Y -= InValue;
	Z -= InValue;
	return *this;
}

FVector3D& FVector3D::operator*=(float InValue)
{
	X *= InValue;
	Y *= InValue;
	Z *= InValue;
	return *this;
}

FVector3D& FVector3D::operator/=(float InValue)
{
	// Multiplication is faster than division.
	float InverseValue = 1.0f / InValue;
	X *= InverseValue;
	Y *= InverseValue;
	Z *= InverseValue;
	return *this;
}

FVector3D FVector3D::operator+(float InValue) const
{
	FVector3D Result = *this;
	return Result += InValue;
}

FVector3D FVector3D::operator-(float InValue) const
{
	FVector3D Result = *this;
	return Result -= InValue;
}

FVector3D FVector3D::operator*(float InValue) const
{
	FVector3D Result = *this;
	return Result *= InValue;
}

FVector3D FVector3D::operator/(float InValue) const
{
	FVector3D Result = *this;
	return Result /= InValue;
}

FVector3D operator*(float InValue, const FVector3D& InVector)
{
	return InVector * InValue;
}

// Unary minus operator.
FVector3D FVector3D::operator-() const
{
	return FVector3D(-X, -Y, -Z);
}

// Equality operators.
bool FVector3D::operator==(const FVector3D& InVector) const
{
	return FMath::IsApproximatelyEqual(X, InVector.X) 
		&& FMath::IsApproximatelyEqual(Y, InVector.Y) 
		&& FMath::IsApproximatelyEqual(Z, InVector.Z);
}

bool FVector3D::operator!=(const FVector3D& InVector) const
{
	return !(*this == InVector);
}
