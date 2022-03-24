#include "OutlinerUI.h"
#include "RenderManager.h"
#include "RendererFileSystem.h"

#include "imgui/imgui.h"

#include <string>

const ANSICHAR* FOutlinerUI::WindowName = "Outliner";

void FOutlinerUI::Init()
{
	Scene = FRenderManager::GetRenderer()->GetScene();
	Camera = FRenderManager::GetRenderer()->GetCamera();

	// Store scene objects.
	Skybox = Scene->GetSkybox();
	
	Models.Empty();
	Models.Append(Scene->GetVisibleModels());
	Models.Append(Scene->GetInvisibleModels());
	
	DirectionalLights.Empty();
	DirectionalLights.Append(Scene->GetVisibleDirectionalLights());
	DirectionalLights.Append(Scene->GetInvisibleDirectionalLights());
	
	PointLights.Empty();
	PointLights.Append(Scene->GetVisiblePointLights());
	PointLights.Append(Scene->GetInvisiblePointLights());

	// Select the camera by default.
	SelectedObjectIndex = 0;
	SelectedSceneIndex = 0;
	SelectedAddIndex = 0;
}

void FOutlinerUI::Shutdown()
{
}

void FOutlinerUI::Update()
{
	ImGui::Begin(WindowName);

	// List scene configuration buttons.
	RenderLoadSceneButton();
	ImGui::SameLine();
	RenderAddButton();
	ImGui::SameLine();
	RenderRemoveButton();

	ImGui::Spacing();

	// List camera.
	int32 CurrentObjectIndex = 0;
	if (ImGui::Selectable("Camera", SelectedObjectIndex == CurrentObjectIndex))
	{
		SelectedObjectIndex = CurrentObjectIndex;
	}

	// List skybox.
	CurrentObjectIndex = 1;
	if (Skybox)
	{
		bool bIsVisible = Skybox->IsVisible();
		std::string VisibilityLabel = std::string("##IsSkyboxVisible") + Skybox->GetName().GetString().GetData();
		ImGui::Checkbox(VisibilityLabel.c_str(), &bIsVisible);
		ImGui::SameLine();

		if (bIsVisible)
		{
			Skybox->SetVisible();
		}
		else
		{
			Skybox->SetInvisible();
		}

		if (ImGui::Selectable(Skybox->GetName().GetString().GetData(), SelectedObjectIndex == CurrentObjectIndex))
		{
			SelectedObjectIndex = CurrentObjectIndex;
		}

		++CurrentObjectIndex;
	}

	// List models.
	int32 StartIndex = CurrentObjectIndex;
	int32 EndIndex = CurrentObjectIndex + Models.GetSize();
	for (CurrentObjectIndex; CurrentObjectIndex < EndIndex; ++CurrentObjectIndex)
	{
		int32 Index = CurrentObjectIndex - StartIndex;

		bool bIsVisible = Models[Index]->IsVisible();
		std::string VisibilityLabel = std::string("##IsVisible") + Models[Index]->GetName().GetString().GetData();
		ImGui::Checkbox(VisibilityLabel.c_str(), &bIsVisible);
		ImGui::SameLine();

		if (bIsVisible)
		{
			Models[Index]->SetVisible();
		}
		else
		{
			Models[Index]->SetInvisible();
		}

		if (ImGui::Selectable(Models[Index]->GetName().GetString().GetData(), SelectedObjectIndex == CurrentObjectIndex))
		{
			SelectedObjectIndex = CurrentObjectIndex;
		}
	}

	// List directional lights.
	StartIndex = CurrentObjectIndex;
	EndIndex = CurrentObjectIndex + DirectionalLights.GetSize();
	for (CurrentObjectIndex; CurrentObjectIndex < EndIndex; ++CurrentObjectIndex)
	{
		int32 Index = CurrentObjectIndex - StartIndex;

		bool bIsVisible = DirectionalLights[Index]->IsVisible();
		std::string VisibilityLabel = std::string("##IsVisible") + DirectionalLights[Index]->GetName().GetString().GetData();
		ImGui::Checkbox(VisibilityLabel.c_str(), &bIsVisible);
		ImGui::SameLine();

		if (bIsVisible)
		{
			DirectionalLights[Index]->SetVisible();
		}
		else
		{
			DirectionalLights[Index]->SetInvisible();
		}

		if (ImGui::Selectable(DirectionalLights[Index]->GetName().GetString().GetData(), SelectedObjectIndex == CurrentObjectIndex))
		{
			SelectedObjectIndex = CurrentObjectIndex;
		}
	}

	// List point lights.
	StartIndex = CurrentObjectIndex;
	EndIndex = CurrentObjectIndex + PointLights.GetSize();
	for (CurrentObjectIndex; CurrentObjectIndex < EndIndex; ++CurrentObjectIndex)
	{
		int32 Index = CurrentObjectIndex - StartIndex;

		bool bIsVisible = PointLights[Index]->IsVisible();
		std::string VisibilityLabel = std::string("##IsVisible") + PointLights[Index]->GetName().GetString().GetData();
		ImGui::Checkbox(VisibilityLabel.c_str(), &bIsVisible);
		ImGui::SameLine();

		if (bIsVisible)
		{
			PointLights[Index]->SetVisible();
		}
		else
		{
			PointLights[Index]->SetInvisible();
		}

		if (ImGui::Selectable(PointLights[Index]->GetName().GetString().GetData(), SelectedObjectIndex == CurrentObjectIndex))
		{
			SelectedObjectIndex = CurrentObjectIndex;
		}
	}

	ImGui::End();
}

TSharedPtr<FCamera> FOutlinerUI::GetSelectedCamera()
{
	return (SelectedObjectIndex == 0) ? Camera : nullptr;
}

TSharedPtr<FSkybox> FOutlinerUI::GetSelectedSkybox()
{
	if (!Skybox)
	{
		return nullptr;
	}

	return (SelectedObjectIndex == 1) ? Skybox : nullptr;
}

TSharedPtr<FModel> FOutlinerUI::GetSelectedModel()
{
	int32 FirstModelIndex = (Skybox) ? 2 : 1;
	int32 LastModelIndex = FirstModelIndex + Models.GetSize() - 1;
	bool bIsSelected = !Models.IsEmpty() && (FirstModelIndex <= SelectedObjectIndex) && (SelectedObjectIndex <= LastModelIndex);
	if (bIsSelected)
	{
		int32 Index = SelectedObjectIndex - FirstModelIndex;
		return Models[Index];
	}
	else 
	{
		return nullptr;
	}
}

TSharedPtr<FDirectionalLight> FOutlinerUI::GetSelectedDirectionalLight()
{
	int32 NumModels = Models.GetSize();
	int32 FirstDirectionalLightIndex = (Skybox) ? NumModels + 2 : NumModels + 1;
	int32 LastDirectionalLightIndex = FirstDirectionalLightIndex + DirectionalLights.GetSize() - 1;
	bool bIsSelected = !DirectionalLights.IsEmpty() && (FirstDirectionalLightIndex <= SelectedObjectIndex) && (SelectedObjectIndex <= LastDirectionalLightIndex);
	
	if (bIsSelected)
	{
		int32 Index = SelectedObjectIndex - FirstDirectionalLightIndex;
		return DirectionalLights[Index];
	}
	else
	{
		return nullptr;
	}
}

TSharedPtr<FPointLight> FOutlinerUI::GetSelectedPointLight()
{
	int32 NumModels = Models.GetSize();
	int32 NumDirectionalLights = DirectionalLights.GetSize();
	int32 FirstPointLightIndex = (Skybox) ? NumModels + NumDirectionalLights + 2 : NumModels + NumDirectionalLights + 1;
	int32 LastPointLightIndex = FirstPointLightIndex + PointLights.GetSize() - 1;
	bool bIsSelected = !PointLights.IsEmpty() && (FirstPointLightIndex <= SelectedObjectIndex) && (SelectedObjectIndex <= LastPointLightIndex);

	if (bIsSelected)
	{
		int32 Index = SelectedObjectIndex - FirstPointLightIndex;
		return PointLights[Index];
	}
	else
	{
		return nullptr;
	}
}

void FOutlinerUI::RenderLoadSceneButton()
{
	if (ImGui::Button("Load Scene"))
	{
		ImGui::OpenPopup("Load Scene");
	}

	ImVec2 Center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(Center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	if (ImGui::BeginPopupModal("Load Scene", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		TArray<FStringId> SceneFileNames = FRendererFileSystem::GetAllSceneFileNames();
		for (int32 Index = 0; Index < SceneFileNames.GetSize(); ++Index)
		{
			if (ImGui::Selectable(SceneFileNames[Index].GetString().GetData(), SelectedSceneIndex == Index, ImGuiSelectableFlags_DontClosePopups))
			{
				SelectedSceneIndex = Index;
			}
		}

		ImGui::Separator();

		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			FRenderManager::GetRenderer()->SetScene(SceneFileNames[SelectedSceneIndex].GetString().GetData());
			Init();
			ImGui::CloseCurrentPopup();
		}
		
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void FOutlinerUI::RenderAddButton()
{
	if (ImGui::Button("Add"))
	{
		ImGui::OpenPopup("Add");
	}

	ImVec2 Center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(Center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	if (ImGui::BeginPopupModal("Add", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Separator();
		ImGui::Text("Name");
		ImGui::Separator();

		static char ObjectName[128] = "New Object";
		ImGui::InputText("##Name", ObjectName, IM_ARRAYSIZE(ObjectName));

		bool bIsNameUnique = true;
		if (Scene->IsModelContained(ObjectName) || Scene->IsDirectionalLightContained(ObjectName) || Scene->IsPointLightContained(ObjectName))
		{
			bIsNameUnique = false;
			ImGui::Text("Object name must be unique!");
		}

		ImGui::Separator();
		ImGui::Text("Models");
		ImGui::Separator();

		int32 Index = 0;
		TArray<FStringId> ModelFileNames = FRendererFileSystem::GetAllModelFileNames();
		for (Index; Index < ModelFileNames.GetSize(); ++Index)
		{
			if (ImGui::Selectable(ModelFileNames[Index].GetString().GetData(), SelectedAddIndex == Index, ImGuiSelectableFlags_DontClosePopups))
			{
				SelectedAddIndex = Index;
			}
		}

		ImGui::Separator();
		ImGui::Text("Lights");
		ImGui::Separator();

		if (ImGui::Selectable("Directional Light", SelectedAddIndex == Index, ImGuiSelectableFlags_DontClosePopups))
		{
			SelectedAddIndex = Index;
		}

		++Index;

		if (ImGui::Selectable("Point Light", SelectedAddIndex == Index, ImGuiSelectableFlags_DontClosePopups))
		{
			SelectedAddIndex = Index;
		}

		if (!bIsNameUnique)
		{
			ImGui::BeginDisabled();
		}

		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			int32 NumModels = ModelFileNames.GetSize();
			bool bIsModelSelected = (SelectedAddIndex < NumModels);
			bool bIsDirectionalLightSelected = (SelectedAddIndex == NumModels);
			bool bIsPointLightSelected = (SelectedAddIndex == NumModels + 1);

			if (bIsModelSelected)
			{
				// Add selected model to scene.
				TSharedPtr<FModel> Model = MakeShared<FModel>(ObjectName, ModelFileNames[SelectedAddIndex].GetString().GetData());
				Scene->AddModel(Model);
				Models.Add(Model);
			}
			else if (bIsDirectionalLightSelected)
			{
				// Add default directional light.
				TSharedPtr<FDirectionalLight> DirectionalLight = MakeShared<FDirectionalLight>(
					// Name.
					ObjectName,
					// Light direction.
					FVector3D(0.0f, 0.0f, 0.0f),
					// Light color.
					FColor(1.0f, 1.0f, 1.0f),
					// Light intensity.
					1.0f);
				Scene->AddDirectionalLight(DirectionalLight);
				DirectionalLights.Add(DirectionalLight);
			}
			else if (bIsPointLightSelected)
			{
				// Add default point light.
				TSharedPtr<FPointLight> PointLight = MakeShared<FPointLight>(
					// Name
					ObjectName,
					// Light position.
					FVector3D(0.0f, 0.0f, 0.0f),
					// Light color.
					FColor(1.0f, 1.0f, 1.0f),
					// Light intensity.
					1.0f,
					// Light attenuation.
					FAttenuation(1.0f, 0.032f, 0.09f)
					);
				Scene->AddPointLight(PointLight);
				PointLights.Add(PointLight);
			}

			// Reset default object name.
			strcpy(ObjectName, "New Object");

			ImGui::CloseCurrentPopup();
		}

		ImGui::SetItemDefaultFocus();

		if (!bIsNameUnique)
		{
			ImGui::EndDisabled();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
		}
		
		ImGui::EndPopup();
	}
}

void FOutlinerUI::RenderRemoveButton()
{
	if (GetSelectedCamera())
	{
		// Disable the "Remove" button if the camera is selected.
		ImGui::BeginDisabled();
		ImGui::Button("Remove");
		ImGui::EndDisabled();
	}
	else if (ImGui::Button("Remove"))
	{
		if (GetSelectedSkybox())
		{
			Scene->SetSkybox(nullptr);
		}
		else if (TSharedPtr<FModel> Model = GetSelectedModel())
		{
			Scene->RemoveModel(Model);
			Models.RemoveFirst(Model);
		}
		else if (TSharedPtr<FDirectionalLight> DirectionalLight = GetSelectedDirectionalLight())
		{
			Scene->RemoveDirectionalLight(DirectionalLight);
			DirectionalLights.RemoveFirst(DirectionalLight);
		}
		else if (TSharedPtr<FPointLight> PointLight = GetSelectedPointLight())
		{
			Scene->RemovePointLight(PointLight);
			PointLights.RemoveFirst(PointLight);
		}
		
		SelectedObjectIndex = 0;
	}
}
