#pragma once

#include "CoreMinimal.h"

class FGenericWindow
{
public:
	explicit FGenericWindow(const ANSICHAR* InWindowTitle, int32 InWidth, int32 InHeight) 
		: WindowTitle(InWindowTitle)
		, Width(InWidth)
		, Height(InHeight)
	{
	};
	virtual ~FGenericWindow() = default;
	
	// Non-copyable.
	FGenericWindow(const FGenericWindow&) = delete;
	FGenericWindow& operator=(const FGenericWindow&) = delete;
	
	// Non-movable.
	FGenericWindow(FGenericWindow&&) = default;
	FGenericWindow& operator=(FGenericWindow&&) = default;

	// Default empty implementations.
	virtual void Show() {}
	virtual void Hide() {}
	virtual void Minimize() {}
	virtual void Maximize() {}
	
	// Boolean functions return false by default.
	virtual bool IsShown() const
	{
		return false;
	}
	virtual bool IsHidden() const
	{
		return false;
	}
	virtual bool IsMinimized() const
	{
		return false;
	}
	virtual bool IsMaximized() const
	{
		return false;
	}

	// Returns the OS-specific window handle associated with the window (e.g. Win32 HWND).
	virtual const void* GetNativeWindowHandle() const
	{
		return nullptr;
	}	
	virtual void* GetNativeWindowHandle()
	{
		return nullptr;
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
	// Text that is shown in the title bar of the window.
	FANSIString WindowTitle;
	int32 Width;
	int32 Height;
};
