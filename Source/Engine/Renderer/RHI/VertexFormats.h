#pragma once

#include "Math/Vector2D.h"
#include "Math/Vector3D.h"

struct FVertex1P1N1UV
{
	explicit FVertex1P1N1UV(const FVector3D& InPosition, const FVector3D& InNormal, const FVector2D& InTextureCoordinates)
		: Position(InPosition)
		, Normal(InNormal)
		, TextureCoordinates(InTextureCoordinates)
	{
	}

	bool operator==(const FVertex1P1N1UV& InVertex) const
	{
		return (Position == InVertex.Position) 
			&& (Normal == InVertex.Normal) 
			&& (TextureCoordinates == InVertex.TextureCoordinates);
	}

	FVector3D Position;
	FVector3D Normal;
	FVector2D TextureCoordinates;
};
