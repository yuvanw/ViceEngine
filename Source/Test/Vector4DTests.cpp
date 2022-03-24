#include "catch/catch.hpp"

#include "Math/Vector4D.h"
#include "Math/MathUtilities.h"

// Constructors
TEST_CASE("FVector4D default constructor.")
{
	FVector4D Vector;
	REQUIRE(Vector.X == 0.0f);
	REQUIRE(Vector.Y == 0.0f);
	REQUIRE(Vector.Z == 0.0f);
	REQUIRE(Vector.W == 0.0f);
}

TEST_CASE("FVector4D constructor taking three floats.")
{
	SECTION("Data members are initialized to their corresponding argument's value.")
	{
		FVector4D Vector(1.0f, 2.0f, 3.0f, 1.0f);
		REQUIRE(Vector.X == 1.0f);
		REQUIRE(Vector.Y == 2.0f);
		REQUIRE(Vector.Z == 3.0f);
		REQUIRE(Vector.W == 1.0f);
	}

	SECTION("Arguments can be specified via copy-list-initialization syntax.")
	{
		FVector4D Vector = { 1.0f, 2.0f, 3.0f, 1.0f };
		REQUIRE(Vector.X == 1.0f);
		REQUIRE(Vector.Y == 2.0f);
		REQUIRE(Vector.Z == 3.0f);
		REQUIRE(Vector.W == 1.0f);
	}
}

// Copy operations
TEST_CASE("FVector4D copy constructor.")
{
	FVector4D Vector1 = { 1.0f, 2.0f, 3.0f, 1.0f };
	FVector4D Vector2 = Vector1;
	REQUIRE(Vector1 == Vector2);
}

TEST_CASE("FVector4D copy assignment operator.")
{
	FVector4D Vector1 = { 1.0f, 2.0f, 3.0f, 1.0f };
	FVector4D Vector2;
	Vector2 = Vector1;
	REQUIRE(Vector1 == Vector2);
}

// Vector math
TEST_CASE("FVector4D::GetLength")
{
	FVector4D Vector = { 2.0f, 2.0f, 0.0f, 1.0f };
	float LengthSquared = Vector.GetLength();
	REQUIRE(FMath::IsApproximatelyEqual(LengthSquared, 3.0f));
}

TEST_CASE("FVector4D::GetLengthSquared")
{
	FVector4D Vector = { 2.0f, 2.0f, 0.0f, 1.0f };
	float Length = Vector.GetLengthSquared();
	REQUIRE(FMath::IsApproximatelyEqual(Length, 9.0f));
}

TEST_CASE("FVector4D::Normalize")
{
	SECTION("Vectors with zero length are unchanged.")
	{
		FVector4D Zero = { 0.0f, 0.0f, 0.0f, 0.0f };
		Zero.Normalize();
		REQUIRE(Zero.X == 0.0f);
		REQUIRE(Zero.Y == 0.0f);
		REQUIRE(Zero.Z == 0.0f);
		REQUIRE(Zero.W == 0.0f);
	}

	SECTION("Vectors with non-zero length are normalized.")
	{
		FVector4D Vector = { 4.0f, 3.0f, 0.0f, 0.0f };
		float Length = Vector.GetLength();

		FVector4D NormalizedVector = Vector.Normalize();
		REQUIRE(NormalizedVector.X == (4.0f / Length));
		REQUIRE(NormalizedVector.Y == (3.0f / Length));
		REQUIRE(NormalizedVector.Z == (0.0f / Length));
		REQUIRE(NormalizedVector.Z == (0.0f / Length));
	}
}

TEST_CASE("FVector4D::DotProduct")
{
	FVector4D Vector1 = { 1.0f, 2.0f, 3.0f, 1.0f };
	FVector4D Vector2 = { 2.0f, 3.0f, 4.0f, 1.0f };
	float ActualDotProduct = FVector4D::DotProduct(Vector1, Vector2);
	float ExpectedDotProduct = 21.0f;
	REQUIRE(FMath::IsApproximatelyEqual(ActualDotProduct, ExpectedDotProduct));
}

TEST_CASE("FVector4D::CrossProduct")
{
	FVector4D Vector1 = { 1.0f, 2.0f, 3.0f, 1.0f };
	FVector4D Vector2 = { 2.0f, 3.0f, 4.0f, 1.0f };
	FVector4D ActualCrossProduct = FVector4D::CrossProduct(Vector1, Vector2);
	// Cross product is calculated using X, Y, and Z, and W is set to zero.
	FVector4D ExpectedCrossProduct = { -1.0f, 2.0f, -1.0f, 0.0f };
	REQUIRE(ActualCrossProduct == ExpectedCrossProduct);
}

// Accessors
TEST_CASE("FVector4D::operator[]")
{
	SECTION("Read and write access is permitted for non-const FVector4D data members.")
	{
		FVector4D Vector = { 1.0f, 2.0f, 3.0f, 1.0f };
		REQUIRE(Vector[0] == 1.0f);
		REQUIRE(Vector[1] == 2.0f);
		REQUIRE(Vector[2] == 3.0f);
		REQUIRE(Vector[3] == 1.0f);

		Vector[0] = 4.0f;
		REQUIRE(Vector[0] == 4.0f);
	}

	SECTION("Only read access is permitted for const FVector4D data members.")
	{
		const FVector4D Vector = { 1.0f, 2.0f, 3.0f, 1.0f };
		REQUIRE(Vector[0] == 1.0f);
		REQUIRE(Vector[1] == 2.0f);
		REQUIRE(Vector[2] == 3.0f);
		REQUIRE(Vector[3] == 1.0f);
	}
}

// Component-wise operations
TEST_CASE("FVector4D::operator+=(const FVector4D&)")
{
	FVector4D Vector1 = { 1.0f, 2.0f, 3.0f, 1.0f };
	FVector4D Vector2 = { 2.0f, 3.0f, 4.0f, 0.0f };
	Vector1 += Vector2;
	FVector4D ExpectedSum = { 3.0f, 5.0f, 7.0f, 1.0f };
	REQUIRE(Vector1 == ExpectedSum);
}

TEST_CASE("FVector4D::operator-=(const FVector4D&)")
{
	FVector4D Vector1 = { 1.0f, 2.0f, 3.0f, 1.0f };
	FVector4D Vector2 = { 2.0f, 3.0f, 4.0f, 0.0f };
	Vector1 -= Vector2;
	FVector4D ExpectedDifference = { -1.0f, -1.0f, -1.0f, 1.0f };
	REQUIRE(Vector1 == ExpectedDifference);

}

TEST_CASE("FVector4D::operator*=(const FVector4D&)")
{
	FVector4D Vector1 = { 1.0f, 2.0f, 3.0f, 1.0f };
	FVector4D Vector2 = { 2.0f, 3.0f, 4.0f, 0.0f };
	Vector1 *= Vector2;
	FVector4D ExpectedProduct = { 2.0f, 6.0f, 12.0f, 0.0f };
	REQUIRE(Vector1 == ExpectedProduct);
}

TEST_CASE("FVector4D::operator/=(const FVector4D&)")
{
	FVector4D Vector1 = { 1.0f, 2.0f, 3.0f, 1.0f };
	FVector4D Vector2 = { 1.0f, 2.0f, 3.0f, 1.0f };
	Vector1 /= Vector2;
	FVector4D ExpectedQuotient = { 1.0f, 1.0f, 1.0f, 1.0f };
	REQUIRE(Vector1 == ExpectedQuotient);
}

TEST_CASE("FVector4D::operator+(const FVector4D&)")
{
	FVector4D Vector1 = { 1.0f, 2.0f, 3.0f, 1.0f };
	FVector4D Vector2 = { 2.0f, 3.0f, 4.0f, 0.0f };
	FVector4D ActualSum = Vector1 + Vector2;
	FVector4D ExpectedSum = { 3.0f, 5.0f, 7.0f, 1.0f };
	REQUIRE(ActualSum == ExpectedSum);
}

TEST_CASE("FVector4D::operator-(const FVector4D&)")
{
	FVector4D Vector1 = { 1.0f, 2.0f, 3.0f, 1.0f };
	FVector4D Vector2 = { 2.0f, 3.0f, 4.0f, 0.0f };
	FVector4D ActualDifference = Vector1 - Vector2;
	FVector4D ExpectedDifference = { -1.0f, -1.0f, -1.0f, 1.0f };
	REQUIRE(ActualDifference == ExpectedDifference);
}

TEST_CASE("FVector4D::operator*(const FVector4D&)")
{
	FVector4D Vector1 = { 1.0f, 2.0f, 3.0f, 1.0f };
	FVector4D Vector2 = { 2.0f, 3.0f, 4.0f, 0.0f };
	FVector4D ActualProduct = Vector1 * Vector2;
	FVector4D ExpectedProduct = { 2.0f, 6.0f, 12.0f, 0.0f };
	REQUIRE(ActualProduct == ExpectedProduct);
}

TEST_CASE("FVector4D::operator/(const FVector4D&)")
{
	FVector4D Vector1 = { 1.0f, 2.0f, 3.0f, 0.0f };
	FVector4D Vector2 = { 1.0f, 2.0f, 3.0f, 1.0f };
	FVector4D ActualQuotient = Vector1 / Vector2;
	FVector4D ExpectedQuotient = { 1.0f, 1.0f, 1.0f, 0.0f };
	REQUIRE(ActualQuotient == ExpectedQuotient);
}

// Scalar operations
TEST_CASE("FVector4D::operator+=(float)")
{
	FVector4D Vector = { 1.0f, 2.0f, 3.0f, 1.0f };
	Vector += 2.0f;
	FVector4D ExpectedSum = { 3.0f, 4.0f, 5.0f, 3.0f };
	REQUIRE(Vector == ExpectedSum);
}

TEST_CASE("FVector4D::operator-=(float)")
{
	FVector4D Vector = { 1.0f, 2.0f, 3.0f, 1.0f };
	Vector -= 2.0f;
	FVector4D ExpectedSum = { -1.0f, 0.0f, 1.0f, -1.0f };
	REQUIRE(Vector == ExpectedSum);
}

TEST_CASE("FVector4D::operator*=(float)")
{
	FVector4D Vector = { 1.0f, 2.0f, 3.0f, 1.0f };
	Vector *= 2.0f;
	FVector4D ExpectedProduct = { 2.0f, 4.0f, 6.0f, 2.0f };
	REQUIRE(Vector == ExpectedProduct);
}

TEST_CASE("FVector4D::operator/=(float)")
{
	FVector4D Vector = { 1.0f, 2.0f, 3.0f, 1.0f };
	Vector /= 2.0f;
	FVector4D ExpectedQuotient = { 0.5f, 1.0f, 1.5f, 0.5f };
	REQUIRE(Vector == ExpectedQuotient);
}

TEST_CASE("FVector4D::operator+(float)")
{
	FVector4D Vector = { 1.0f, 2.0f, 3.0f, 1.0f };
	FVector4D ActualSum = Vector + 2.0f;
	FVector4D ExpectedSum = { 3.0f, 4.0f, 5.0f, 3.0f };
	REQUIRE(ActualSum == ExpectedSum);
}

TEST_CASE("FVector4D::operator-(float)")
{
	FVector4D Vector = { 1.0f, 2.0f, 3.0f, 1.0f };
	FVector4D ActualDifference = Vector - 2.0f;
	FVector4D ExpectedDifference = { -1.0f, 0.0f, 1.0f, -1.0f };
	REQUIRE(ActualDifference == ExpectedDifference);
}

TEST_CASE("FVector4D::operator*(float)")
{
	FVector4D Vector = { 1.0f, 2.0f, 3.0f, 1.0f };
	FVector4D ActualProduct = Vector * 2.0f;
	FVector4D ExpectedProduct = { 2.0f, 4.0f, 6.0f, 2.0f };
	REQUIRE(ActualProduct == ExpectedProduct);
}

TEST_CASE("FVector4D::operator/(float)")
{
	FVector4D Vector = { 1.0f, 2.0f, 3.0f, 1.0f };
	FVector4D ActualQuotient = Vector / 2.0f;
	FVector4D ExpectedQuotient = { 0.5f, 1.0f, 1.5f, 0.5f };
	REQUIRE(ActualQuotient == ExpectedQuotient);
}

TEST_CASE("operator*(float, const FVector4D&)")
{
	FVector4D Vector = { 1.0f, 2.0f, 3.0f, 1.0f };
	FVector4D ActualProduct = 2.0f * Vector;
	FVector4D ExpectedProduct = { 2.0f, 4.0f, 6.0f, 2.0f };
	REQUIRE(ActualProduct == ExpectedProduct);
}

// Equality operators
TEST_CASE("FVector4D::operator==")
{
	FVector4D Vector1 = { 1.0f, 2.0f, 3.0f, 1.0f };
	FVector4D Vector2 = { 1.0f, 2.0f, 3.0f, 1.0f };
	REQUIRE(Vector1 == Vector2);
}

TEST_CASE("FVector4D::operator!=")
{
	FVector4D Vector1 = { 1.0f, 2.0f, 3.0f, 1.0f };
	FVector4D Vector2 = { 1.0f, 2.0f, 3.0f, 0.0f };
	REQUIRE(Vector1 != Vector2);
}
