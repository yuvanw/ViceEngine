#include "Vector2D.h"

#include "AssertionMacros.h"
#include "Math/MathUtilities.h"

// Includes needed for FVector2D constructors.
#include "Vector3D.h"
#include "Vector4D.h"

// Definitions for class constants.
const FVector2D FVector2D::Zero(0.0f, 0.0f);
const FVector2D FVector2D::One(1.0f, 1.0f);
const FVector2D FVector2D::XAxis(1.0f, 0.0f);
const FVector2D FVector2D::YAxis(0.0f, 1.0f);

// Constructors.
FVector2D::FVector2D(float InX, float InY)
	: X(InX)
	, Y(InY)
{
}

FVector2D::FVector2D(const FVector3D& InVector)
	: X(InVector.X)
	, Y(InVector.Y)
{
}

FVector2D::FVector2D(const FVector4D& InVector)
	: X(InVector.X)
	, Y(InVector.Y)
{
}

// Vector math.
float FVector2D::GetLength() const
{
	return FMath::Sqrt(X*X + Y*Y);
}

float FVector2D::GetLengthSquared() const
{
	return X*X + Y*Y;
}

FVector2D& FVector2D::Normalize()
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
	return *this;
}

/*static*/ float FVector2D::DotProduct(const FVector2D& InVector1, const FVector2D& InVector2)
{
	return InVector1.X * InVector2.X + InVector1.Y * InVector2.Y;
}

// Accessors
float& FVector2D::operator[](int32 InIndex)
{
	// We are assuming that data members are laid out in X, Y order.
	ensure(InIndex >= 0 && InIndex < 2);
	return (&X)[InIndex];
}

const float& FVector2D::operator[](int32 InIndex) const
{
	ensure(InIndex >= 0 && InIndex < 2);
	return (&X)[InIndex];
}

// Component-wise operations.
FVector2D& FVector2D::operator+=(const FVector2D& InVector)
{
	X += InVector.X;
	Y += InVector.Y;
	return *this;
}

FVector2D& FVector2D::operator-=(const FVector2D& InVector)
{
	X -= InVector.X;
	Y -= InVector.Y;
	return *this;
}

FVector2D& FVector2D::operator*=(const FVector2D& InVector)
{
	X *= InVector.X;
	Y *= InVector.Y;
	return *this;
}

FVector2D& FVector2D::operator/=(const FVector2D& InVector)
{
	X /= InVector.X;
	Y /= InVector.Y;
	return *this;
}

FVector2D FVector2D::operator+(const FVector2D& InVector) const
{
	FVector2D Result = *this;
	return Result += InVector;
}

FVector2D FVector2D::operator-(const FVector2D& InVector) const
{
	FVector2D Result = *this;
	return Result -= InVector;
}

FVector2D FVector2D::operator*(const FVector2D& InVector) const
{
	FVector2D Result = *this;
	return Result *= InVector;
}

FVector2D FVector2D::operator/(const FVector2D& InVector) const
{
	FVector2D Result = *this;
	return Result /= InVector;
}

// Scalar operations
FVector2D& FVector2D::operator+=(float InValue)
{
	X += InValue;
	Y += InValue;
	return *this;
}

FVector2D& FVector2D::operator-=(float InValue)
{
	X -= InValue;
	Y -= InValue;
	return *this;
}

FVector2D& FVector2D::operator*=(float InValue)
{
	X *= InValue;
	Y *= InValue;
	return *this;
}

FVector2D& FVector2D::operator/=(float InValue)
{
	// Multiplication is faster than division.
	float InverseValue = 1.0f / InValue;
	X *= InverseValue;
	Y *= InverseValue;
	return *this;
}

FVector2D FVector2D::operator+(float InValue) const
{
	FVector2D Result = *this;
	return Result += InValue;
}

FVector2D FVector2D::operator-(float InValue) const
{
	FVector2D Result = *this;
	return Result -= InValue;
}

FVector2D FVector2D::operator*(float InValue) const
{
	FVector2D Result = *this;
	return Result *= InValue;
}

FVector2D FVector2D::operator/(float InValue) const
{
	FVector2D Result = *this;
	return Result /= InValue;
}

FVector2D operator*(float InValue, const FVector2D& InVector)
{
	return InVector * InValue;
}

// Unary minus operator.
FVector2D FVector2D::operator-() const
{
	return FVector2D(-X, -Y);
}

// Equality operators.
bool FVector2D::operator==(const FVector2D& InVector) const
{
	return FMath::IsApproximatelyEqual(X, InVector.X) && FMath::IsApproximatelyEqual(Y, InVector.Y);
}

bool FVector2D::operator!=(const FVector2D& InVector) const
{
	return !(*this == InVector);
}
