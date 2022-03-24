#pragma once

#include "CoreMinimal.h"

class FVector4D;

/**
 * A 4x4 column-major matrix of floating point values.
 * Matrix values are accessed as follows: Matrix[ColumnIndex][RowIndex].
 */
class FMatrix4D
{
public:
	// Constants.
	static const FMatrix4D Zero;
	static const FMatrix4D Identity;

	// Constructors
	FMatrix4D() = default;
	FMatrix4D(float InValue00, float InValue01, float InValue02, float InValue03,
			  float InValue10, float InValue11, float InValue12, float InValue13,
			  float InValue20, float InValue21, float InValue22, float InValue23,
			  float InValue30, float InValue31, float InValue32, float InValue33);

	// Copy operations
	FMatrix4D(const FMatrix4D&) = default;
	FMatrix4D& operator=(const FMatrix4D&) = default;

	// Accessors
	FVector4D& operator[](int32 InColumn);
	const FVector4D& operator[](int32 InColumn) const;
	float* GetData();
	const float* GetData() const;

	// Arithmetic operations.
	FVector4D operator*(const FVector4D& InVector) const;

	// Equality operators.
	bool operator==(const FMatrix4D& InMatrix) const;
	bool operator!=(const FMatrix4D& InMatrix) const;

private:
	float Data[4 * 4] = {0};
};
