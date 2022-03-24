#pragma once

#include "CoreMinimal.h"

struct FSkyboxInfo
{
	FStringId Name;
	FStringId RightTextureFileName;
	FStringId LeftTextureFileName;
	FStringId TopTextureFileName;
	FStringId BottomTextureFileName;
	FStringId BackTextureFileName;
	FStringId FrontTextureFileName;
};

struct FDirectionalLightInfo
{
	FStringId Name;
	FVector3D Direction;
	FVector3D Color;
	float Intensity;
};

struct FAttenuationInfo
{
	float Constant;
	float Linear;
	float Quadratic;
};

struct FPointLightInfo
{
	FStringId Name;
	FVector3D Position;
	FVector3D Color;
	float Intensity;
	FAttenuationInfo Attenuation;
};

struct FTransformInfo
{
	FVector3D Position;
	FVector3D Rotation;
	FVector3D Scale;
};

struct FModelInfo
{
	FStringId Name;
	FStringId FileName;
	FTransformInfo Transform;
};

/**
 * A scene file (.scn) specifies all of the objects that make up a scene,
 * including models, lights, and the skybox.
 */
class FSceneFile
{
public:
	explicit FSceneFile(const FStringId& InSceneFileName);
	~FSceneFile() = default;

	FSceneFile(const FSceneFile&) = default;
	FSceneFile& operator=(const FSceneFile&) = default;
	FSceneFile(FSceneFile&&) = default;
	FSceneFile& operator=(FSceneFile&&) = default;

private:
	FSkyboxInfo Skybox;
	bool bHasSkybox;
	
	TArray<FDirectionalLightInfo> DirectionalLights;
	TArray<FPointLightInfo> PointLights;
	TArray<FModelInfo> Models;

	friend class FScene;
};
