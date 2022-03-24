#include "DirectionalLight.h"
#include "Scene/Scene.h"

FDirectionalLight::FDirectionalLight(const FStringId& InName, const FVector3D& InDirection, const FColor& InColor, float InIntensity)
	: Name(InName)
	, Scene(nullptr)
	, Direction(InDirection)
	, Color(InColor)
	, Intensity(InIntensity)
{
}

bool FDirectionalLight::IsVisible() const
{
	if (Scene)
	{
		return Scene->IsDirectionalLightVisible(Name);
	}

	return false;
}

void FDirectionalLight::SetVisible()
{
	if (Scene)
	{
		Scene->SetDirectionalLightVisible(Name);
	}
}

void FDirectionalLight::SetInvisible()
{
	if (Scene)
	{
		Scene->SetDirectionalLightInvisible(Name);
	}
}
