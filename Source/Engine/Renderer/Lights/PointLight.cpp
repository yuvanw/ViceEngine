#include "PointLight.h"
#include "Scene/Scene.h"

FPointLight::FPointLight(const FStringId& InName, const FVector3D& InPosition, const FColor& InColor, float InIntensity, const FAttenuation& InAttenuation)
	: Name(InName)
	, Scene(nullptr)
	, Position(InPosition)
	, Color(InColor)
	, Attenuation(InAttenuation)
	, Intensity(InIntensity)
{
}

bool FPointLight::IsVisible() const
{
	if (Scene)
	{
		return Scene->IsPointLightVisible(Name);
	}

	return false;
}

void FPointLight::SetVisible()
{
	if (Scene)
	{
		Scene->SetPointLightVisible(Name);
	}
}

void FPointLight::SetInvisible()
{
	if (Scene)
	{
		Scene->SetPointLightInvisible(Name);
	}
}
