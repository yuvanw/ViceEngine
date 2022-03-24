#include "Scene.h"
#include "SceneFile.h"

FScene::FScene(const FStringId& InSceneFileName)
{
	// Parse .scn file.
	FSceneFile SceneFile(InSceneFileName);

	// Create skybox if one was specified.
	if (SceneFile.bHasSkybox)
	{
		const ANSICHAR* SkyboxName = SceneFile.Skybox.Name.GetString().GetData();
		FCubemap Cubemap(
			SceneFile.Skybox.RightTextureFileName.GetString().GetData(),
			SceneFile.Skybox.LeftTextureFileName.GetString().GetData(),
			SceneFile.Skybox.TopTextureFileName.GetString().GetData(),
			SceneFile.Skybox.BottomTextureFileName.GetString().GetData(),
			SceneFile.Skybox.BackTextureFileName.GetString().GetData(),
			SceneFile.Skybox.FrontTextureFileName.GetString().GetData()
		);
		TSharedPtr<FSkybox> Skybox = MakeShared<FSkybox>(SkyboxName, Cubemap);
		SetSkybox(Skybox);
	}

	// Create directional lights.
	for (const FDirectionalLightInfo& DirectionalLightInfo : SceneFile.DirectionalLights)
	{
		TSharedPtr<FDirectionalLight> DirectionalLight = MakeShared<FDirectionalLight>
		(
			DirectionalLightInfo.Name,
			DirectionalLightInfo.Direction,
			DirectionalLightInfo.Color, 
			DirectionalLightInfo.Intensity
		);
		AddDirectionalLight(DirectionalLight);
	}

	// Create point lights.
	for (const FPointLightInfo& PointLightInfo : SceneFile.PointLights)
	{
		TSharedPtr<FPointLight> PointLight = MakeShared<FPointLight>
		(
			PointLightInfo.Name,
			PointLightInfo.Position,
			PointLightInfo.Color,
			PointLightInfo.Intensity,
			FAttenuation
			(
				PointLightInfo.Attenuation.Constant, 
				PointLightInfo.Attenuation.Linear, 
				PointLightInfo.Attenuation.Quadratic
			)
		);
		AddPointLight(PointLight);
	}

	// Create models.
	for (const FModelInfo& ModelInfo : SceneFile.Models)
	{
		float RotationRadiansX = FMath::DegreesToRadians(ModelInfo.Transform.Rotation.X);
		float RotationRadiansY = FMath::DegreesToRadians(ModelInfo.Transform.Rotation.Y);
		float RotationRadiansZ = FMath::DegreesToRadians(ModelInfo.Transform.Rotation.Z);
		FVector3D Rotation = { RotationRadiansX, RotationRadiansY, RotationRadiansZ };
		
		TSharedPtr<FModel> Model = MakeShared<FModel>(ModelInfo.Name.GetString().GetData(), ModelInfo.FileName.GetString().GetData());
		Model->SetPosition(ModelInfo.Transform.Position);
		Model->SetRotation(Rotation);
		Model->SetScale(ModelInfo.Transform.Scale);
		AddModel(Model);
	}
}

void FScene::SetSkybox(const TSharedPtr<FSkybox>& InSkybox)
{
	if (InSkybox)
	{
		FSetScene<FSkybox>(*InSkybox, *this);
	}
	Skybox = InSkybox;
}

void FScene::AddModel(const TSharedPtr<FModel>& InModel)
{
	FSetScene<FModel>(*InModel, *this);
	VisibleModels.Add(InModel);
}

void FScene::AddModel(const FStringId& InModelName, const FStringId& InModelFileName)
{
	TSharedPtr<FModel> Model = MakeShared<FModel>(InModelName, InModelFileName);
	FSetScene<FModel>(*Model, *this);
	VisibleModels.Add(Model);
}

void FScene::AddDirectionalLight(const TSharedPtr<FDirectionalLight>& InDirectionalLight)
{
	FSetScene<FDirectionalLight>(*InDirectionalLight, *this);
	VisibleDirectionalLights.Add(InDirectionalLight);
}

void FScene::AddPointLight(const TSharedPtr<FPointLight>& InPointLight)
{
	FSetScene<FPointLight>(*InPointLight, *this);
	VisiblePointLights.Add(InPointLight);
}

bool FScene::RemoveModel(const TSharedPtr<FModel>& InModel)
{
	// Try removing model from VisibleModels. If it isn't in VisibleModels, try removing it from InvisibleModels.
	return VisibleModels.RemoveFirst(InModel) || InvisibleModels.RemoveFirst(InModel);
}

bool FScene::RemoveDirectionalLight(const TSharedPtr<FDirectionalLight>& InDirectionalLight)
{
	return VisibleDirectionalLights.RemoveFirst(InDirectionalLight) || InvisibleDirectionalLights.RemoveFirst(InDirectionalLight);
}

bool FScene::RemovePointLight(const TSharedPtr<FPointLight>& InPointLight)
{
	return VisiblePointLights.RemoveFirst(InPointLight) || InvisiblePointLights.RemoveFirst(InPointLight);
}

void FScene::SetModelVisible(const FStringId& InModelName)
{
	const TSharedPtr<FModel>* ModelPtr = InvisibleModels.FindByPredicate(
		[&InModelName](const TSharedPtr<FModel>& InModel)
		{
			return InModel->GetName() == InModelName;
		}
	);

	if (ModelPtr)
	{
		const TSharedPtr<FModel>& Model = *ModelPtr;
		VisibleModels.Add(Model);
		InvisibleModels.RemoveFirst(Model);
	}
}

void FScene::SetModelInvisible(const FStringId& InModelName)
{
	const TSharedPtr<FModel>* ModelPtr = VisibleModels.FindByPredicate(
		[&InModelName](const TSharedPtr<FModel>& InModel)
		{
			return InModel->GetName() == InModelName;
		}
	);

	if (ModelPtr)
	{
		const TSharedPtr<FModel>& Model = *ModelPtr;
		InvisibleModels.Add(Model);
		VisibleModels.RemoveFirst(Model);
	}
}

void FScene::SetDirectionalLightVisible(const FStringId& InDirectionalLightName)
{
	const TSharedPtr<FDirectionalLight>* DirectionalLightPtr = InvisibleDirectionalLights.FindByPredicate(
		[&InDirectionalLightName](const TSharedPtr<FDirectionalLight>& InDirectionalLight)
		{
			return InDirectionalLight->GetName() == InDirectionalLightName;
		}
	);

	if (DirectionalLightPtr)
	{
		const TSharedPtr<FDirectionalLight>& DirectionalLight = *DirectionalLightPtr;
		VisibleDirectionalLights.Add(DirectionalLight);
		InvisibleDirectionalLights.RemoveFirst(DirectionalLight);
	}
}

void FScene::SetDirectionalLightInvisible(const FStringId& InDirectionalLightName)
{
	const TSharedPtr<FDirectionalLight>* DirectionalLightPtr = VisibleDirectionalLights.FindByPredicate(
		[&InDirectionalLightName](const TSharedPtr<FDirectionalLight>& InDirectionalLight)
		{
			return InDirectionalLight->GetName() == InDirectionalLightName;
		}
	);

	if (DirectionalLightPtr)
	{
		const TSharedPtr<FDirectionalLight>& DirectionalLight = *DirectionalLightPtr;
		InvisibleDirectionalLights.Add(DirectionalLight);
		VisibleDirectionalLights.RemoveFirst(DirectionalLight);
	}
}

void FScene::SetPointLightVisible(const FStringId& InPointLightName)
{
	const TSharedPtr<FPointLight>* PointLightPtr = InvisiblePointLights.FindByPredicate(
		[&InPointLightName](const TSharedPtr<FPointLight>& InPointLight)
		{
			return InPointLight->GetName() == InPointLightName;
		}
	);

	if (PointLightPtr)
	{
		const TSharedPtr<FPointLight>& PointLight = *PointLightPtr;
		VisiblePointLights.Add(PointLight);
		InvisiblePointLights.RemoveFirst(PointLight);
	}
}

void FScene::SetPointLightInvisible(const FStringId& InPointLightName)
{
	const TSharedPtr<FPointLight>* PointLightPtr = VisiblePointLights.FindByPredicate(
		[&InPointLightName](const TSharedPtr<FPointLight>& InPointLight)
		{
			return InPointLight->GetName() == InPointLightName;
		}
	);

	if (PointLightPtr)
	{
		const TSharedPtr<FPointLight>& PointLight = *PointLightPtr;
		InvisiblePointLights.Add(PointLight);
		VisiblePointLights.RemoveFirst(PointLight);
	}
}

bool FScene::IsModelVisible(const FStringId& InModelName)
{
	return VisibleModels.IsContainedByPredicate(
		[&InModelName](const TSharedPtr<FModel>& InModel)
		{
			return InModel->GetName() == InModelName;
		}
	);
}

bool FScene::IsDirectionalLightVisible(const FStringId& InDirectionalLightName)
{
	return VisibleDirectionalLights.IsContainedByPredicate(
		[&InDirectionalLightName](const TSharedPtr<FDirectionalLight>& InDirectionalLight)
		{
			return InDirectionalLight->GetName() == InDirectionalLightName;
		}
	);
}

bool FScene::IsPointLightVisible(const FStringId& InPointLightName)
{
	return VisiblePointLights.IsContainedByPredicate(
		[&InPointLightName](const TSharedPtr<FPointLight>& InPointLight)
		{
			return InPointLight->GetName() == InPointLightName;
		}
	);
}

bool FScene::IsModelContained(const FStringId& InModelName)
{
	auto IsSameName = [&InModelName](const TSharedPtr<FModel>& InModel)
	{
		return InModel->GetName() == InModelName;
	};

	// First check if the model is in VisibleModels, then check if it's in InvisibleModels.
	return VisibleModels.IsContainedByPredicate(IsSameName) || InvisibleModels.IsContainedByPredicate(IsSameName);
}

bool FScene::IsDirectionalLightContained(const FStringId& InDirectionalLightName)
{
	auto IsSameName = [&InDirectionalLightName](const TSharedPtr<FDirectionalLight>& InDirectionalLight)
	{
		return InDirectionalLight->GetName() == InDirectionalLightName;
	};

	return VisibleDirectionalLights.IsContainedByPredicate(IsSameName) || InvisibleDirectionalLights.IsContainedByPredicate(IsSameName);
}

bool FScene::IsPointLightContained(const FStringId& InPointLightName)
{
	auto IsSameName = [&InPointLightName](const TSharedPtr<FPointLight>& InPointLight)
	{
		return InPointLight->GetName() == InPointLightName;
	};

	return VisiblePointLights.IsContainedByPredicate(IsSameName) || InvisiblePointLights.IsContainedByPredicate(IsSameName);
}
