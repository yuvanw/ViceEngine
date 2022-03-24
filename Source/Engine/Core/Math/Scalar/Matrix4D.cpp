#include "Matrix4D.h"
#include "Vector4D.h"
#include "Math/MathUtilities.h"
#include "AssertionMacros.h"

// Definitions for class constants.
const FMatrix4D FMatrix4D::Zero(0.0f, 0.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 0.0f, 0.0f);
const FMatrix4D FMatrix4D::Identity(1.0f, 0.0f, 0.0f, 0.0f,
									0.0f, 1.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 1.0f, 0.0f,
									0.0f, 0.0f, 0.0f, 1.0f);

// Constructors.
FMatrix4D::FMatrix4D(
	float InValue00, float InValue01, float InValue02, float InValue03,
	float InValue10, float InValue11, float InValue12, float InValue13,
	float InValue20, float InValue21, float InValue22, float InValue23,
	float InValue30, float InValue31, float InValue32, float InValue33
)
{
	Data[0] = InValue00; Data[4] = InValue01; Data[8] = InValue02; Data[12] =  InValue03;
	Data[1] = InValue10; Data[5] = InValue11; Data[9] = InValue12; Data[13] =  InValue13;
	Data[2] = InValue20; Data[6] = InValue21; Data[10] = InValue22; Data[14] = InValue23;
	Data[3] = InValue30; Data[7] = InValue31; Data[11] = InValue32; Data[15] = InValue33;
}

// Accessors.
FVector4D& FMatrix4D::operator[](int32 InColumn)
{
	ensure(0 <= InColumn && InColumn <= 3);
	return (*reinterpret_cast<FVector4D*>(&Data[InColumn * 4]));
}

const FVector4D& FMatrix4D::operator[](int32 InColumn) const
{
	ensure(0 <= InColumn && InColumn <= 3);
	return (*reinterpret_cast<const FVector4D*>(&Data[InColumn * 4]));
}

const float* FMatrix4D::GetData() const
{
	return Data;
}

float* FMatrix4D::GetData()
{
	return Data;
}

// Arithmetic operations.
FVector4D FMatrix4D::operator*(const FVector4D& InVector) const
{
	return FVector4D(
		Data[0] * InVector.X + Data[4] * InVector.Y + Data[8] * InVector.Z + Data[12] * InVector.W,
		Data[1] * InVector.X + Data[5] * InVector.Y + Data[9] * InVector.Z + Data[13] * InVector.W,
		Data[2] * InVector.X + Data[6] * InVector.Y + Data[10] * InVector.Z + Data[14] * InVector.W,
		Data[3] * InVector.X + Data[7] * InVector.Y + Data[11] * InVector.Z + Data[15] * InVector.W
	);
}

// Equality operators.
bool FMatrix4D::operator==(const FMatrix4D& InMatrix) const
{
	const float* A = Data;
	const float* B = InMatrix.GetData();
	return FMath::IsApproximatelyEqual(A[0], B[0])
		&& FMath::IsApproximatelyEqual(A[1], B[1])
		&& FMath::IsApproximatelyEqual(A[2], B[2])
		&& FMath::IsApproximatelyEqual(A[3], B[3])
		&& FMath::IsApproximatelyEqual(A[4], B[4])
		&& FMath::IsApproximatelyEqual(A[5], B[5])
		&& FMath::IsApproximatelyEqual(A[6], B[6])
		&& FMath::IsApproximatelyEqual(A[7], B[7])
		&& FMath::IsApproximatelyEqual(A[8], B[8])
		&& FMath::IsApproximatelyEqual(A[9], B[9])
		&& FMath::IsApproximatelyEqual(A[10], B[10])
		&& FMath::IsApproximatelyEqual(A[11], B[11])
		&& FMath::IsApproximatelyEqual(A[12], B[12])
		&& FMath::IsApproximatelyEqual(A[13], B[13])
		&& FMath::IsApproximatelyEqual(A[14], B[14])
		&& FMath::IsApproximatelyEqual(A[15], B[15]);
}

bool FMatrix4D::operator!=(const FMatrix4D& InMatrix) const
{
	return !(*this == InMatrix);
}
