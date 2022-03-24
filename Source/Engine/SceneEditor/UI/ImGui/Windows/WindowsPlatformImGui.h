#pragma once

#include "GenericPlatform/GenericPlatformImGui.h"

class FWindowsPlatformImGui : public FGenericPlatformImGui
{
public:
	// Begin FGenericPlatformImGui interface.
	static bool Init(void* InNativeWindowHandle);
	static void Shutdown();
	static void NewFrame();

	static void OnKeyDown(const FKeyEvent& InKeyEvent);
	static void OnKeyUp(const FKeyEvent& InKeyEvent);
	static void OnMouseDown(const FMouseButtonEvent& InMouseButtonEvent);
	static void OnMouseUp(const FMouseButtonEvent& InMouseButtonEvent);
	// End FGenericPlatformImGui interface.
};

using FPlatformImGui = FWindowsPlatformImGui;
