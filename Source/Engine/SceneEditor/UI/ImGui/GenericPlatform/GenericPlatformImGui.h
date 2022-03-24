#pragma once

#include "Events.h"

class FGenericPlatformImGui
{
public:
	// Empty default implementations.
	static bool Init(void* InNativeWindowHandle)
	{
		return false;
	}

	static void Shutdown()
	{
	}

	static void NewFrame()
	{
	}

	static void OnKeyDown(const FKeyEvent& InKeyEvent)
	{
	}

	static void OnKeyUp(const FKeyEvent& InKeyEvent)
	{
	}

	static void OnMouseDown(const FMouseButtonEvent& InMouseButtonEvent)
	{
	}

	static void OnMouseUp(const FMouseButtonEvent& InMouseButtonEvent)
	{
	}
};
