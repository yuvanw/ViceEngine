#include "InspectorUI.h"
#include "RendererFileSystem.h"
#include "Camera/Camera.h"
#include "Lights/DirectionalLight.h"
#include "Lights/PointLight.h"
#include "Geometry/Model.h"
#include "Geometry/Mesh.h"
#include "Materials/Material.h"

#include "imgui/imgui.h"
#include "ImGui/ImGuiUtilities.h"

#include <string>

const ANSICHAR* FInspectorUI::WindowName = "Inspector";

void FInspectorUI::Init()
{
	ClearObjects();

	for (const FStringId& MaterialFileName : FRendererFileSystem::GetAllMaterialFileNames())
	{
		MaterialFileNames.Add(MaterialFileName.GetString().GetData());
	}

	for (const FStringId& TextureFileName : FRendererFileSystem::GetAllTextureFileNames())
	{
		TextureFileNames.Add(TextureFileName.GetString().GetData());
	}
}

void FInspectorUI::Shutdown()
{
}

void FInspectorUI::Update()
{
	ImGui::Begin(WindowName);

	if (Camera)
	{
		RenderCameraInfo();
	}

	if (Model)
	{
		RenderModelInfo();
	}
	
	if (DirectionalLight)
	{
		RenderDirectionalLightInfo();
	}
	
	if (PointLight)
	{
		RenderPointLightInfo();
	}

	ImGui::End();
}

void FInspectorUI::SetSelected(const TSharedPtr<FCamera>& InCamera)
{
	ClearObjects();
	Camera = InCamera;
}

void FInspectorUI::SetSelected(const TSharedPtr<FSkybox>& InSkybox)
{
	ClearObjects();
	Skybox = InSkybox;
}

void FInspectorUI::SetSelected(const TSharedPtr<FModel>& InModel)
{
	ClearObjects();
	Model = InModel;
}

void FInspectorUI::SetSelected(const TSharedPtr<FDirectionalLight>& InDirectionalLight)
{
	ClearObjects();
	DirectionalLight = InDirectionalLight;
}

void FInspectorUI::SetSelected(const TSharedPtr<FPointLight>& InPointLight)
{
	ClearObjects();
	PointLight = InPointLight;
}

void FInspectorUI::RenderCameraInfo()
{
	ImGui::Separator();
	ImGui::Text("Projection Type");
	ImGui::Separator();

	const char* ProjectionTypes[] = { "Perspective", "Orthographic" };
	static int SelectedProjectionType = 0;
	ImGui::Combo("##ProjectionType", &SelectedProjectionType, ProjectionTypes, IM_ARRAYSIZE(ProjectionTypes));

	if (SelectedProjectionType == 0)
	{
		Camera->SetProjectionType(EProjectionType::Perspective);
	}
	else
	{
		Camera->SetProjectionType(EProjectionType::Orthographic);
	}

	ImGui::Separator();
	ImGui::Text("Near Plane Distance");
	ImGui::Separator();

	float NearPlaneDistance = Camera->GetNearPlaneDistance();
	ImGui::DragFloat("##NearPlaneDistance", &NearPlaneDistance, 0.01f, 0.0f);
	Camera->SetNearPlaneDistance(NearPlaneDistance);

	ImGui::Separator();
	ImGui::Text("Far Plane Distance");
	ImGui::Separator();

	float FarPlaneDistance = Camera->GetFarPlaneDistance();
	ImGui::DragFloat("##FarPlaneDistance", &FarPlaneDistance, 0.01f, 0.0f);
	Camera->SetFarPlaneDistance(FarPlaneDistance);
}

void FInspectorUI::RenderModelInfo()
{
	// Render transform info.
	FVector3D Position = Model->GetPosition();
	FVector3D Rotation = Model->GetRotation();
	FVector3D Scale = Model->GetScale();
	ImGui::Transform(Position, Rotation, Scale);

	// Update transform if it has changed.
	if (Position != Model->GetPosition())
	{
		Model->SetPosition(Position);
	}
	if (Rotation != Model->GetRotation())
	{
		Model->SetRotation(Rotation);
	}
	if (Scale != Model->GetScale())
	{
		Model->SetScale(Scale);
	}

	// Render drawing mode options.
	ImGui::Separator();
	ImGui::Text("Drawing Mode");
	ImGui::Separator();

	const ANSICHAR* DrawingModesNames[3] = { "Filled", "Wireframe", "Points" };
	const EDrawingMode DrawingModes[3] = { EDrawingMode::Filled, EDrawingMode::Wireframe, EDrawingMode::Points };

	int32 DrawingModeIndex = 0;
	switch (Model->GetDrawingMode())
	{
	case EDrawingMode::Filled:
		DrawingModeIndex = 0;
		break;
	case EDrawingMode::Wireframe:
		DrawingModeIndex = 1;
		break;
	case EDrawingMode::Points:
		DrawingModeIndex = 2;
		break;
	default:
		// Unknown drawing mode.
		ensure(false);
		break;
	}
	int32 PrevDrawingModeIndex = DrawingModeIndex;
	ImGui::Combo("##DrawingMode", &DrawingModeIndex, DrawingModesNames, IM_ARRAYSIZE(DrawingModesNames));

	// Update drawing mode if a different one was chosen.
	if (DrawingModeIndex != PrevDrawingModeIndex)
	{
		Model->SetDrawingMode(DrawingModes[DrawingModeIndex]);
	}

	// Render material info per mesh.
	ImGui::Separator();
	ImGui::Text("Meshes");
	ImGui::Separator();

	// Show material info per mesh.
	TArray<FMesh>& Meshes = Model->GetMeshes();
	for (int32 Index = 0; Index < Meshes.GetSize(); ++Index)
	{
		FMesh& Mesh = Meshes[Index];
		if (ImGui::TreeNode(Mesh.GetName().GetString().GetData()))
		{
			RenderMaterialInfo(Mesh);
			ImGui::TreePop();
		}
	}
}

void FInspectorUI::RenderMaterialInfo(FMesh& InMesh)
{
	ImGui::Text("Material");
	ImGui::SameLine();

	// Display material file combo menu.
	const FStringId& MaterialFileName = InMesh.GetMaterial().GetName();
	int32 MaterialIndex = MaterialFileNames.GetIndexOfByPredicate(
		[&MaterialFileName](const FStringId& InMaterialFileName)
		{
			return InMaterialFileName == MaterialFileName;
		}
	);
	int32 PrevMaterialIndex = MaterialIndex;
	std::string MaterialLabel = std::string("##Material") + MaterialFileName.GetString().GetData();
	ImGui::Combo(MaterialLabel.c_str(), &MaterialIndex, MaterialFileNames.GetData(), MaterialFileNames.GetSize());

	// Update the mesh's material if a different material was chosen.
	if (MaterialIndex != PrevMaterialIndex)
	{
		InMesh.SetMaterial(MaterialFileNames[MaterialIndex]);
	}

	// Display the material's float properties.
	FMaterial& Material = InMesh.GetMaterial();
	if (!Material.GetFloatProperties().IsEmpty())
	{
		if (ImGui::TreeNode("Float Properties"))
		{
			const TArray<FMaterialProperty<float> >& FloatProperties = Material.GetFloatProperties();
			for (int32 Index = 0; Index < FloatProperties.GetSize(); ++Index)
			{
				const ANSICHAR* PropertyName = FloatProperties[Index].Name.GetString().GetData();
				float PropertyValue = FloatProperties[Index].Property;
				float PrevPropertyValue = PropertyValue;

				ImGui::Text(PropertyName);
				ImGui::SameLine();

				std::string FloatLabel = std::string("##Float") + PropertyName + InMesh.GetName().GetString().GetData();
				ImGui::DragFloat(FloatLabel.c_str(), &PropertyValue, 0.01f);

				// Update the float property if it was changed.
				if (PropertyValue != PrevPropertyValue)
				{
					Material.SetFloat(PropertyName, PropertyValue);
				}
			}
			ImGui::TreePop();
		}
	}

	// Display material color properties.
	if (!Material.GetColorProperties().IsEmpty())
	{
		if (ImGui::TreeNode("Color Properties"))
		{
			const TArray<FMaterialProperty<FColor> >& ColorProperties = Material.GetColorProperties();
			for (int32 Index = 0; Index < ColorProperties.GetSize(); ++Index)
			{
				const ANSICHAR* PropertyName = ColorProperties[Index].Name.GetString().GetData();
				FColor PropertyValue = ColorProperties[Index].Property;
				FColor PrevPropertyValue = PropertyValue;

				ImGui::Text(PropertyName);
				ImGui::SameLine();

				std::string ColorLabel = std::string("##Color") + PropertyName + InMesh.GetName().GetString().GetData();
				ImGui::ColorEdit3(ColorLabel.c_str(), (float*)&PropertyValue);

				// Update the color property if it was changed.
				if (PropertyValue != PrevPropertyValue)
				{
					Material.SetColor(PropertyName, PropertyValue);
				}
			}
			ImGui::TreePop();
		}
	}

	// Display the material's texture properties.
	if (!Material.GetTextureProperties().IsEmpty())
	{
		if (ImGui::TreeNode("Texture Properties"))
		{
			const TArray<FMaterialProperty<FTexture2D> >& TextureProperties = Material.GetTextureProperties();
			for (int32 Index = 0; Index < TextureProperties.GetSize(); ++Index)
			{
				const ANSICHAR* PropertyName = TextureProperties[Index].Name.GetString().GetData();
				FTexture2D PropertyValue = TextureProperties[Index].Property;

				ImGui::Text(PropertyName);
				ImGui::SameLine();

				// Display texture file combo menu.
				const FStringId& TextureFileName = TextureProperties[Index].Property.GetName();
				int32 TextureIndex = TextureFileNames.GetIndexOfByPredicate(
					[&TextureFileName](const FStringId& InTextureFileName)
					{
						return InTextureFileName == TextureFileName;
					}
				);
				int32 PrevTextureIndex = TextureIndex;
				std::string TextureLabel = std::string("##Texture") + PropertyName + InMesh.GetName().GetString().GetData();
				ImGui::Combo(TextureLabel.c_str(), &TextureIndex, TextureFileNames.GetData(), TextureFileNames.GetSize());

				// Update the texture property if it was changed.
				if (TextureIndex != PrevTextureIndex)
				{
					FTexture2D Texture(TextureFileNames[TextureIndex]);
					Material.SetTexture(PropertyName, Texture);
				}
			}
			ImGui::TreePop();
		}
	}
}

void FInspectorUI::RenderDirectionalLightInfo()
{
	ImGui::Separator();
	ImGui::Text("Direction");
	ImGui::Separator();

	FVector3D Direction = DirectionalLight->GetDirection();
	ImGui::Vector3("##Direction", Direction);
	DirectionalLight->SetDirection(Direction);

	ImGui::Separator();
	ImGui::Text("Intensity");
	ImGui::Separator();

	float Intensity = DirectionalLight->GetIntensity();
	ImGui::DragFloat("##Intensity", (float*)&Intensity, 0.01f, 0.0f, 1.0f);
	DirectionalLight->SetIntensity(Intensity);

	ImGui::Separator();
	ImGui::Text("Color");
	ImGui::Separator();

	FColor Color = DirectionalLight->GetColor();
	ImGui::ColorEdit3("##Color", (float*)&Color);
	DirectionalLight->SetColor(Color);
}

void FInspectorUI::RenderPointLightInfo()
{
	ImGui::Separator();
	ImGui::Text("Position");
	ImGui::Separator();

	FVector3D Position = PointLight->GetPosition();
	ImGui::Vector3("##Position", Position);
	PointLight->SetPosition(Position);

	ImGui::Separator();
	ImGui::Text("Intensity");
	ImGui::Separator();

	float Intensity = PointLight->GetIntensity();
	ImGui::DragFloat("##Intensity", (float*)&Intensity, 0.01f, 0.0f, 1.0f);
	PointLight->SetIntensity(Intensity);

	ImGui::Separator();
	ImGui::Text("Color");
	ImGui::Separator();

	FColor Color = PointLight->GetColor();
	ImGui::ColorEdit3("##Color", (float*)&Color);
	PointLight->SetColor(Color);
}

void FInspectorUI::ClearObjects()
{
	Camera = nullptr;
	Skybox = nullptr;
	Model = nullptr;
	DirectionalLight = nullptr;
	PointLight = nullptr;
}
