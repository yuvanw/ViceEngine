#include "SceneFile.h"
#include "RendererFileSystem.h"

// Includes used for JSON parsing.
#include <fstream>
#include <string>
#include "json/json.hpp"

namespace
{
	// Fixed key names used in scene JSON files.
	struct FSceneFileKeys
	{
		static constexpr const ANSICHAR* Name = "Name";

		static constexpr const ANSICHAR* Skybox = "Skybox";
		static constexpr const ANSICHAR* Lights = "Lights";
		static constexpr const ANSICHAR* DirectionalLights = "Directional";
		static constexpr const ANSICHAR* PointLights = "Point";
		static constexpr const ANSICHAR* Direction = "Direction";
		static constexpr const ANSICHAR* Color = "Color";
		static constexpr const ANSICHAR* Attenuation = "Attenuation";
		static constexpr const ANSICHAR* ConstantAttenuationFactor = "Constant";
		static constexpr const ANSICHAR* LinearAttenuationFactor = "Linear";
		static constexpr const ANSICHAR* QuadraticAttenuationFactor = "Quadratic";
		static constexpr const ANSICHAR* Intensity = "Intensity";

		static constexpr const ANSICHAR* Models = "Models";
		static constexpr const ANSICHAR* Model = "Model";
		static constexpr const ANSICHAR* Transform = "Transform";
		static constexpr const ANSICHAR* Position = "Position";
		static constexpr const ANSICHAR* Rotation = "Rotation";
		static constexpr const ANSICHAR* Scale = "Scale";

		static constexpr const ANSICHAR* Textures = "Textures";
		static constexpr const ANSICHAR* Right = "Right";
		static constexpr const ANSICHAR* Left = "Left";
		static constexpr const ANSICHAR* Top = "Top";
		static constexpr const ANSICHAR* Bottom = "Bottom";
		static constexpr const ANSICHAR* Back = "Back";
		static constexpr const ANSICHAR* Front = "Front";
	};
}

// Helper functions for parsing.
static FTransformInfo ParseTransform(const nlohmann::json& InJsonObject);
static FAttenuationInfo ParseAttenuation(const nlohmann::json& InJsonObject);
static FVector3D ParseVector3D(const nlohmann::json& InJsonArray);
static float ParseFloat(const nlohmann::json& InJsonNumber);
static std::string ParseString(const nlohmann::json& InJsonString);

FSceneFile::FSceneFile(const FStringId& InSceneFileName)
{
	FStringId SceneFilePath = FRendererFileSystem::GetSceneFilePath(InSceneFileName);

	// Initialize JSON file object.
	std::ifstream SceneFileInputStream(SceneFilePath.GetString().GetData());
	nlohmann::json SceneFileJson;
	SceneFileInputStream >> SceneFileJson;
	
	// Parse skybox.
	if (SceneFileJson.contains(FSceneFileKeys::Skybox))
	{
		const auto& SkyboxJson = SceneFileJson[FSceneFileKeys::Skybox];
		ensure(SkyboxJson.contains(FSceneFileKeys::Name) && SkyboxJson[FSceneFileKeys::Name].is_string());
		ensure(SkyboxJson.contains(FSceneFileKeys::Textures) && SkyboxJson[FSceneFileKeys::Textures].is_object());

		const auto& Textures = SkyboxJson[FSceneFileKeys::Textures];
		ensure(Textures.contains(FSceneFileKeys::Right) && Textures[FSceneFileKeys::Right].is_string());
		ensure(Textures.contains(FSceneFileKeys::Left) && Textures[FSceneFileKeys::Left].is_string());
		ensure(Textures.contains(FSceneFileKeys::Top) && Textures[FSceneFileKeys::Top].is_string());
		ensure(Textures.contains(FSceneFileKeys::Bottom) && Textures[FSceneFileKeys::Bottom].is_string());
		ensure(Textures.contains(FSceneFileKeys::Back) && Textures[FSceneFileKeys::Back].is_string());
		ensure(Textures.contains(FSceneFileKeys::Front) && Textures[FSceneFileKeys::Front].is_string());

		FSkyboxInfo SkyboxInfo;
		SkyboxInfo.Name = ParseString(SkyboxJson[FSceneFileKeys::Name]).c_str();
		SkyboxInfo.RightTextureFileName = ParseString(Textures[FSceneFileKeys::Right]).c_str();
		SkyboxInfo.LeftTextureFileName = ParseString(Textures[FSceneFileKeys::Left]).c_str();
		SkyboxInfo.TopTextureFileName = ParseString(Textures[FSceneFileKeys::Top]).c_str();
		SkyboxInfo.BottomTextureFileName = ParseString(Textures[FSceneFileKeys::Bottom]).c_str();
		SkyboxInfo.BackTextureFileName = ParseString(Textures[FSceneFileKeys::Back]).c_str();
		SkyboxInfo.FrontTextureFileName = ParseString(Textures[FSceneFileKeys::Front]).c_str();

		Skybox = SkyboxInfo;
		bHasSkybox = true;
	}
	else
	{
		bHasSkybox = false;
	}

	// Parse lights.
	if (SceneFileJson.contains(FSceneFileKeys::Lights))
	{
		const auto& Lights = SceneFileJson[FSceneFileKeys::Lights];
		if (Lights.contains(FSceneFileKeys::DirectionalLights))
		{
			// Parse directional lights.
			for (const auto& DirectionalLight : Lights[FSceneFileKeys::DirectionalLights])
			{
				ensure(DirectionalLight.contains(FSceneFileKeys::Name) && DirectionalLight[FSceneFileKeys::Name].is_string());
				ensure(DirectionalLight.contains(FSceneFileKeys::Direction) && DirectionalLight[FSceneFileKeys::Direction].is_array());
				ensure(DirectionalLight.contains(FSceneFileKeys::Color) && DirectionalLight[FSceneFileKeys::Color].is_array());
				ensure(DirectionalLight.contains(FSceneFileKeys::Intensity) && DirectionalLight[FSceneFileKeys::Intensity].is_number());

				FDirectionalLightInfo DirectionalLightInfo;
				DirectionalLightInfo.Name = ParseString(DirectionalLight[FSceneFileKeys::Name]).c_str();
				DirectionalLightInfo.Direction = ParseVector3D(DirectionalLight[FSceneFileKeys::Direction]);
				DirectionalLightInfo.Color = ParseVector3D(DirectionalLight[FSceneFileKeys::Color]);
				DirectionalLightInfo.Intensity = ParseFloat(DirectionalLight[FSceneFileKeys::Intensity]);
				DirectionalLights.Add(DirectionalLightInfo);
			}
		}

		if (Lights.contains(FSceneFileKeys::PointLights))
		{
			// Parse point lights.
			for (const auto& PointLight : Lights[FSceneFileKeys::PointLights])
			{
				ensure(PointLight.contains(FSceneFileKeys::Name) && PointLight[FSceneFileKeys::Name].is_string());
				ensure(PointLight.contains(FSceneFileKeys::Position) && PointLight[FSceneFileKeys::Position].is_array());
				ensure(PointLight.contains(FSceneFileKeys::Color) && PointLight[FSceneFileKeys::Color].is_array());
				ensure(PointLight.contains(FSceneFileKeys::Attenuation) && PointLight[FSceneFileKeys::Attenuation].is_object());
				ensure(PointLight.contains(FSceneFileKeys::Intensity) && PointLight[FSceneFileKeys::Intensity].is_number());

				FPointLightInfo PointLightInfo;
				PointLightInfo.Name = ParseString(PointLight[FSceneFileKeys::Name]).c_str();
				PointLightInfo.Position = ParseVector3D(PointLight[FSceneFileKeys::Position]);
				PointLightInfo.Color = ParseVector3D(PointLight[FSceneFileKeys::Color]);
				PointLightInfo.Attenuation = ParseAttenuation(PointLight[FSceneFileKeys::Attenuation]);
				PointLightInfo.Intensity = ParseFloat(PointLight[FSceneFileKeys::Intensity]);
				PointLights.Add(PointLightInfo);
			}
		}
	}

	// Parse models.
	if (SceneFileJson.contains(FSceneFileKeys::Models))
	{
		for (const auto& Model : SceneFileJson[FSceneFileKeys::Models])
		{
			ensure(Model.contains(FSceneFileKeys::Name) && Model[FSceneFileKeys::Name].is_string());
			ensure(Model.contains(FSceneFileKeys::Model) && Model[FSceneFileKeys::Model].is_string());
			ensure(Model.contains(FSceneFileKeys::Transform) && Model[FSceneFileKeys::Transform].is_object());

			FModelInfo ModelInfo;
			ModelInfo.Name = ParseString(Model[FSceneFileKeys::Name]).c_str();
			ModelInfo.FileName = ParseString(Model[FSceneFileKeys::Model]).c_str();
			ModelInfo.Transform = ParseTransform(Model[FSceneFileKeys::Transform]);
			Models.Add(ModelInfo);
		}
	}
}

static FTransformInfo ParseTransform(const nlohmann::json& InJsonObject)
{
	ensure(InJsonObject.contains(FSceneFileKeys::Position) && InJsonObject[FSceneFileKeys::Position].is_array());
	ensure(InJsonObject.contains(FSceneFileKeys::Rotation) && InJsonObject[FSceneFileKeys::Rotation].is_array());
	ensure(InJsonObject.contains(FSceneFileKeys::Scale) && InJsonObject[FSceneFileKeys::Scale].is_array());

	FTransformInfo TransformInfo;
	TransformInfo.Position = ParseVector3D(InJsonObject[FSceneFileKeys::Position]);
	TransformInfo.Rotation = ParseVector3D(InJsonObject[FSceneFileKeys::Rotation]);
	TransformInfo.Scale = ParseVector3D(InJsonObject[FSceneFileKeys::Scale]);
	return TransformInfo;
}

static FAttenuationInfo ParseAttenuation(const nlohmann::json& InJsonObject)
{
	ensure(InJsonObject.contains(FSceneFileKeys::ConstantAttenuationFactor) && InJsonObject[FSceneFileKeys::ConstantAttenuationFactor].is_number());
	ensure(InJsonObject.contains(FSceneFileKeys::LinearAttenuationFactor) && InJsonObject[FSceneFileKeys::LinearAttenuationFactor].is_number());
	ensure(InJsonObject.contains(FSceneFileKeys::QuadraticAttenuationFactor) && InJsonObject[FSceneFileKeys::QuadraticAttenuationFactor].is_number());

	FAttenuationInfo AttenuationInfo;
	AttenuationInfo.Constant = ParseFloat(InJsonObject[FSceneFileKeys::ConstantAttenuationFactor]);
	AttenuationInfo.Linear = ParseFloat(InJsonObject[FSceneFileKeys::LinearAttenuationFactor]);
	AttenuationInfo.Quadratic = ParseFloat(InJsonObject[FSceneFileKeys::QuadraticAttenuationFactor]);
	return AttenuationInfo;
}

static FVector3D ParseVector3D(const nlohmann::json& InJsonArray)
{
	ensure(InJsonArray[0].is_number());
	ensure(InJsonArray[1].is_number());
	ensure(InJsonArray[2].is_number());
	return FVector3D(InJsonArray[0], InJsonArray[1], InJsonArray[2]);
}

static float ParseFloat(const nlohmann::json& InJsonNumber)
{
	return InJsonNumber.get<float>();
}

static std::string ParseString(const nlohmann::json& InJsonString)
{
	return InJsonString.get<std::string>();
}
