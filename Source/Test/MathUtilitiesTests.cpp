#include "catch/catch.hpp"

#include "Math/MathUtilities.h"

TEST_CASE("FMath::Cos")
{
	REQUIRE(FMath::IsApproximatelyEqual(FMath::Cos(0.0f), 1.0f));
	REQUIRE(FMath::IsApproximatelyEqual(FMath::Cos(FMath::Pi / 3.0f), 0.5f));
	REQUIRE(FMath::IsApproximatelyZero(FMath::Cos(FMath::PiOverTwo)));
}

TEST_CASE("FMath::Sin")
{
	REQUIRE(FMath::IsApproximatelyZero(FMath::Sin(0.0f)));
	REQUIRE(FMath::IsApproximatelyEqual(FMath::Sin(FMath::Pi / 6.0f), 0.5f));
	REQUIRE(FMath::IsApproximatelyEqual(FMath::Sin(FMath::PiOverTwo), 1.0f));
}

TEST_CASE("FMath::Tan")
{
	REQUIRE(FMath::IsApproximatelyZero(FMath::Tan(0.0f)));
	REQUIRE(FMath::IsApproximatelyEqual(FMath::Tan(FMath::PiOverFour), 1.0f));
	REQUIRE(FMath::IsApproximatelyZero(FMath::Tan(FMath::Pi)));
}

TEST_CASE("FMath::Acos")
{
	REQUIRE(FMath::IsApproximatelyZero(FMath::Acos(1.0f)));
	REQUIRE(FMath::IsApproximatelyEqual(FMath::Acos(0.5f), FMath::Pi / 3.0f));
	REQUIRE(FMath::IsApproximatelyEqual(FMath::Acos(0.0f), FMath::PiOverTwo));
}

TEST_CASE("FMath::Asin")
{
	REQUIRE(FMath::IsApproximatelyZero(FMath::Asin(0.0f)));
	REQUIRE(FMath::IsApproximatelyEqual(FMath::Asin(0.5f), FMath::Pi / 6.0f));
	REQUIRE(FMath::IsApproximatelyEqual(FMath::Asin(1.0f), FMath::PiOverTwo));
}

TEST_CASE("FMath::Atan")
{
	REQUIRE(FMath::IsApproximatelyZero(FMath::Atan(0.0f)));
	REQUIRE(FMath::IsApproximatelyEqual(FMath::Atan(1.0f), FMath::PiOverFour));
}

TEST_CASE("FMath::Atan2")
{
	REQUIRE(FMath::IsApproximatelyZero(FMath::Atan2(0.0f, 1.0f)));
	REQUIRE(FMath::IsApproximatelyEqual(FMath::Atan2(0.0f, -1.0f), FMath::Pi));
}

TEST_CASE("FMath::IsApproximatelyZero")
{
	SECTION("Using default error tolerance value FMath::Epsilon (1.e-6f).")
	{
		// Input is less than the error tolerance value.
		REQUIRE(FMath::IsApproximatelyZero(0));
		REQUIRE(FMath::IsApproximatelyZero(1.e-7f));

		// Input is greater than or equal to the error tolerance value.
		REQUIRE(!FMath::IsApproximatelyZero(FMath::Epsilon));
		REQUIRE(!FMath::IsApproximatelyZero(1.e-5f));
	}

	SECTION("Using user-specified error tolerance value.")
	{
		float ErrorTolerance = 0.1f;

		// Input is less than the error tolerance value.
		REQUIRE(FMath::IsApproximatelyZero(0, ErrorTolerance));
		REQUIRE(FMath::IsApproximatelyZero(0.01f, ErrorTolerance));

		// Input is greater than or equal to the error tolerance value.
		REQUIRE(!FMath::IsApproximatelyZero(ErrorTolerance, ErrorTolerance));
		REQUIRE(!FMath::IsApproximatelyZero(ErrorTolerance + 0.1f, ErrorTolerance));
	}
}

TEST_CASE("FMath::IsApproximatelyEqual")
{
	SECTION("Using default error tolerance value FMath::Epsilon (1.e-6f).")
	{
		// Inputs are within FMath::Epsilon units of each other.
		REQUIRE(FMath::IsApproximatelyEqual(5.0f, 5.0f));
		REQUIRE(FMath::IsApproximatelyEqual(2.e-7f, 1.e-7f));

		// Inputs are not within FMath::Epsilon units of each other.
		REQUIRE(!FMath::IsApproximatelyEqual(2.0f * FMath::Epsilon, FMath::Epsilon));
		REQUIRE(!FMath::IsApproximatelyEqual(5.1f, 5.0f));
	}

	SECTION("Using user-specified error tolerance value.")
	{
		float ErrorTolerance = 0.1f;

		// Inputs is less than the error tolerance value.
		REQUIRE(FMath::IsApproximatelyEqual(5.0f, 5.0f, ErrorTolerance));
		REQUIRE(FMath::IsApproximatelyEqual(0.02f, 0.01f, ErrorTolerance));

		// Input is greater than or equal to the error tolerance value.
		REQUIRE(!FMath::IsApproximatelyEqual(0.2f, 0.1f, ErrorTolerance));
		REQUIRE(!FMath::IsApproximatelyEqual(2.0f, 1.0f, ErrorTolerance));
	}
}

TEST_CASE("FMath::DegreesToRadians")
{
	REQUIRE(FMath::IsApproximatelyZero(FMath::DegreesToRadians(0.0f)));
	REQUIRE(FMath::IsApproximatelyEqual(FMath::DegreesToRadians(45.0f), FMath::PiOverFour));
	REQUIRE(FMath::IsApproximatelyEqual(FMath::DegreesToRadians(90.0f), FMath::PiOverTwo));
}

TEST_CASE("FMath::RadiansToDegrees")
{
	REQUIRE(FMath::IsApproximatelyZero(FMath::RadiansToDegrees(0.0f)));
	REQUIRE(FMath::IsApproximatelyEqual(FMath::RadiansToDegrees(FMath::PiOverFour), 45.0f));
	REQUIRE(FMath::IsApproximatelyEqual(FMath::RadiansToDegrees(FMath::PiOverTwo), 90.0f));
}

TEST_CASE("FMath::Pow")
{
	// 2^0 = 1
	REQUIRE(FMath::IsApproximatelyEqual(FMath::Pow(2.0f, 0.0f), 1.0f));
	// 2^6 = 64
	REQUIRE(FMath::IsApproximatelyEqual(FMath::Pow(2.0f, 6.0f), 64.0f));
	// Pi^2 =  9.86960440109
	REQUIRE(FMath::IsApproximatelyEqual(FMath::Pow(FMath::Pi, 2.0f), 9.86960440109f));
}

TEST_CASE("FMath::Sqrt")
{
	// Square root of 4 = 2
	REQUIRE(FMath::IsApproximatelyEqual(FMath::Sqrt(4.0f), 2.0f));
	// Square root of 2 = 1.41421356237
	REQUIRE(FMath::IsApproximatelyEqual(FMath::Sqrt(2.0f), 1.41421356237f));
}

TEST_CASE("FMath::InvSqrt")
{
	// Inverse square root of 4 = 1/2
	REQUIRE(FMath::IsApproximatelyEqual(FMath::InvSqrt(4.0f), 0.5f));
	// Square root of 2 = 1/1.41421356237
	REQUIRE(FMath::IsApproximatelyEqual(FMath::InvSqrt(2.0f), 1.0f / 1.41421356237f));
}

TEST_CASE("FMath::Ceil")
{
	REQUIRE(FMath::IsApproximatelyEqual(FMath::Ceil(0.1f), 1.0f));
	REQUIRE(FMath::IsApproximatelyEqual(FMath::Ceil(0.5f), 1.0f));
	REQUIRE(FMath::IsApproximatelyZero(FMath::Ceil(-0.1f)));
	REQUIRE(FMath::IsApproximatelyZero(FMath::Ceil(-0.5f)));
}

TEST_CASE("FMath::Floor")
{
	REQUIRE(FMath::IsApproximatelyZero(FMath::Floor(0.1f)));
	REQUIRE(FMath::IsApproximatelyZero(FMath::Floor(0.5f)));
	REQUIRE(FMath::IsApproximatelyEqual(FMath::Floor(-0.1f), -1.0f));
	REQUIRE(FMath::IsApproximatelyEqual(FMath::Floor(-0.5f), -1.0f));
}

TEST_CASE("FMath::Trunc")
{
	REQUIRE(FMath::IsApproximatelyZero(FMath::Trunc(0.1f)));
	REQUIRE(FMath::IsApproximatelyZero(FMath::Trunc(0.5f)));
	REQUIRE(FMath::IsApproximatelyZero(FMath::Trunc(-0.1f)));
	REQUIRE(FMath::IsApproximatelyZero(FMath::Trunc(-0.5f)));
}

TEST_CASE("FMath::Round")
{
	REQUIRE(FMath::IsApproximatelyZero(FMath::Round(0.1f)));
	REQUIRE(FMath::IsApproximatelyEqual(FMath::Round(0.5f), 1.0f));
	REQUIRE(FMath::IsApproximatelyEqual(FMath::Round(-0.1f), 0.0f));
	REQUIRE(FMath::IsApproximatelyEqual(FMath::Round(-0.5f), -1.0f));
}

TEST_CASE("FMath::RoundUpToNearestPowerOfTwo")
{
	REQUIRE(FMath::RoundUpToNearestPowerOfTwo(1) == 1);
	REQUIRE(FMath::RoundUpToNearestPowerOfTwo(2) == 2);
	REQUIRE(FMath::RoundUpToNearestPowerOfTwo(3) == 4);
	REQUIRE(FMath::RoundUpToNearestPowerOfTwo(-3) == 0);
}

TEST_CASE("FMath::Max")
{
	// FMath::Max(float, float)
	REQUIRE(FMath::Max(1.0f, 0.0f) == 1.0f);
	REQUIRE(FMath::Max(0.0f, 1.0f) == 1.0f);

	// FMath::Max(int32, int32)
	REQUIRE(FMath::Max(1, 0) == 1);
	REQUIRE(FMath::Max(0, 1) == 1);
}

TEST_CASE("FMath::Min")
{
	// FMath::Max(float, float)
	REQUIRE(FMath::Min(1.0f, 0.0f) == 0.0f);
	REQUIRE(FMath::Min(0.0f, 1.0f) == 0.0f);

	// FMath::Max(int32, int32)
	REQUIRE(FMath::Min(1, 0) == 0);
	REQUIRE(FMath::Min(0, 1) == 0);
}

TEST_CASE("FMath::Abs")
{
	REQUIRE(FMath::Abs(5.0f) == 5.0f);
	REQUIRE(FMath::Abs(-5.0f) == 5.0f);
}

TEST_CASE("FMath::Clamp")
{
	int Min = 0.0f;
	int Max = 1.0f;
	REQUIRE(FMath::Clamp(0.5f, Min, Max) == 0.5f);
	REQUIRE(FMath::Clamp(-1.0f, Min, Max) == Min);
	REQUIRE(FMath::Clamp(2.0f, Min, Max) == Max);
}

TEST_CASE("FMath::IsPowerOfTwo")
{
	REQUIRE(FMath::IsPowerOfTwo(2));
	REQUIRE(FMath::IsPowerOfTwo(64));
	REQUIRE(!FMath::IsPowerOfTwo(3));
	REQUIRE(!FMath::IsPowerOfTwo(-2));
}
