#include "Transform4D.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "Matrix4D.h"
#include "Math/MathUtilities.h"
#include "AssertionMacros.h"

// Definitions for class constants.
const FTransform4D FTransform4D::Identity(1.0f, 0.0f, 0.0f, 0.0f,
										  0.0f, 1.0f, 0.0f, 0.0f,
										  0.0f, 0.0f, 1.0f, 0.0f);

// Constructors.
FTransform4D::FTransform4D()
{
	Data[0] = 1.0f; Data[3] = 0.0f; Data[6] = 0.0f; Data[9] = 0.0f;
	Data[1] = 0.0f; Data[4] = 1.0f; Data[7] = 0.0f; Data[10] = 0.0f;
	Data[2] = 0.0f; Data[5] = 0.0f; Data[8] = 1.0f; Data[11] = 0.0f;
}

FTransform4D::FTransform4D(
	float InValue00, float InValue01, float InValue02, float InValue03,
	float InValue10, float InValue11, float InValue12, float InValue13,
	float InValue20, float InValue21, float InValue22, float InValue23
)
{
	Data[0] = InValue00; Data[3] = InValue01; Data[6] = InValue02; Data[9] = InValue03;
	Data[1] = InValue10; Data[4] = InValue11; Data[7] = InValue12; Data[10] = InValue13;
	Data[2] = InValue20; Data[5] = InValue21; Data[8] = InValue22; Data[11] = InValue23;
}

FTransform4D::FTransform4D(const FVector3D& InColumn0, const FVector3D& InColumn1, const FVector3D& InColumn2, const FVector3D& InColumn3)
{
	Data[0] = InColumn0.X; Data[3] = InColumn1.X; Data[6] = InColumn2.X; Data[9] = InColumn3.X;
	Data[1] = InColumn0.Y; Data[4] = InColumn1.Y; Data[7] = InColumn2.Y; Data[10] = InColumn3.Y;
	Data[2] = InColumn0.Z; Data[5] = InColumn1.Z; Data[8] = InColumn2.Z; Data[11] = InColumn3.Z;
}

// Accessors.
FVector3D& FTransform4D::operator[](int32 InColumn)
{
	ensure(0 <= InColumn && InColumn <= 3);
	return (*reinterpret_cast<FVector3D*>(&Data[InColumn * 3]));
}

const FVector3D& FTransform4D::operator[](int32 InColumn) const
{
	ensure(0 <= InColumn && InColumn <= 3);
	return (*reinterpret_cast<const FVector3D*>(&Data[InColumn * 3]));
}

float* FTransform4D::GetData()
{
	return Data;
}

const float* FTransform4D::GetData() const
{
	return Data;
}

const FVector3D& FTransform4D::GetTranslation() const
{
	return (*reinterpret_cast<const FVector3D*>(&Data[9]));
}

void FTransform4D::SetTranslation(const FVector3D& InTranslation)
{
	(*this)[3] = InTranslation;
}

// Transform matrix generation functions.
/*static*/ FTransform4D FTransform4D::MakeScale(float InScale)
{
	return FTransform4D(
		InScale, 0.0f, 0.0f, 0.0f,
		0.0f, InScale, 0.0f, 0.0f,
		0.0f, 0.0f, InScale, 0.0f
	);
}

/*static*/ FTransform4D FTransform4D::MakeScale(const FVector3D& InScale)
{
	return FTransform4D(
		InScale.X, 0.0f, 0.0f, 0.0f,
		0.0f, InScale.Y, 0.0f, 0.0f,
		0.0f, 0.0f, InScale.Z, 0.0f
	);
}

/*static*/ FTransform4D FTransform4D::MakeRotationX(float InRadians)
{
	float Cos = FMath::Cos(InRadians);
	float Sin = FMath::Sin(InRadians);
	return FTransform4D(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, Cos, -Sin, 0.0f,
		0.0f, Sin, Cos, 0.0f
	);
}

/*static*/ FTransform4D FTransform4D::MakeRotationY(float InRadians)
{
	float Cos = FMath::Cos(InRadians);
	float Sin = FMath::Sin(InRadians);
	return FTransform4D(
		Cos, 0.0f, Sin, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		-Sin, 0.0f, Cos, 0.0f
	);
}

/*static*/ FTransform4D FTransform4D::MakeRotationZ(float InRadians)
{
	float Cos = FMath::Cos(InRadians);
	float Sin = FMath::Sin(InRadians);
	return FTransform4D(
		Cos, -Sin, 0.0f, 0.0f,
		Sin, Cos, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f
	);
}

/**
* Formula for computing a rotation matrix about an arbitrary axis taken from
* Foundations of Game Engine Development, Volume 1: Mathematics (page 64).
*/
/*static*/ FTransform4D FTransform4D::MakeRotation(float InRadians, const FVector3D& InAxis)
{
	float Cos = FMath::Cos(InRadians);
	float Sin = FMath::Sin(InRadians);
	float Diff = 1.0f - Cos;

	float X = InAxis.X * Diff;
	float Y = InAxis.Y * Diff;
	float Z = InAxis.Z * Diff;
	float XY = X * InAxis.Y;
	float XZ = X * InAxis.Z;
	float YZ = Y * InAxis.Z;

	return FTransform4D(
		Cos + X * InAxis.X, XY - Sin * InAxis.Z, XZ + Sin * InAxis.Y, 0.0f,
		XY + Sin * InAxis.Z, Cos + Y * InAxis.Y, YZ - Sin * InAxis.X, 0.0f,
		XZ - Sin * InAxis.Y, YZ + Sin * InAxis.X, Cos + Z * InAxis.Z, 0.0f
	);
}

/*static*/ FTransform4D FTransform4D::MakeTranslation(const FVector3D& InTranslation)
{
	return FTransform4D(
		1.0f, 0.0f, 0.0f, InTranslation.X,
		0.0f, 1.0f, 0.0f, InTranslation.Y,
		0.0f, 0.0f, 1.0f, InTranslation.Z
	);
}

// Arithmetic operations with transforms/vectors.
FTransform4D& FTransform4D::operator*=(const FTransform4D& InTransform)
{
	FTransform4D Result = *this * InTransform;
	*this = Result;
	return *this;
}

FTransform4D FTransform4D::operator*(const FTransform4D& InTransform) const
{
	const float* A = Data;
	const float* B = InTransform.GetData();
	return FTransform4D(
		A[0]*B[0] + A[3]*B[1]  + A[6]*B[2],
		A[0]*B[3] + A[3]*B[4]  + A[6]*B[5],
		A[0]*B[6] + A[3]*B[7]  + A[6]*B[8],
		A[0]*B[9] + A[3]*B[10] + A[6]*B[11] + A[9],
		A[1]*B[0] + A[4]*B[1]  + A[7]*B[2],
		A[1]*B[3] + A[4]*B[4]  + A[7]*B[5],
		A[1]*B[6] + A[4]*B[7]  + A[7]*B[8],
		A[1]*B[9] + A[4]*B[10] + A[7]*B[11] + A[10],
		A[2]*B[0] + A[5]*B[1]  + A[8]*B[2],
		A[2]*B[3] + A[5]*B[4]  + A[8]*B[5],
		A[2]*B[6] + A[5]*B[7]  + A[8]*B[8],
		A[2]*B[9] + A[5]*B[10] + A[8]*B[11] + A[11]
	);
}

FVector4D FTransform4D::operator*(const FVector4D& InVector) const
{
	return FVector4D(
		Data[0]*InVector.X + Data[3]*InVector.Y + Data[6]*InVector.Z + Data[9]*InVector.W,
		Data[1]*InVector.X + Data[4]*InVector.Y + Data[7]*InVector.Z + Data[10]*InVector.W,
		Data[2]*InVector.X + Data[5]*InVector.Y + Data[8]*InVector.Z + Data[11]*InVector.W,
		InVector.W
	);
}

FVector3D FTransform4D::operator*(const FVector3D& InVector) const
{
	return FVector3D(
		Data[0]*InVector.X + Data[3]*InVector.Y + Data[6]*InVector.Z,
		Data[1]*InVector.X + Data[4]*InVector.Y + Data[7]*InVector.Z,
		Data[2]*InVector.X + Data[5]*InVector.Y + Data[8]*InVector.Z
	);
}

FMatrix4D FTransform4D::operator*(const FMatrix4D& InMatrix) const
{
	const float* A = Data;
	const float* B = InMatrix.GetData();
	return FMatrix4D(
		A[0]*B[0]  + A[3]*B[1]  + A[6]*B[2]  + A[9]*B[3],
		A[0]*B[4]  + A[3]*B[5]  + A[6]*B[6]  + A[9]*B[7],
		A[0]*B[8]  + A[3]*B[9]  + A[6]*B[10] + A[9]*B[11],
		A[0]*B[12] + A[3]*B[13] + A[6]*B[14] + A[9]*B[15],
		A[1]*B[0]  + A[4]*B[1]  + A[7]*B[2]  + A[10]*B[3],
		A[1]*B[4]  + A[4]*B[5]  + A[7]*B[6]  + A[10]*B[7],
		A[1]*B[8]  + A[4]*B[9]  + A[7]*B[10] + A[10]*B[11],
		A[1]*B[12] + A[4]*B[13] + A[7]*B[14] + A[10]*B[15],
		A[2]*B[0]  + A[5]*B[1]  + A[8]*B[2]  + A[11]*B[3],
		A[2]*B[4]  + A[5]*B[5]  + A[8]*B[6]  + A[11]*B[7],
		A[2]*B[8]  + A[5]*B[9]  + A[8]*B[10] + A[11]*B[11],
		A[2]*B[12] + A[5]*B[13] + A[8]*B[14] + A[11]*B[15],
		B[3],
		B[7],
		B[11],
		B[15]
	);
}

FMatrix4D operator*(const FMatrix4D& InMatrix, const FTransform4D& InTransform)
{
	const float* A = InMatrix.GetData();
	const float* B = InTransform.GetData();
	return FMatrix4D(
		A[0]*B[0] + A[4]*B[1]  + A[8]*B[2],
		A[0]*B[3] + A[4]*B[4]  + A[8]*B[5],
		A[0]*B[6] + A[4]*B[7]  + A[8]*B[8],
		A[0]*B[9] + A[4]*B[10] + A[8]*B[11] + A[12],
		A[1]*B[0] + A[5]*B[1]  + A[9]*B[2],
		A[1]*B[3] + A[5]*B[4]  + A[9]*B[5],
		A[1]*B[6] + A[5]*B[7]  + A[9]*B[8],
		A[1]*B[9] + A[5]*B[10] + A[9]*B[11] + A[13],
		A[2]*B[0] + A[6]*B[1]  + A[10]*B[2],
		A[2]*B[3] + A[6]*B[4]  + A[10]*B[5],
		A[2]*B[6] + A[6]*B[7]  + A[10]*B[8],
		A[2]*B[9] + A[6]*B[10] + A[10]*B[11] + A[14],
		A[3]*B[0] + A[7]*B[1]  + A[11]*B[2],
		A[3]*B[3] + A[7]*B[4]  + A[11]*B[5],
		A[3]*B[6] + A[7]*B[7]  + A[11]*B[8],
		A[3]*B[9] + A[7]*B[10] + A[11]*B[11] + A[15]
	);
}

// Inverse
void FTransform4D::Invert()
{
	*this = GetInverted();
}

// Implementation of inverse taken from "Foundations of Game Engine Development, Volume 1: Mathematics" (page 80).
FTransform4D FTransform4D::GetInverted() const
{
	const FVector3D& A = (*this)[0];
	const FVector3D& B = (*this)[1];
	const FVector3D& C = (*this)[2];
	const FVector3D& D = (*this)[3];

	FVector3D S = FVector3D::CrossProduct(A, B);
	FVector3D T = FVector3D::CrossProduct(C, D);

	float InverseDeterminant = 1.0f / FVector3D::DotProduct(S, C);

	S *= InverseDeterminant;
	T *= InverseDeterminant;
	FVector3D V = C * InverseDeterminant;

	FVector3D R0 = FVector3D::CrossProduct(B, V);
	FVector3D R1 = FVector3D::CrossProduct(V, A);

	return FTransform4D(
		R0.X, R0.Y, R0.Z, -FVector3D::DotProduct(B, T),
		R1.X, R1.Y, R1.Z, FVector3D::DotProduct(A, T),
		S.X, S.Y, S.Z, -FVector3D::DotProduct(D, S)
	);
}

// Conversion functions.
FMatrix4D FTransform4D::ToMatrix() const
{
	return FMatrix4D(
		Data[0], Data[3], Data[6], Data[9],
		Data[1], Data[4], Data[7], Data[10],
		Data[2], Data[5], Data[8], Data[11],
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

// Equality operators.
bool FTransform4D::operator==(const FTransform4D& InTransform) const 
{
	const float* A = Data;
	const float* B = InTransform.GetData();
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
		&& FMath::IsApproximatelyEqual(A[11], B[11]);
}

bool FTransform4D::operator!=(const FTransform4D& InMatrix) const
{
	return !(*this == InMatrix);
}
