#pragma once

#include "CoreMinimal.h"

class FScene;
class FCamera;
class FSkybox;
class FModel;
class FMesh;
class FDirectionalLight;
class FPointLight;
class FMaterial;

enum class ESceneObjectType : uint8
{
	Camera,
	Skybox,
	Model,
	DirectionalLight,
	PointLight
};

/**
 * The outliner UI lists all scene objects, including camera, skybox, models, and lights.
 * It allows the user to select any scene object, add/remove objects from the scene, 
 * and load any scene file under the Assets directory.
 */
class FOutlinerUI
{
public:
	FOutlinerUI() = default;
	~FOutlinerUI() = default;

	// Non-copyable.
	FOutlinerUI(const FOutlinerUI&) = default;
	FOutlinerUI& operator=(const FOutlinerUI&) = default;

	// Non-movable.
	FOutlinerUI(FOutlinerUI&&) = default;
	FOutlinerUI& operator=(FOutlinerUI&&) = default;

	void Init();
	void Shutdown();
	void Update();

	TSharedPtr<FCamera> GetSelectedCamera();
	TSharedPtr<FSkybox> GetSelectedSkybox();
	TSharedPtr<FModel> GetSelectedModel();
	TSharedPtr<FDirectionalLight> GetSelectedDirectionalLight();
	TSharedPtr<FPointLight> GetSelectedPointLight();

	static const ANSICHAR* WindowName;

private:
	int32 SelectedObjectIndex;
	int32 SelectedSceneIndex;
	int32 SelectedAddIndex;

	TSharedPtr<FScene> Scene;
	TSharedPtr<FCamera> Camera;
	TSharedPtr<FSkybox> Skybox;
	TArray<TSharedPtr<FModel> > Models;
	TArray<TSharedPtr<FDirectionalLight> > DirectionalLights;
	TArray<TSharedPtr<FPointLight> > PointLights;

	void RenderLoadSceneButton();
	void RenderAddButton();
	void RenderRemoveButton();
};
