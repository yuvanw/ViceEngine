#pragma once

#include "CoreMinimal.h"

class FScene;

template <typename ObjectType> 
class FSetScene;

/**
 * A directional light emits light in a single direction. The light does not
 * have a position, because it is perceived to be infinitely far away.
 */
class FDirectionalLight
{
public:
	explicit FDirectionalLight(const FStringId& InName, const FVector3D& InDirection, const FColor& InColor, float InIntensity);
	~FDirectionalLight() = default;

	// Non-copyable.
	FDirectionalLight(const FDirectionalLight&) = delete;
	FDirectionalLight& operator=(const FDirectionalLight&) = delete;

	FDirectionalLight(FDirectionalLight&&) = default;
	FDirectionalLight& operator=(FDirectionalLight&&) = default;

	// Getters.
	const FStringId& GetName() const 
	{ 
		return Name; 
	}

	const FVector3D& GetDirection() const 
	{
		return Direction; 
	}
	const FColor& GetColor() const 
	{
		return Color;
	}
	float GetIntensity() const 
	{ 
		return Intensity; 
	}

	// Setters.
	void SetDirection(const FVector3D& InDirection) 
	{ 
		Direction = InDirection; 
	}
	void SetColor(const FColor& InColor) 
	{ 
		Color = InColor;
	}
	void SetIntensity(float InIntensity) 
	{ 
		Intensity = InIntensity;
	}
	void SetVisible();
	void SetInvisible();

	bool IsVisible() const;

private:
	FStringId Name;
	FScene* Scene;

	FVector3D Direction;
	FColor Color;
	float Intensity;

	friend class FSetScene<FDirectionalLight>;
};
