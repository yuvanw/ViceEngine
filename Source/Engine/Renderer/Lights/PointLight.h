#pragma once

#include "CoreMinimal.h"

class FScene;

template <typename ObjectType>
class FSetScene;

/**
 * Attenuation defines how the intensity of a point light decreases as distance from
 * the point light increases. For more information on the attenuation formula, see:
 * https://imdoingitwrong.wordpress.com/2011/01/31/light-attenuation/
 */
struct FAttenuation
{
	explicit FAttenuation(float InConstant, float InLinear, float InQuadratic)
		: Constant(InConstant)
		, Linear(InLinear)
		, Quadratic(InQuadratic)
	{
	}

	float Constant;
	float Linear;
	float Quadratic;
};

/**
 * A point light emits light in all directions from a single point in space.
 */
class FPointLight
{
public:
	explicit FPointLight(const FStringId& InName, const FVector3D& InPosition, const FColor& InColor, float InIntensity, const FAttenuation& InAttenuation);
	~FPointLight() = default;

	// Non-copyable.
	FPointLight(const FPointLight&) = delete;
	FPointLight& operator=(const FPointLight&) = delete;

	FPointLight(FPointLight&&) = default;
	FPointLight& operator=(FPointLight&&) = default;

	// Getters.
	const FStringId& GetName() const 
	{ 
		return Name; 
	}
	const FVector3D& GetPosition() const 
	{ 
		return Position; 
	}
	const FColor& GetColor() const 
	{ 
		return Color;
	}
	const FAttenuation& GetAttenuation() const
	{ 
		return Attenuation;
	}
	float GetIntensity() const 
	{ 
		return Intensity; 
	}

	// Setters.
	void SetPosition(const FVector3D& InPosition) 
	{ 
		Position = InPosition; 
	}
	void SetColor(const FColor& InColor) 
	{ 
		Color = InColor; 
	}
	void SetAttenuation(const FAttenuation& InAttenuation) 
	{ 
		Attenuation = InAttenuation; 
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

	FVector3D Position;
	FColor Color;
	FAttenuation Attenuation;
	float Intensity;

	friend class FSetScene<FPointLight>;
};
