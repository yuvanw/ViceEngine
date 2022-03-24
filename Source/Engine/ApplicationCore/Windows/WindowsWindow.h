#pragma once

#include "Strings/StringId.h"
#include "GenericPlatform/GenericWindow.h"

#include <Windows.h>

class FWindowsWindow : public FGenericWindow
{
public:
	explicit FWindowsWindow(const ANSICHAR* InWindowTitle, int32 InWidth, int32 InHeight, HINSTANCE InInstanceHandle);
	~FWindowsWindow();

	// Non-copyable.
	FWindowsWindow(const FWindowsWindow&) = delete;
	FWindowsWindow& operator=(const FWindowsWindow&) = delete;

	// Non-movable.
	FWindowsWindow(const FWindowsWindow&&) = delete;
	FWindowsWindow& operator=(const FWindowsWindow&&) = delete;

	// Begin FGenericWindow interface.
	virtual void Show() override;
	virtual void Hide() override;
	virtual void Minimize() override;
	virtual void Maximize() override;
	virtual const void* GetNativeWindowHandle() const override;
	virtual void* GetNativeWindowHandle() override;
	// End FGenericWindow interface.

	// Name of the window class to use create a window.
	// For more details, see: WindowsApplication.cpp.
	static const FStringId& GetWindowClassName();

private:
	HWND WindowHandle;
};
