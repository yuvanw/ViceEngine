#include "catch/catch.hpp"

#include "Math/Transform4D.h"
#include "Math/Vector3D.h"
#include "Math/Vector4D.h"
#include "Math/Matrix4D.h"
#include "Math/MathUtilities.h"

// Constructors
TEST_CASE("FTransform4D default constructor.")
{
	FTransform4D Transform;
	float* Data = Transform.GetData();
	
	REQUIRE(Data[0] == 1);
	REQUIRE(Data[1] == 0);
	REQUIRE(Data[2] == 0);
	REQUIRE(Data[3] == 0);
	REQUIRE(Data[4] == 1);
	REQUIRE(Data[5] == 0);
	REQUIRE(Data[6] == 0);
	REQUIRE(Data[7] == 0);
	REQUIRE(Data[8] == 1);
	REQUIRE(Data[9] == 0);
	REQUIRE(Data[10] == 0);
	REQUIRE(Data[11] == 0);
}

TEST_CASE("FTransform4D constructor taking 12 floats.")
{
	FTransform4D Transform(
		0,1,2,3,
		4,5,6,7,
		8,9,10,11
	);
	float* Data = Transform.GetData();
	
	REQUIRE(Data[0] == 0);
	REQUIRE(Data[1] == 4);
	REQUIRE(Data[2] == 8);
	REQUIRE(Data[3] == 1);
	REQUIRE(Data[4] == 5);
	REQUIRE(Data[5] == 9);
	REQUIRE(Data[6] == 2);
	REQUIRE(Data[7] == 6);
	REQUIRE(Data[8] == 10);
	REQUIRE(Data[9] == 3);
	REQUIRE(Data[10] == 7);
	REQUIRE(Data[11] == 11);
}

TEST_CASE("FTransform4D constructor taking 4 column vectors.")
{
	FTransform4D Transform(
		FVector3D(0, 1, 2),
		FVector3D(3, 4, 5),
		FVector3D(6, 7, 8),
		FVector3D(9, 10, 11)
	);
	float* Data = Transform.GetData();

	REQUIRE(Data[0] == 0);
	REQUIRE(Data[1] == 1);
	REQUIRE(Data[2] == 2);
	REQUIRE(Data[3] == 3);
	REQUIRE(Data[4] == 4);
	REQUIRE(Data[5] == 5);
	REQUIRE(Data[6] == 6);
	REQUIRE(Data[7] == 7);
	REQUIRE(Data[8] == 8);
	REQUIRE(Data[9] == 9);
	REQUIRE(Data[10] == 10);
	REQUIRE(Data[11] == 11);
}

// Copy operations
TEST_CASE("FTransform4D copy constructor.")
{
	FTransform4D Transform(
		0, 1, 2, 3,
		4, 5, 6, 7,
		8, 9, 10, 11
	);
	FTransform4D TransformCopy = Transform;
	float* Data = TransformCopy.GetData();

	REQUIRE(Data[0] == 0);
	REQUIRE(Data[1] == 4);
	REQUIRE(Data[2] == 8);
	REQUIRE(Data[3] == 1);
	REQUIRE(Data[4] == 5);
	REQUIRE(Data[5] == 9);
	REQUIRE(Data[6] == 2);
	REQUIRE(Data[7] == 6);
	REQUIRE(Data[8] == 10);
	REQUIRE(Data[9] == 3);
	REQUIRE(Data[10] == 7);
	REQUIRE(Data[11] == 11);
}

TEST_CASE("FTransform4D copy assignment operator.")
{
	FTransform4D Transform(
		0, 1, 2, 3,
		4, 5, 6, 7,
		8, 9, 10, 11
	);
	FTransform4D TransformCopy;
	TransformCopy = Transform;
	float* Data = TransformCopy.GetData();

	REQUIRE(Data[0] == 0);
	REQUIRE(Data[1] == 4);
	REQUIRE(Data[2] == 8);
	REQUIRE(Data[3] == 1);
	REQUIRE(Data[4] == 5);
	REQUIRE(Data[5] == 9);
	REQUIRE(Data[6] == 2);
	REQUIRE(Data[7] == 6);
	REQUIRE(Data[8] == 10);
	REQUIRE(Data[9] == 3);
	REQUIRE(Data[10] == 7);
	REQUIRE(Data[11] == 11);
}

// Accessors
TEST_CASE("FTransform4D::operator[]")
{
	SECTION("Read and write access is permitted for non-const FTransform4D data.")
	{
		FTransform4D Transform;
		FVector3D Column0 = Transform[0];
		FVector3D Column1 = Transform[1];
		FVector3D Column2 = Transform[2];
		FVector3D Column3 = Transform[3];

		REQUIRE(Column0 == FVector3D(1, 0, 0));
		REQUIRE(Column1 == FVector3D(0, 1, 0));
		REQUIRE(Column2 == FVector3D(0, 0, 1));
		REQUIRE(Column3 == FVector3D(0, 0, 0));

		Transform[0] = FVector3D(4,4,4);
		float* Data = Transform.GetData();
		REQUIRE(Data[0] == 4);
		REQUIRE(Data[1] == 4);
		REQUIRE(Data[2] == 4);
		REQUIRE(Data[3] == 0);
		REQUIRE(Data[4] == 1);
		REQUIRE(Data[5] == 0);
		REQUIRE(Data[6] == 0);
		REQUIRE(Data[7] == 0);
		REQUIRE(Data[8] == 1);
		REQUIRE(Data[9] == 0);
		REQUIRE(Data[10] == 0);
		REQUIRE(Data[11] == 0);
	}

	SECTION("Only read access is permitted for const FTransform4D data members.")
	{
		const FTransform4D Transform;
		FVector3D Column0 = Transform[0];
		FVector3D Column1 = Transform[1];
		FVector3D Column2 = Transform[2];
		FVector3D Column3 = Transform[3];

		REQUIRE(Column0 == FVector3D(1, 0, 0));
		REQUIRE(Column1 == FVector3D(0, 1, 0));
		REQUIRE(Column2 == FVector3D(0, 0, 1));
		REQUIRE(Column3 == FVector3D(0, 0, 0));
	}
}

TEST_CASE("FTransform4D::GetTranslation")
{
	FTransform4D Transform(
		0, 1, 2, 3,
		4, 5, 6, 7,
		8, 9, 10, 11
	);
	FVector3D Translation = Transform.GetTranslation();
	REQUIRE(Translation == FVector3D(3, 7, 11));
}

TEST_CASE("FTransform4D::SetTranslation")
{
	FTransform4D Transform(
		0, 1, 2, 3,
		4, 5, 6, 7,
		8, 9, 10, 11
	);
	FVector3D Translation = { 12, 13, 14 };
	Transform.SetTranslation(Translation);
	REQUIRE(Transform.GetTranslation()  == Translation);
}

// Transform matrix generation functions
TEST_CASE("FTransform4D::MakeScale")
{
	SECTION("Uniform scale along all three axes.")
	{
		FTransform4D ScaleTransform = FTransform4D::MakeScale(2.0f);
		float* Data = ScaleTransform.GetData();

		REQUIRE(Data[0] == 2.0f);
		REQUIRE(Data[1] == 0.0f);
		REQUIRE(Data[2] == 0.0f);
		REQUIRE(Data[3] == 0.0f);
		REQUIRE(Data[4] == 2.0f);
		REQUIRE(Data[5] == 0.0f);
		REQUIRE(Data[6] == 0.0f);
		REQUIRE(Data[7] == 0.0f);
		REQUIRE(Data[8] == 2.0f);
		REQUIRE(Data[9] == 0.0f);
		REQUIRE(Data[10] == 0.0f);
		REQUIRE(Data[11] == 0.0f);
	}

	SECTION("Specify scale for each axis.")
	{
		FTransform4D ScaleTransform = FTransform4D::MakeScale({ 1.0f, 2.0f, 3.0f });
		float* Data = ScaleTransform.GetData();

		REQUIRE(Data[0] == 1.0f);
		REQUIRE(Data[1] == 0.0f);
		REQUIRE(Data[2] == 0.0f);
		REQUIRE(Data[3] == 0.0f);
		REQUIRE(Data[4] == 2.0f);
		REQUIRE(Data[5] == 0.0f);
		REQUIRE(Data[6] == 0.0f);
		REQUIRE(Data[7] == 0.0f);
		REQUIRE(Data[8] == 3.0f);
		REQUIRE(Data[9] == 0.0f);
		REQUIRE(Data[10] == 0.0f);
		REQUIRE(Data[11] == 0.0f);
	}
}

TEST_CASE("FTransform4D::MakeRotationX")
{
	float Radians = FMath::DegreesToRadians(90);
	FTransform4D RotationXTransform = FTransform4D::MakeRotationX(Radians);
	float* Data = RotationXTransform.GetData();

	REQUIRE(Data[0] == 1.0f);
	REQUIRE(Data[1] == 0.0f);
	REQUIRE(Data[2] == 0.0f);
	REQUIRE(Data[3] == 0.0f);
	REQUIRE(FMath::IsApproximatelyZero(Data[4]));
	REQUIRE(FMath::IsApproximatelyEqual(Data[5], 1.0f));
	REQUIRE(Data[6] == 0.0f);
	REQUIRE(FMath::IsApproximatelyEqual(Data[7], -1.0f));
	REQUIRE(FMath::IsApproximatelyZero(Data[8]));
	REQUIRE(Data[9] == 0.0f);
	REQUIRE(Data[10] == 0.0f);
	REQUIRE(Data[11] == 0.0f);
}

TEST_CASE("FTransform4D::MakeRotationY")
{
	float Radians = FMath::DegreesToRadians(90);
	FTransform4D RotationYTransform = FTransform4D::MakeRotationY(Radians);
	float* Data = RotationYTransform.GetData();

	REQUIRE(FMath::IsApproximatelyZero(Data[0]));
	REQUIRE(Data[1] == 0.0f);
	REQUIRE(FMath::IsApproximatelyEqual(Data[2], -1.0f));
	REQUIRE(Data[3] == 0.0f);
	REQUIRE(Data[4] == 1.0f);
	REQUIRE(Data[5] == 0.0f);
	REQUIRE(FMath::IsApproximatelyEqual(Data[6], 1.0f));
	REQUIRE(Data[7] == 0.0f);
	REQUIRE(FMath::IsApproximatelyZero(Data[8]));
	REQUIRE(Data[9] == 0.0f);
	REQUIRE(Data[10] == 0.0f);
	REQUIRE(Data[11] == 0.0f);
}

TEST_CASE("FTransform4D::MakeRotationZ")
{
	float Radians = FMath::DegreesToRadians(90);
	FTransform4D RotationZTransform = FTransform4D::MakeRotationZ(Radians);
	float* Data = RotationZTransform.GetData();

	REQUIRE(FMath::IsApproximatelyZero(Data[0]));
	REQUIRE(FMath::IsApproximatelyEqual(Data[1], 1.0f));
	REQUIRE(Data[2] == 0.0f);
	REQUIRE(FMath::IsApproximatelyEqual(Data[3], -1.0f));
	REQUIRE(FMath::IsApproximatelyZero(Data[4]));
	REQUIRE(Data[5] == 0.0f);
	REQUIRE(Data[6] == 0.0f);
	REQUIRE(Data[7] == 0.0f);
	REQUIRE(Data[8] == 1.0f);
	REQUIRE(Data[9] == 0.0f);
	REQUIRE(Data[10] == 0.0f);
	REQUIRE(Data[11] == 0.0f);
}

TEST_CASE("FTransform4D::MakeTranslation")
{
	FVector3D Translation(1.0f, 2.0f, 3.0f);
	FTransform4D TranslationTransform = FTransform4D::MakeTranslation(Translation);
	float* Data = TranslationTransform.GetData();

	REQUIRE(Data[0] == 1.0f);
	REQUIRE(Data[1] == 0.0f);
	REQUIRE(Data[2] == 0.0f);
	REQUIRE(Data[3] == 0.0f);
	REQUIRE(Data[4] == 1.0f);
	REQUIRE(Data[5] == 0.0f);
	REQUIRE(Data[6] == 0.0f);
	REQUIRE(Data[7] == 0.0f);
	REQUIRE(Data[8] == 1.0f);
	REQUIRE(Data[9] == 1.0f);
	REQUIRE(Data[10] == 2.0f);
	REQUIRE(Data[11] == 3.0f);
}

// Arithmetic operations with transforms/vectors
TEST_CASE("FTransform4D::operator*=(const FTransform4D&)")
{
	FTransform4D Transform(
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f,
		8.0f, 9.0f, 10.0f, 11.0f
	);
	Transform *= Transform;
	float* Data = Transform.GetData();

	REQUIRE(Data[0] == 20.0f);
	REQUIRE(Data[1] == 68.0f);
	REQUIRE(Data[2] == 116.0f);
	REQUIRE(Data[3] == 23.0f);
	REQUIRE(Data[4] == 83.0f);
	REQUIRE(Data[5] == 143.0f);
	REQUIRE(Data[6] == 26.0f);
	REQUIRE(Data[7] == 98.0f);
	REQUIRE(Data[8] == 170.0f);
	REQUIRE(Data[9] == 32.0f);
	REQUIRE(Data[10] == 120.0f);
	REQUIRE(Data[11] == 208.0f);
}

TEST_CASE("FTransform4D::operator*(const FTransform4D&)")
{
	FTransform4D Transform(
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f,
		8.0f, 9.0f, 10.0f, 11.0f
	);
	FTransform4D Product = Transform * Transform;
	float* Data = Product.GetData();

	REQUIRE(Data[0] == 20.0f);
	REQUIRE(Data[1] == 68.0f);
	REQUIRE(Data[2] == 116.0f);
	REQUIRE(Data[3] == 23.0f);
	REQUIRE(Data[4] == 83.0f);
	REQUIRE(Data[5] == 143.0f);
	REQUIRE(Data[6] == 26.0f);
	REQUIRE(Data[7] == 98.0f);
	REQUIRE(Data[8] == 170.0f);
	REQUIRE(Data[9] == 32.0f);
	REQUIRE(Data[10] == 120.0f);
	REQUIRE(Data[11] == 208.0f);
}

TEST_CASE("FTransform4D::operator*(const FVector4D&)")
{
	FTransform4D Transform(
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f,
		8.0f, 9.0f, 10.0f, 11.0f
	);
	FVector4D Vector(1.0f, 2.0f, 3.0f, 1.0f);
	FVector4D Product = Transform * Vector;

	REQUIRE(Product.X == 11.0f);
	REQUIRE(Product.Y == 39.0f);
	REQUIRE(Product.Z == 67.0f);
	REQUIRE(Product.W == 1.0f);
}

TEST_CASE("FTransform4D::operator*(const FVector3D&)")
{
	FTransform4D Transform(
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f,
		8.0f, 9.0f, 10.0f, 11.0f
	);
	FVector3D Vector(1.0f, 2.0f, 3.0f);
	FVector3D Product = Transform * Vector;

	REQUIRE(Product.X == 8.0f);
	REQUIRE(Product.Y == 32.0f);
	REQUIRE(Product.Z == 56.0f);
}

TEST_CASE("FTransform4D::operator*(const FMatrix4D&)")
{
	FTransform4D Transform(
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f,
		8.0f, 9.0f, 10.0f, 11.0f
	);
	FMatrix4D Matrix(
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f,
		8.0f, 9.0f, 10.0f, 11.0f,
		12.0f, 13.0f, 14.0f, 15.0f
	);
	FMatrix4D ExpectedProduct
	(
		56.0f, 62.0f, 68.0f, 74.0f,
		152.0f, 174.0f, 196.0f, 218.0f,
		248.0f, 286.0f, 324.0f, 362.0f,
		12.0f, 13.0f, 14.0f, 15.0f
	);

	FMatrix4D Product = Transform * Matrix;
	REQUIRE(Product == ExpectedProduct);
}

TEST_CASE("operator*(const FMatrix4D&, const FTransform4D&)")
{
	FMatrix4D Matrix(
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f,
		8.0f, 9.0f, 10.0f, 11.0f,
		12.0f, 13.0f, 14.0f, 15.0f
	);
	FTransform4D Transform(
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f,
		8.0f, 9.0f, 10.0f, 11.0f
	);
	FMatrix4D ExpectedProduct(
		20.0f, 23.0f, 26.0f, 32.0f,
		68.0f, 83.0f, 98.0f, 120.0f,
		116.0f, 143.0f, 170.0f, 208.0f,
		164.0f, 203.0f, 242.0f, 296.0f
	);

	FMatrix4D Product = Matrix * Transform;
	REQUIRE(Product == ExpectedProduct);
}

// Inverse
TEST_CASE("FTransform inverse.")
{
	float Radians = FMath::DegreesToRadians(45.0f);
	FTransform4D Scale = FTransform4D::MakeScale(2.0f);
	FTransform4D RotationX = FTransform4D::MakeRotationX(Radians);
	FTransform4D RotationY = FTransform4D::MakeRotationY(Radians);
	FTransform4D RotationZ = FTransform4D::MakeRotationZ(Radians);
	FTransform4D Translation = FTransform4D::MakeTranslation({ 1.0f, 2.0f, 3.0f });
	FTransform4D Transform = Scale * RotationX * RotationY * RotationZ * Translation;

	FTransform4D InverseScale = FTransform4D::MakeScale(0.5f);
	FTransform4D InverseRotationX = FTransform4D::MakeRotationX(-Radians);
	FTransform4D InverseRotationY = FTransform4D::MakeRotationY(-Radians);
	FTransform4D InverseRotationZ = FTransform4D::MakeRotationZ(-Radians);
	FTransform4D InverseTranslation = FTransform4D::MakeTranslation({ -1.0f, -2.0f, -3.0f });
	FTransform4D ExpectedInverse = InverseTranslation * InverseRotationZ * InverseRotationY * InverseRotationX * InverseScale;

	SECTION("FTransform::Invert")
	{
		Transform.Invert();
		REQUIRE(Transform == ExpectedInverse);
	}

	SECTION("FTransform::GetInverted")
	{
		FTransform4D Inverse = Transform.GetInverted();
		REQUIRE(Inverse == ExpectedInverse);
	}
}

// Conversion functions
TEST_CASE("FTransform4D::ToMatrix")
{
	FTransform4D Transform(
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f,
		8.0f, 9.0f, 10.0f, 11.0f
	);
	FMatrix4D ExpectedMatrix(
		// 3x4 matrix data from FTransform4D
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f,
		8.0f, 9.0f, 10.0f, 11.0f,
		// Implict 4th row of FTransform4D
		0.0f, 0.0f, 0.0f, 1.0f
	);
	FMatrix4D Matrix = Transform.ToMatrix();
	REQUIRE(Matrix == ExpectedMatrix);
}

// Equality operators
TEST_CASE("FTransform4D::operator==(const FTransform4D&)")
{
	FTransform4D Transform1(
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f,
		8.0f, 9.0f, 10.0f, 11.0f
	);
	FTransform4D Transform2(
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f,
		8.0f, 9.0f, 10.0f, 11.0f
	);
	REQUIRE(Transform1 == Transform2);
}

// Equality operators
TEST_CASE("FTransform4D::operator!=(const FTransform4D&)")
{
	FTransform4D Transform1(
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f,
		8.0f, 9.0f, 10.0f, 11.0f
	);
	FTransform4D Transform2(
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f,
		8.0f, 9.0f, 10.0f, 999999999.0f
	);
	REQUIRE(Transform1 != Transform2);
}
