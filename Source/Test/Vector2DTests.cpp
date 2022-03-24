#include "catch/catch.hpp"

#include "Math/Vector2D.h"
#include "Math/MathUtilities.h"

// Constructors
TEST_CASE("FVector2D default constructor.")
{
	FVector2D Vector;
	REQUIRE(Vector.X == 0.0f);
	REQUIRE(Vector.Y == 0.0f);
}

TEST_CASE("FVector2D constructor taking three floats.")
{
	SECTION("Data members are initialized to their corresponding argument's value.")
	{
		FVector2D Vector(1.0f, 2.0f);
		REQUIRE(Vector.X == 1.0f);
		REQUIRE(Vector.Y == 2.0f);
	}

	SECTION("Arguments can be specified via copy-list-initialization syntax.")
	{
		FVector2D Vector = { 1.0f, 2.0f };
		REQUIRE(Vector.X == 1.0f);
		REQUIRE(Vector.Y == 2.0f);
	}
}

// Copy operations
TEST_CASE("FVector2D copy constructor.")
{
	FVector2D Vector1 = { 1.0f, 2.0f };
	FVector2D Vector2 = Vector1;
	REQUIRE(Vector1 == Vector2);
}

TEST_CASE("FVector2D copy assignment operator.")
{
	FVector2D Vector1 = { 1.0f, 2.0f };
	FVector2D Vector2;
	Vector2 = Vector1;
	REQUIRE(Vector1 == Vector2);
}

// Vector math
TEST_CASE("FVector2D::GetLength")
{
	FVector2D Vector = { 4.0f, 3.0f };
	float LengthSquared = Vector.GetLength();
	REQUIRE(FMath::IsApproximatelyEqual(LengthSquared, 5.0f));
}

TEST_CASE("FVector2D::GetLengthSquared")
{
	FVector2D Vector = { 4.0f, 3.0f };
	float Length = Vector.GetLengthSquared();
	REQUIRE(FMath::IsApproximatelyEqual(Length, 25.0f));
}

TEST_CASE("FVector2D::Normalize")
{
	SECTION("Vectors with zero length are unchanged.")
	{
		FVector2D Zero = { 0.0f, 0.0f };
		Zero.Normalize();
		REQUIRE(Zero.X == 0.0f);
		REQUIRE(Zero.Y == 0.0f);
	}

	SECTION("Vectors with non-zero length are normalized.")
	{
		FVector2D Vector = { 4.0f, 3.0f };
		float Length = Vector.GetLength();

		FVector2D NormalizedVector = Vector.Normalize();
		REQUIRE(NormalizedVector.X == (4.0f / Length));
		REQUIRE(NormalizedVector.Y == (3.0f / Length));
	}
}

TEST_CASE("FVector2D::DotProduct")
{
	FVector2D Vector1 = { 1.0f, 2.0f };
	FVector2D Vector2 = { 2.0f, 3.0f };
	float ActualDotProduct = FVector2D::DotProduct(Vector1, Vector2);
	float ExpectedDotProduct = 8.0f;
	REQUIRE(FMath::IsApproximatelyEqual(ActualDotProduct, ExpectedDotProduct));
}

// Accessors
TEST_CASE("FVector2D::operator[]")
{
	SECTION("Read and write access is permitted for non-const FVector2D data members.")
	{
		FVector2D Vector = { 1.0f, 2.0f };
		REQUIRE(Vector[0] == 1.0f);
		REQUIRE(Vector[1] == 2.0f);

		Vector[0] = 4.0f;
		REQUIRE(Vector[0] == 4.0f);
	}

	SECTION("Only read access is permitted for const FVector2D data members.")
	{
		const FVector2D Vector = { 1.0f, 2.0f };
		REQUIRE(Vector[0] == 1.0f);
		REQUIRE(Vector[1] == 2.0f);
	}
}

// Component-wise operations
TEST_CASE("FVector2D::operator+=(const FVector2D&)")
{
	FVector2D Vector1 = { 1.0f, 2.0f };
	FVector2D Vector2 = { 2.0f, 3.0f };
	Vector1 += Vector2;
	FVector2D ExpectedSum = { 3.0f, 5.0f };
	REQUIRE(Vector1 == ExpectedSum);
}

TEST_CASE("FVector2D::operator-=(const FVector2D&)")
{
	FVector2D Vector1 = { 1.0f, 2.0f };
	FVector2D Vector2 = { 2.0f, 3.0f };
	Vector1 -= Vector2;
	FVector2D ExpectedDifference = { -1.0f, -1.0f };
	REQUIRE(Vector1 == ExpectedDifference);

}

TEST_CASE("FVector2D::operator*=(const FVector2D&)")
{
	FVector2D Vector1 = { 1.0f, 2.0f };
	FVector2D Vector2 = { 2.0f, 3.0f };
	Vector1 *= Vector2;
	FVector2D ExpectedProduct = { 2.0f, 6.0f };
	REQUIRE(Vector1 == ExpectedProduct);
}

TEST_CASE("FVector2D::operator/=(const FVector2D&)")
{
	FVector2D Vector1 = { 1.0f, 2.0f };
	FVector2D Vector2 = { 1.0f, 2.0f };
	Vector1 /= Vector2;
	FVector2D ExpectedQuotient = { 1.0f, 1.0f };
	REQUIRE(Vector1 == ExpectedQuotient);
}

TEST_CASE("FVector2D::operator+(const FVector2D&)")
{
	FVector2D Vector1 = { 1.0f, 2.0f };
	FVector2D Vector2 = { 2.0f, 3.0f };
	FVector2D ActualSum = Vector1 + Vector2;
	FVector2D ExpectedSum = { 3.0f, 5.0f };
	REQUIRE(ActualSum == ExpectedSum);
}

TEST_CASE("FVector2D::operator-(const FVector2D&)")
{
	FVector2D Vector1 = { 1.0f, 2.0f };
	FVector2D Vector2 = { 2.0f, 3.0f };
	FVector2D ActualDifference = Vector1 - Vector2;
	FVector2D ExpectedDifference = { -1.0f, -1.0f };
	REQUIRE(ActualDifference == ExpectedDifference);
}

TEST_CASE("FVector2D::operator*(const FVector2D&)")
{
	FVector2D Vector1 = { 1.0f, 2.0f };
	FVector2D Vector2 = { 2.0f, 3.0f };
	FVector2D ActualProduct = Vector1 * Vector2;
	FVector2D ExpectedProduct = { 2.0f, 6.0f };
	REQUIRE(ActualProduct == ExpectedProduct);
}

TEST_CASE("FVector2D::operator/(const FVector2D&)")
{
	FVector2D Vector1 = { 1.0f, 2.0f };
	FVector2D Vector2 = { 1.0f, 2.0f };
	FVector2D ActualQuotient = Vector1 / Vector2;
	FVector2D ExpectedQuotient = { 1.0f, 1.0f };
	REQUIRE(ActualQuotient == ExpectedQuotient);
}

// Scalar operations
TEST_CASE("FVector2D::operator+=(float)")
{
	FVector2D Vector = { 1.0f, 2.0f };
	Vector += 2.0f;
	FVector2D ExpectedSum = { 3.0f, 4.0f };
	REQUIRE(Vector == ExpectedSum);
}

TEST_CASE("FVector2D::operator-=(float)")
{
	FVector2D Vector = { 1.0f, 2.0f };
	Vector -= 2.0f;
	FVector2D ExpectedSum = { -1.0f, 0.0f };
	REQUIRE(Vector == ExpectedSum);
}

TEST_CASE("FVector2D::operator*=(float)")
{
	FVector2D Vector = { 1.0f, 2.0f };
	Vector *= 2.0f;
	FVector2D ExpectedProduct = { 2.0f, 4.0f };
	REQUIRE(Vector == ExpectedProduct);
}

TEST_CASE("FVector2D::operator/=(float)")
{
	FVector2D Vector = { 1.0f, 2.0f };
	Vector /= 2.0f;
	FVector2D ExpectedQuotient = { 0.5f, 1.0f };
	REQUIRE(Vector == ExpectedQuotient);
}

TEST_CASE("FVector2D::operator+(float)")
{
	FVector2D Vector = { 1.0f, 2.0f };
	FVector2D ActualSum = Vector + 2.0f;
	FVector2D ExpectedSum = { 3.0f, 4.0f };
	REQUIRE(ActualSum == ExpectedSum);
}

TEST_CASE("FVector2D::operator-(float)")
{
	FVector2D Vector = { 1.0f, 2.0f };
	FVector2D ActualDifference = Vector - 2.0f;
	FVector2D ExpectedDifference = { -1.0f, 0.0f };
	REQUIRE(ActualDifference == ExpectedDifference);
}

TEST_CASE("FVector2D::operator*(float)")
{
	FVector2D Vector = { 1.0f, 2.0f };
	FVector2D ActualProduct = Vector * 2.0f;
	FVector2D ExpectedProduct = { 2.0f, 4.0f };
	REQUIRE(ActualProduct == ExpectedProduct);
}

TEST_CASE("FVector2D::operator/(float)")
{
	FVector2D Vector = { 1.0f, 2.0f };
	FVector2D ActualQuotient = Vector / 2.0f;
	FVector2D ExpectedQuotient = { 0.5f, 1.0f };
	REQUIRE(ActualQuotient == ExpectedQuotient);
}

TEST_CASE("operator*(float, const FVector2D&)")
{
	FVector2D Vector = { 1.0f, 2.0f };
	FVector2D ActualProduct = 2.0f * Vector;
	FVector2D ExpectedProduct = { 2.0f, 4.0f };
	REQUIRE(ActualProduct == ExpectedProduct);
}

// Equality operators
TEST_CASE("FVector2D::operator==")
{
	FVector2D Vector1 = { 1.0f, 2.0f };
	FVector2D Vector2 = { 1.0f, 2.0f };
	REQUIRE(Vector1 == Vector2);
}

TEST_CASE("FVector2D::operator!=")
{
	FVector2D Vector1 = { 1.0f, 2.0f };
	FVector2D Vector2 = { 2.0f, 3.0f };
	REQUIRE(Vector1 != Vector2);
}
