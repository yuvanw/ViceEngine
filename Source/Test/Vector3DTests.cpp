#include "catch/catch.hpp"

#include "Math/Vector3D.h"
#include "Math/MathUtilities.h"

// Constructors
TEST_CASE("FVector3D default constructor.")
{
	FVector3D Vector;
	REQUIRE(Vector.X == 0.0f);
	REQUIRE(Vector.Y == 0.0f);
	REQUIRE(Vector.Z == 0.0f);
}

TEST_CASE("FVector3D constructor taking three floats.")
{
	SECTION("Data members are initialized to their corresponding argument's value.")
	{
		FVector3D Vector(1.0f, 2.0f, 3.0f);
		REQUIRE(Vector.X == 1.0f);
		REQUIRE(Vector.Y == 2.0f);
		REQUIRE(Vector.Z == 3.0f);
	}

	SECTION("Arguments can be specified via copy-list-initialization syntax.")
	{
		FVector3D Vector = { 1.0f, 2.0f, 3.0f };
		REQUIRE(Vector.X == 1.0f);
		REQUIRE(Vector.Y == 2.0f);
		REQUIRE(Vector.Z == 3.0f);
	}
}

// Copy operations
TEST_CASE("FVector3D copy constructor.")
{
	FVector3D Vector1 = { 1.0f, 2.0f, 3.0f };
	FVector3D Vector2 = Vector1;
	REQUIRE(Vector1 == Vector2);
}

TEST_CASE("FVector3D copy assignment operator.")
{
	FVector3D Vector1 = { 1.0f, 2.0f, 3.0f };
	FVector3D Vector2;
	Vector2 = Vector1;
	REQUIRE(Vector1 == Vector2);
}

// Vector math
TEST_CASE("FVector3D::GetLength")
{
	FVector3D Vector = { 4.0f, 3.0f, 0.0f };
	float LengthSquared = Vector.GetLength();
	REQUIRE(FMath::IsApproximatelyEqual(LengthSquared, 5.0f));
}

TEST_CASE("FVector3D::GetLengthSquared")
{
	FVector3D Vector = { 4.0f, 3.0f, 0.0f };
	float Length = Vector.GetLengthSquared();
	REQUIRE(FMath::IsApproximatelyEqual(Length, 25.0f));
}

TEST_CASE("FVector3D::Normalize")
{
	SECTION("Vectors with zero length are unchanged.")
	{
		FVector3D Zero = { 0.0f, 0.0f, 0.0f };
		Zero.Normalize();
		REQUIRE(Zero.X == 0.0f);
		REQUIRE(Zero.Y == 0.0f);
		REQUIRE(Zero.Z == 0.0f);
	}

	SECTION("Vectors with non-zero length are normalized.")
	{
		FVector3D Vector = { 4.0f, 3.0f, 0.0f };
		float Length = Vector.GetLength();

		FVector3D NormalizedVector = Vector.Normalize();
		REQUIRE(NormalizedVector.X == (4.0f / Length));
		REQUIRE(NormalizedVector.Y == (3.0f / Length));
		REQUIRE(NormalizedVector.Z == (0.0f / Length));
	}
}

TEST_CASE("FVector3D::DotProduct")
{
	FVector3D Vector1 = { 1.0f, 2.0f, 3.0f };
	FVector3D Vector2 = { 2.0f, 3.0f, 4.0f };
	float ActualDotProduct = FVector3D::DotProduct(Vector1, Vector2);
	float ExpectedDotProduct = 20.0f;
	REQUIRE(FMath::IsApproximatelyEqual(ActualDotProduct, ExpectedDotProduct));
}

TEST_CASE("FVector3D::CrossProduct")
{
	FVector3D Vector1 = { 1.0f, 2.0f, 3.0f };
	FVector3D Vector2 = { 2.0f, 3.0f, 4.0f };
	FVector3D ActualCrossProduct = FVector3D::CrossProduct(Vector1, Vector2);
	FVector3D ExpectedCrossProduct = { -1.0f, 2.0f, -1.0f };
	REQUIRE(ActualCrossProduct == ExpectedCrossProduct);
}

// Accessors
TEST_CASE("FVector3D::operator[]")
{
	SECTION("Read and write access is permitted for non-const FVector3D data members.")
	{
		FVector3D Vector = { 1.0f, 2.0f, 3.0f };
		REQUIRE(Vector[0] == 1.0f);
		REQUIRE(Vector[1] == 2.0f);
		REQUIRE(Vector[2] == 3.0f);

		Vector[0] = 4.0f;
		REQUIRE(Vector[0] == 4.0f);
	}

	SECTION("Only read access is permitted for const FVector3D data members.")
	{
		const FVector3D Vector = { 1.0f, 2.0f, 3.0f };
		REQUIRE(Vector[0] == 1.0f);
		REQUIRE(Vector[1] == 2.0f);
		REQUIRE(Vector[2] == 3.0f);
	}
}

// Component-wise operations
TEST_CASE("FVector3D::operator+=(const FVector3D&)")
{
	FVector3D Vector1 = { 1.0f, 2.0f, 3.0f };
	FVector3D Vector2 = { 2.0f, 3.0f, 4.0f };
	Vector1 += Vector2;
	FVector3D ExpectedSum = { 3.0f, 5.0f, 7.0f };
	REQUIRE(Vector1 == ExpectedSum);
}

TEST_CASE("FVector3D::operator-=(const FVector3D&)")
{
	FVector3D Vector1 = { 1.0f, 2.0f, 3.0f };
	FVector3D Vector2 = { 2.0f, 3.0f, 4.0f };
	Vector1 -= Vector2;
	FVector3D ExpectedDifference = { -1.0f, -1.0f, -1.0f };
	REQUIRE(Vector1 == ExpectedDifference);

}

TEST_CASE("FVector3D::operator*=(const FVector3D&)")
{
	FVector3D Vector1 = { 1.0f, 2.0f, 3.0f };
	FVector3D Vector2 = { 2.0f, 3.0f, 4.0f };
	Vector1 *= Vector2;
	FVector3D ExpectedProduct = { 2.0f, 6.0f, 12.0f };
	REQUIRE(Vector1 == ExpectedProduct);
}

TEST_CASE("FVector3D::operator/=(const FVector3D&)")
{
	FVector3D Vector1 = { 1.0f, 2.0f, 3.0f };
	FVector3D Vector2 = { 1.0f, 2.0f, 3.0f };
	Vector1 /= Vector2;
	FVector3D ExpectedQuotient = { 1.0f, 1.0f, 1.0f };
	REQUIRE(Vector1 == ExpectedQuotient);
}

TEST_CASE("FVector3D::operator+(const FVector3D&)")
{
	FVector3D Vector1 = { 1.0f, 2.0f, 3.0f };
	FVector3D Vector2 = { 2.0f, 3.0f, 4.0f };
	FVector3D ActualSum = Vector1 + Vector2;
	FVector3D ExpectedSum = { 3.0f, 5.0f, 7.0f };
	REQUIRE(ActualSum == ExpectedSum);
}

TEST_CASE("FVector3D::operator-(const FVector3D&)")
{
	FVector3D Vector1 = { 1.0f, 2.0f, 3.0f };
	FVector3D Vector2 = { 2.0f, 3.0f, 4.0f };
	FVector3D ActualDifference = Vector1 - Vector2;
	FVector3D ExpectedDifference = { -1.0f, -1.0f, -1.0f };
	REQUIRE(ActualDifference == ExpectedDifference);
}

TEST_CASE("FVector3D::operator*(const FVector3D&)")
{
	FVector3D Vector1 = { 1.0f, 2.0f, 3.0f };
	FVector3D Vector2 = { 2.0f, 3.0f, 4.0f };
	FVector3D ActualProduct = Vector1 * Vector2;
	FVector3D ExpectedProduct = { 2.0f, 6.0f, 12.0f };
	REQUIRE(ActualProduct == ExpectedProduct);
}

TEST_CASE("FVector3D::operator/(const FVector3D&)")
{
	FVector3D Vector1 = { 1.0f, 2.0f, 3.0f };
	FVector3D Vector2 = { 1.0f, 2.0f, 3.0f };
	FVector3D ActualQuotient = Vector1 / Vector2;
	FVector3D ExpectedQuotient = { 1.0f, 1.0f, 1.0f };
	REQUIRE(ActualQuotient == ExpectedQuotient);
}

// Scalar operations
TEST_CASE("FVector3D::operator+=(float)")
{
	FVector3D Vector = { 1.0f, 2.0f, 3.0f };
	Vector += 2.0f;
	FVector3D ExpectedSum = { 3.0f, 4.0f, 5.0f };
	REQUIRE(Vector == ExpectedSum);
}

TEST_CASE("FVector3D::operator-=(float)")
{
	FVector3D Vector = { 1.0f, 2.0f, 3.0f };
	Vector -= 2.0f;
	FVector3D ExpectedSum = { -1.0f, 0.0f, 1.0f };
	REQUIRE(Vector == ExpectedSum);
}

TEST_CASE("FVector3D::operator*=(float)")
{
	FVector3D Vector = { 1.0f, 2.0f, 3.0f };
	Vector *= 2.0f;
	FVector3D ExpectedProduct = { 2.0f, 4.0f, 6.0f };
	REQUIRE(Vector == ExpectedProduct);
}

TEST_CASE("FVector3D::operator/=(float)")
{
	FVector3D Vector = { 1.0f, 2.0f, 3.0f };
	Vector /= 2.0f;
	FVector3D ExpectedQuotient = { 0.5f, 1.0f, 1.5f };
	REQUIRE(Vector == ExpectedQuotient);
}

TEST_CASE("FVector3D::operator+(float)")
{
	FVector3D Vector = { 1.0f, 2.0f, 3.0f };
	FVector3D ActualSum = Vector + 2.0f;
	FVector3D ExpectedSum = { 3.0f, 4.0f, 5.0f };
	REQUIRE(ActualSum == ExpectedSum);
}

TEST_CASE("FVector3D::operator-(float)")
{
	FVector3D Vector = { 1.0f, 2.0f, 3.0f };
	FVector3D ActualDifference = Vector - 2.0f;
	FVector3D ExpectedDifference = { -1.0f, 0.0f, 1.0f };
	REQUIRE(ActualDifference == ExpectedDifference);
}

TEST_CASE("FVector3D::operator*(float)")
{
	FVector3D Vector = { 1.0f, 2.0f, 3.0f };
	FVector3D ActualProduct = Vector * 2.0f;
	FVector3D ExpectedProduct = { 2.0f, 4.0f, 6.0f };
	REQUIRE(ActualProduct == ExpectedProduct);
}

TEST_CASE("FVector3D::operator/(float)")
{
	FVector3D Vector = { 1.0f, 2.0f, 3.0f };
	FVector3D ActualQuotient = Vector / 2.0f;
	FVector3D ExpectedQuotient = { 0.5f, 1.0f, 1.5f };
	REQUIRE(ActualQuotient == ExpectedQuotient);
}

TEST_CASE("operator*(float, const FVector3D&)")
{
	FVector3D Vector = { 1.0f, 2.0f, 3.0f };
	FVector3D ActualProduct = 2.0f * Vector;
	FVector3D ExpectedProduct = { 2.0f, 4.0f, 6.0f };
	REQUIRE(ActualProduct == ExpectedProduct);
}

// Equality operators
TEST_CASE("FVector3D::operator==")
{
	FVector3D Vector1 = { 1.0f, 2.0f, 3.0f };
	FVector3D Vector2 = { 1.0f, 2.0f, 3.0f };
	REQUIRE(Vector1 == Vector2);
}

TEST_CASE("FVector3D::operator!=")
{
	FVector3D Vector1 = { 1.0f, 2.0f, 3.0f };
	FVector3D Vector2 = { 2.0f, 3.0f, 4.0f };
	REQUIRE(Vector1 != Vector2);
}
