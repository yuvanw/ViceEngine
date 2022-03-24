#pragma once

#include "CoreMinimal.h"

class FCamera;
class FSkybox;
class FModel;
class FMesh;
class FDirectionalLight;
class FPointLight;

/**
 * The inspector UI allows the user to view and modify the properties of the selected scene object.
 * Below are the list of properties the user can view and modify per scene object type:
 * 
 * Camera:
 * - Projection type (i.e. perspective vs. orthographic)
 * - Near plane distance
 * - Far plane distance
 * 
 * Model:
 * - Transform (position, rotation, scale)
 * - Drawing mode (i.e. filled, wireframe, points)
 * - Meshes
 * - The material of each mesh
 * 
 * Directional Light:
 * - Direction
 * - Intensity
 * - Color
 * 
 * Point Light:
 * - Position
 * - Intensity
 * - Color
 */
class FInspectorUI
{
public:
	FInspectorUI() = default;
	~FInspectorUI() = default;

	// Non-copyable.
	FInspectorUI(const FInspectorUI&) = default;
	FInspectorUI& operator=(const FInspectorUI&) = default;

	// Non-movable.
	FInspectorUI(FInspectorUI&&) = default;
	FInspectorUI& operator=(FInspectorUI&&) = default;

	void Init();
	void Shutdown();
	void Update();

	void SetSelected(const TSharedPtr<FCamera>& InCamera);
	void SetSelected(const TSharedPtr<FSkybox>& InSkybox);
	void SetSelected(const TSharedPtr<FModel>& InModel);
	void SetSelected(const TSharedPtr<FDirectionalLight>& InDirectionalLight);
	void SetSelected(const TSharedPtr<FPointLight>& InPointLight);

	static const ANSICHAR* WindowName;

private:
	TSharedPtr<FCamera> Camera;
	TSharedPtr<FSkybox> Skybox;
	TSharedPtr<FModel> Model;
	TSharedPtr<FDirectionalLight> DirectionalLight;
	TSharedPtr<FPointLight> PointLight;

	TArray<const ANSICHAR*> MaterialFileNames;
	TArray<const ANSICHAR*> TextureFileNames;

	void RenderCameraInfo();
	void RenderModelInfo();
	void RenderMaterialInfo(FMesh& InMesh);
	void RenderDirectionalLightInfo();
	void RenderPointLightInfo();
	
	void ClearObjects();
};
