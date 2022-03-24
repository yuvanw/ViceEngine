#include "Skybox.h"

FSkybox::FSkybox(const FStringId& InName, const FCubemap& InCubemap)
	: Name(InName)
	, Scene(nullptr)
	// Create cube mesh with default mesh and material files.
	, CubeMesh("Cube.obj", "Skybox.mat")
	, Cubemap(InCubemap)
	, bIsVisible(true)
{
}

void FSkybox::SetVisible()
{
	bIsVisible = true;
}

void FSkybox::SetInvisible()
{
	bIsVisible = false;
}

bool FSkybox::IsVisible() const
{
	return bIsVisible;
}
