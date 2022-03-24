#include "Vector4D.h"

#include "AssertionMacros.h"
#include "Math/MathUtilities.h"

// Includes needed for FVector4D constructors.
#include "Vector2D.h"
#include "Vector3D.h"

// Definitions for class constants.
const FVector4D FVector4D::Zero(0.0f, 0.0f, 0.0f, 0.0f);
const FVector4D FVector4D::One(1.0f, 1.0f, 1.0f, 1.0f);
const FVector4D FVector4D::XAxis(1.0f, 0.0f, 0.0f, 0.0f);
const FVector4D FVector4D::YAxis(0.0f, 1.0f, 0.0f, 0.0f);
const FVector4D FVector4D::ZAxis(0.0f, 0.0f, 1.0f, 0.0f);
const FVector4D FVector4D::UnitW(0.0f, 0.0f, 0.0f, 1.0f);

// Constructors.
FVector4D::FVector4D(float InX, float InY, float InZ, float InW)
	: X(InX)
	, Y(InY)
	, Z(InZ)
	, W(InW)
{
}

FVector4D::FVector4D(const FVector2D& InVector, float InZ /* = 0.0f */, float InW /* = 0.0f */)
	: X(InVector.X)
	, Y(InVector.Y)
	, Z(InZ)
	, W(InW)
{
}

FVector4D::FVector4D(const FVector3D& InVector, float InW /* = 0.0f */)
	: X(InVector.X)
	, Y(InVector.Y)
	, Z(InVector.Z)
	, W(InW)
{
}

// Vector math.
float FVector4D::GetLength() const
{
	return FMath::Sqrt(X*X + Y*Y + Z*Z + W*W);
}

float FVector4D::GetLengthSquared() const
{
	return X*X + Y*Y + Z*Z + W*W;
}

FVector4D& FVector4D::Normalize()
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
	W *= OneOverLength;
	return *this;
}

/*static*/ float FVector4D::DotProduct(const FVector4D& InVector1, const FVector4D& InVector2)
{
	return InVector1.X * InVector2.X + InVector1.Y * InVector2.Y + InVector1.Z * InVector2.Z + InVector1.W * InVector2.W;
}

/*static*/ FVector4D FVector4D::CrossProduct(const FVector4D& InVector1, const FVector4D& InVector2)
{
	return FVector4D(
		InVector1.Y * InVector2.Z - InVector1.Z * InVector2.Y,
		InVector1.Z * InVector2.X - InVector1.X * InVector2.Z,
		InVector1.X * InVector2.Y - InVector1.Y * InVector2.X,
		0.0f
	);
}

// Accessors.
float& FVector4D::operator[](int32 InIndex)
{
	// We are assuming that data members are laid out in X, Y, Z order.
	ensure(InIndex >= 0 && InIndex < 4);
	return (&X)[InIndex];
}

const float& FVector4D::operator[](int32 InIndex) const
{
	ensure(InIndex >= 0 && InIndex < 4);
	return (&X)[InIndex];
}

// Component-wise operations.
FVector4D& FVector4D::operator+=(const FVector4D& InVector)
{
	X += InVector.X;
	Y += InVector.Y;
	Z += InVector.Z;
	W += InVector.W;
	return *this;
}

FVector4D& FVector4D::operator-=(const FVector4D& InVector)
{
	X -= InVector.X;
	Y -= InVector.Y;
	Z -= InVector.Z;
	W -= InVector.W;
	return *this;
}

FVector4D& FVector4D::operator*=(const FVector4D& InVector)
{
	X *= InVector.X;
	Y *= InVector.Y;
	Z *= InVector.Z;
	W *= InVector.W;
	return *this;
}

FVector4D& FVector4D::operator/=(const FVector4D& InVector)
{
	X /= InVector.X;
	Y /= InVector.Y;
	Z /= InVector.Z;
	W /= InVector.W;
	return *this;
}

FVector4D FVector4D::operator+(const FVector4D& InVector) const
{
	FVector4D Result = *this;
	return Result += InVector;
}

FVector4D FVector4D::operator-(const FVector4D& InVector) const
{
	FVector4D Result = *this;
	return Result -= InVector;
}

FVector4D FVector4D::operator*(const FVector4D& InVector) const
{
	FVector4D Result = *this;
	return Result *= InVector;
}

FVector4D FVector4D::operator/(const FVector4D& InVector) const
{
	FVector4D Result = *this;
	return Result /= InVector;
}

// Scalar operations
FVector4D& FVector4D::operator+=(float InValue)
{
	X += InValue;
	Y += InValue;
	Z += InValue;
	W += InValue;
	return *this;
}

FVector4D& FVector4D::operator-=(float InValue)
{
	X -= InValue;
	Y -= InValue;
	Z -= InValue;
	W -= InValue;
	return *this;
}

FVector4D& FVector4D::operator*=(float InValue)
{
	X *= InValue;
	Y *= InValue;
	Z *= InValue;
	W *= InValue;
	return *this;
}

FVector4D& FVector4D::operator/=(float InValue)
{
	// Multiplication is faster than division.
	float InverseValue = 1.0f / InValue;
	X *= InverseValue;
	Y *= InverseValue;
	Z *= InverseValue;
	W *= InverseValue;
	return *this;
}

FVector4D FVector4D::operator+(float InValue) const
{
	FVector4D Result = *this;
	return Result += InValue;
}

FVector4D FVector4D::operator-(float InValue) const
{
	FVector4D Result = *this;
	return Result -= InValue;
}

FVector4D FVector4D::operator*(float InValue) const
{
	FVector4D Result = *this;
	return Result *= InValue;
}

FVector4D FVector4D::operator/(float InValue) const
{
	FVector4D Result = *this;
	return Result /= InValue;
}

FVector4D operator*(float InValue, const FVector4D& InVector)
{
	return InVector * InValue;
}

// Unary minus operator.
FVector4D FVector4D::operator-() const
{
	return FVector4D(-X, -Y, -Z, -W);
}

// Equality operators.
bool FVector4D::operator==(const FVector4D& InVector) const
{
	return FMath::IsApproximatelyEqual(X, InVector.X)
		&& FMath::IsApproximatelyEqual(Y, InVector.Y)
		&& FMath::IsApproximatelyEqual(Z, InVector.Z)
		&& FMath::IsApproximatelyEqual(W, InVector.W);
}

bool FVector4D::operator!=(const FVector4D& InVector) const
{
	return !(*this == InVector);
}
