#pragma once

#include "CoreGlobals.h"

class FVector3D;
class FVector4D;
class FMatrix4D;

/**
 * A 3x4 column-major matrix of floating point values, with an implicit 4th row assumed to be [0,0,0,1].
 * Matrix values are accessed as follows: Transform[ColumnIndex][RowIndex].
 */
class FTransform4D
{
public:
	// Constants.
	static const FTransform4D Identity;

	// Default constructor produces the identity matrix.
	FTransform4D();
	FTransform4D(float InValue00, float InValue01, float InValue02, float InValue03,
				 float InValue10, float InValue11, float InValue12, float InValue13,
				 float InValue20, float InValue21, float InValue22, float InValue23);
	FTransform4D(const FVector3D& InColumn0, const FVector3D& InColumn1, const FVector3D& InColumn2, const FVector3D& InColumn3);

	// Copy operations.
	FTransform4D(const FTransform4D& InTransform) = default;
	FTransform4D& operator=(const FTransform4D& InTransform) = default;

	// Accessors.
	FVector3D& operator[](int32 InColumn);
	const FVector3D& operator[](int32 InColumn) const;
	float* GetData();
	const float* GetData() const;
	const FVector3D& GetTranslation() const;
	void SetTranslation(const FVector3D& InTranslation);

	// Transform matrix generation functions.
	static FTransform4D MakeScale(float InScale);
	static FTransform4D MakeScale(const FVector3D& InScale);
	static FTransform4D MakeRotationX(float InRadians);
	static FTransform4D MakeRotationY(float InRadians);
	static FTransform4D MakeRotationZ(float InRadians);
	// InAxis is assumed to be a unit-vector.
	static FTransform4D MakeRotation(float InRadians, const FVector3D& InAxis);
	static FTransform4D MakeTranslation(const FVector3D& InTranslation);

	// Arithmetic operations with transforms/vectors.
	FTransform4D& operator*=(const FTransform4D& InTransform);
	FTransform4D operator*(const FTransform4D& InTransform) const;
	FVector4D operator*(const FVector4D& InVector) const;
	FVector3D operator*(const FVector3D& InVector) const;
	FMatrix4D operator*(const FMatrix4D& InMatrix) const;
	friend FMatrix4D operator*(const FMatrix4D& InMatrix, const FTransform4D& InTransform);

	// Inverse.
	void Invert();
	FTransform4D GetInverted() const;

	// Conversion functions.
	FMatrix4D ToMatrix() const;

	// Equality operators.
	bool operator==(const FTransform4D& InTransform) const;
	bool operator!=(const FTransform4D& InTransform) const;

private:
	/**
	 * We store a 3x4 matrix instead of a 4x4 matrix in order to save space,
	 * where the implicit 4th row of the matrix is assumed to be [0,0,0,1].
	 * Matrix elements are stored in column-major order.
	 */
	float Data[3 * 4];
};
