#pragma once

#include "CoreMinimal.h"

/**
 * Utilities functions are declared under the ImGui namespace so that they 
 * are integrated with existing ImGui library functions.
 */
namespace ImGui
{
	void Vector3(const ANSICHAR* InLabel, FVector3D& OutVector, float InMin = 0.0f, float InMax = 0.0f);
	void Transform(FVector3D& OutPosition, FVector3D& OutRotation, FVector3D& OutScale);
	void ComboBox(const ANSICHAR* InLabel, const TArray<FStringId>& InDropdownElements, const ANSICHAR* InInitialComboValue, int32& OutComboIndex);
};
