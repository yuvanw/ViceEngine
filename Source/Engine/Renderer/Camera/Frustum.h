#pragma once

class FFrustum
{
public:
	// Default constructor creates a frustum using the Renderer's current viewport dimensions.
	FFrustum();

	explicit constexpr FFrustum(float InWidth, float InHeight, float InNearDistance, float InFarDistance, float InVerticalFovRadians)
		: Width(InWidth)
		, Height(InHeight)
		, NearDistance(InNearDistance)
		, FarDistance(InFarDistance)
		, VerticalFovRadians(InVerticalFovRadians)
	{
	}

	// Frustum surface width.
	float Width;
	// Frustum surface height.
	float Height;
	// Near plane distance.
	float NearDistance;
	// Far plane distance.
	float FarDistance;
	// Vertical field of view in radians.
	float VerticalFovRadians;

private:
	static const float DefaultNearDistance /* = 0.1f */;
	static const float DefaultFarDistance /* = 1000.0f */;
	static const float DefaultVerticalFovRadians /* = 45.0f */;
};
