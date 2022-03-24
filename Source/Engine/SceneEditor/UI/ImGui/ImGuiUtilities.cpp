#include "ImGuiUtilities.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

static constexpr ImVec4 XAxisRed = { 0.9f, 0.2f, 0.2f, 1.0f };
static constexpr ImVec4 YAxisGreen = { 0.3f, 0.8f, 0.3f, 1.0f };
static constexpr ImVec4 ZAxisBlue = { 0.2f, 0.35f, 0.9f, 1.0f };

void ImGui::Vector3(const ANSICHAR* InLabel, FVector3D& OutVector, float InMin, float InMax)
{
	ImGui::PushID(InLabel);

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	float LineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 ButtonSize = { LineHeight + 3.0f, LineHeight };

	ImGui::PushStyleColor(ImGuiCol_Button, XAxisRed);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, XAxisRed);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, XAxisRed);
	ImGui::Button("X", ButtonSize);
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##X", &OutVector.X, 0.1f, InMin, InMax);
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, YAxisGreen);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, YAxisGreen);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, YAxisGreen);
	ImGui::Button("Y", ButtonSize);
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Y", &OutVector.Y, 0.1f, InMin, InMax);
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ZAxisBlue);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ZAxisBlue);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ZAxisBlue);
	ImGui::Button("Z", ButtonSize);
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Z", &OutVector.Z, 0.1f, InMin, InMax);
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::PopID();

	ImGui::Spacing();
}

void ImGui::Transform(FVector3D& OutPosition, FVector3D& OutRotation, FVector3D& OutScale)
{
	ImGui::Separator();
	ImGui::Text("Position");
	ImGui::Separator();

	Vector3("Position", OutPosition);

	ImGui::Separator();
	ImGui::Text("Rotation");
	ImGui::Separator();

	// Display rotation in degrees.
	float RotationDegreesX = FMath::RadiansToDegrees(OutRotation.X);
	float RotationDegreesY = FMath::RadiansToDegrees(OutRotation.Y);
	float RotationDegreesZ = FMath::RadiansToDegrees(OutRotation.Z);
	FVector3D RotationInDegrees = { RotationDegreesX , RotationDegreesY, RotationDegreesZ };
	
	Vector3("Rotation", RotationInDegrees);
	
	float RotationRadiansX = FMath::DegreesToRadians(RotationInDegrees.X);
	float RotationRadiansY = FMath::DegreesToRadians(RotationInDegrees.Y);
	float RotationRadiansZ = FMath::DegreesToRadians(RotationInDegrees.Z);
	OutRotation = { RotationRadiansX, RotationRadiansY, RotationRadiansZ };

	ImGui::Separator();
	ImGui::Text("Scale");
	ImGui::Separator();

	Vector3("Scale", OutScale);
}

void ImGui::ComboBox(const ANSICHAR* InLabel, const TArray<FStringId>& InDropdownElements, const ANSICHAR* InInitialComboValue, int32& OutComboIndex)
{
	ImGui::PushID(InLabel);

	if (ImGui::BeginCombo("##Combo", InInitialComboValue))
	{
		for (int Index = 0; Index < InDropdownElements.GetSize(); ++Index)
		{
			const bool IsSelected = (OutComboIndex == Index);
			if (ImGui::Selectable(InDropdownElements[Index].GetString().GetData(), IsSelected))
			{
				OutComboIndex = Index;
			}

			if (IsSelected)
			{
				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	ImGui::PopID();
}
