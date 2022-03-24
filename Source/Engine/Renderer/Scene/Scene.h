#pragma once

#include "CoreMinimal.h"
#include "Skybox.h"
#include "Geometry/Model.h"
#include "Lights/DirectionalLight.h"
#include "Lights/PointLight.h"

/**
 * A scene contains all the data needed to render the game world
 * independent of any cameras. Scene objects (models, lights, and skyboxes)
 * can be added/removed from the scene at runtime, and by accessing 
 * individual scene objects, you can modify their individual properties as well.
 */
class FScene
{
public:
	FScene() = default;
	explicit FScene(const FStringId& InSceneFileName);

	~FScene() = default;

	FScene(const FScene&) = default;
	FScene& operator=(const FScene&) = default;
	FScene(FScene&&) = default;
	FScene& operator=(FScene&&) = default;

	// Getters.
	TSharedPtr<FSkybox> GetSkybox() 
	{ 
		return Skybox; 
	}
	TSharedPtr<const FSkybox> GetSkybox() const 
	{ 
		return Skybox; 
	}

	const TArray<TSharedPtr<FModel>>& GetVisibleModels() const 
	{ 
		return VisibleModels;
	};
	const TArray<TSharedPtr<FDirectionalLight>>& GetVisibleDirectionalLights() const 
	{
		return VisibleDirectionalLights; 
	};
	const TArray<TSharedPtr<FPointLight>>& GetVisiblePointLights() const
	{ 
		return VisiblePointLights; 
	};

	const TArray<TSharedPtr<FModel>>& GetInvisibleModels() const 
	{ 
		return InvisibleModels; 
	};
	const TArray<TSharedPtr<FDirectionalLight>>& GetInvisibleDirectionalLights() const 
	{ 
		return InvisibleDirectionalLights; 
	};
	const TArray<TSharedPtr<FPointLight>>& GetInvisiblePointLights() const 
	{ 
		return InvisiblePointLights; 
	};

	// Scene configuration.
	void SetSkybox(const TSharedPtr<FSkybox>& InSkybox);
	
	void AddModel(const TSharedPtr<FModel>& InModel);
	void AddModel(const FStringId& InModelName, const FStringId& InModelFileName);
	void AddDirectionalLight(const TSharedPtr<FDirectionalLight>& InDirectionalLight);
	void AddPointLight(const TSharedPtr<FPointLight>& InPointLight);

	bool RemoveModel(const TSharedPtr<FModel>& InModel);
	bool RemoveDirectionalLight(const TSharedPtr<FDirectionalLight>& InDirectionalLight);
	bool RemovePointLight(const TSharedPtr<FPointLight>& InPointLight);

	// Scene object visibility.
	bool IsModelVisible(const FStringId& InModelName);
	bool IsDirectionalLightVisible(const FStringId& InDirectionalLightName);
	bool IsPointLightVisible(const FStringId& InPointLightName);

	void SetModelVisible(const FStringId& InModelName);
	void SetModelInvisible(const FStringId& InModelName);	

	void SetDirectionalLightVisible(const FStringId& InDirectionalLightName);
	void SetDirectionalLightInvisible(const FStringId& InDirectionalLightName);
	
	void SetPointLightVisible(const FStringId& InPointLightName);
	void SetPointLightInvisible(const FStringId& InPointLightName);

	// Scene object status.
	bool IsModelContained(const FStringId& InModelName);
	bool IsDirectionalLightContained(const FStringId& InDirectionalLightName);
	bool IsPointLightContained(const FStringId& InPointLightName);

private:
	TSharedPtr<FSkybox> Skybox;

	// Using visible/invisible arrays rather than storing visibility info per scene object. 
	// This makes visibility info implicit and reduces our memory footprint.
	TArray<TSharedPtr<FModel>> VisibleModels;
	TArray<TSharedPtr<FModel>> InvisibleModels;

	TArray<TSharedPtr<FDirectionalLight>> VisibleDirectionalLights;
	TArray<TSharedPtr<FDirectionalLight>> InvisibleDirectionalLights;

	TArray<TSharedPtr<FPointLight>> VisiblePointLights;
	TArray<TSharedPtr<FPointLight>> InvisiblePointLights;
};

/**
* Attorney class that gives FScene write-only access to ObjectType's Scene data member.
* Used to set FModel, FDirectionalLight, and FPointLight's Scene data member without granting access to their other private data members.
*/
template <typename ObjectType>
class FSetScene
{
private:
	explicit FSetScene(ObjectType& InObject, FScene& InScene) { InObject.Scene = &InScene; }
	friend class FScene;
};
