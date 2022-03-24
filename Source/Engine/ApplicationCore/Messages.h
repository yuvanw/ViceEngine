#pragma once

#include "CoreMinimal.h"

enum class EMouseButton : uint8
{
	Left,
	Right,
	Middle
};

// Platform-abstracted OS messages.
struct FKeyMessage
{
	FKeyMessage(int32 InVirtualKeyCode)
		: VirtualKeyCode(InVirtualKeyCode)
	{
	}

	int32 VirtualKeyCode;
};

struct FMouseButtonMessage
{
	FMouseButtonMessage(EMouseButton InButton, void* InNativeWindowHandle)
		: Button(InButton)
		, WindowHandle(InNativeWindowHandle)
	{
	}

	EMouseButton Button;
	void* WindowHandle;
};

struct FMouseMoveMessage
{
	FMouseMoveMessage(const FVector2D& InMousePosition)
		: MousePosition(InMousePosition)
	{
	}

	FVector2D MousePosition;
};

struct FMouseScrollMessage
{
	FMouseScrollMessage(float InScrollDelta)
		: ScrollDelta(InScrollDelta)
	{
	}

	float ScrollDelta;
};

struct FWindowResizeMessage
{
	FWindowResizeMessage(int32 InWidth, int32 InHeight)
		: Width(InWidth)
		, Height(InHeight)
	{
	}

	int32 Width;
	int32 Height;
};
