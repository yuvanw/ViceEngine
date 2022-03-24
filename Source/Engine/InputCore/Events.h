#pragma once

#include "Keys.h"

// Input event data wrappers.
class FKeyEvent
{
public:
	FKeyEvent(const FKey& InKey, int32 InVirtualKeyCode)
		: Key(InKey)
		, VirtualKeyCode(InVirtualKeyCode)
	{
	}

	const FKey& GetKey() const 
	{ 
		return Key; 
	}

	int32 GetVirtualKeyCode() const 
	{ 
		return VirtualKeyCode; 
	}

private:
	FKey Key;
	int32 VirtualKeyCode;
};

class FMouseButtonEvent
{
public:
	FMouseButtonEvent(const FKey& InKey, void* InNativeWindowHandle)
		: Key(InKey)
		, WindowHandle(InNativeWindowHandle)
	{
	}

	const FKey& GetKey() const
	{
		return Key;
	}

	const void* GetNativeWindowHandle() const
	{
		return WindowHandle;
	}

private:
	FKey Key;
	void* WindowHandle;
};

class FMouseMoveEvent
{
public:
	FMouseMoveEvent(const FVector2D& InMousePosition)
		: MousePosition(InMousePosition)
	{
	}

	const FVector2D& GetMousePosition() const 
	{ 
		return MousePosition;
	}

private:
	FVector2D MousePosition;
};

class FMouseScrollEvent
{
public:
	FMouseScrollEvent(float InScrollDelta)
		: ScrollDelta(InScrollDelta)
	{
	}

	float GetScrollDelta() const
	{ 
		return ScrollDelta;
	}

private:
	float ScrollDelta;
};

class FWindowResizeEvent
{
public:
	FWindowResizeEvent(int32 InWidth, int32 InHeight)
		: Width(InWidth)
		, Height(InHeight)
	{
	}

	int32 GetWidth() const 
	{
		return Width; 
	}
	int32 GetHeight() const 
	{ 
		return Height; 
	}

private:
	int32 Width;
	int32 Height;
};
