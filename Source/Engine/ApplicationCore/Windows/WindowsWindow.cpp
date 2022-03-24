#include "CoreMinimal.h"
#include "WindowsWindow.h"

FWindowsWindow::FWindowsWindow(const ANSICHAR* InWindowTitle, int32 InWidth, int32 InHeight, HINSTANCE InInstanceHandle)
	: FGenericWindow(InWindowTitle, InWidth, InHeight)
{
	ensure(InWidth >= 0 && InHeight >= 0);
		
	HWND hwnd = CreateWindowEx(
		// Optional window styles.
		0,
		// Window class name.
		GetWindowClassName().GetString().GetData(),
		// Window title.
		InWindowTitle,
		// Window style.
		WS_OVERLAPPEDWINDOW,
		// Size and position.
		0, 0, 
		InWidth, InHeight,
		// Parent window.
		nullptr,
		// Menu.
		nullptr,
		// Instance handle.
		InInstanceHandle,
		// Additional application data.
		nullptr
	);

	ensure(hwnd);
	WindowHandle = hwnd;
}

/* static */ const FStringId& FWindowsWindow::GetWindowClassName()
{
	static FStringId WindowClassName("ViceWindow");
	return WindowClassName;
}

FWindowsWindow::~FWindowsWindow()
{
	DestroyWindow(WindowHandle);
}

void FWindowsWindow::Show()
{
	ShowWindow(WindowHandle, SW_SHOW);
}

void FWindowsWindow::Hide()
{
	ShowWindow(WindowHandle, SW_HIDE);
}

void FWindowsWindow::Minimize()
{
	ShowWindow(WindowHandle, SW_SHOWMINIMIZED);
}

void FWindowsWindow::Maximize()
{
	ShowWindow(WindowHandle, SW_SHOWMAXIMIZED);
}

const void* FWindowsWindow::GetNativeWindowHandle() const
{
	return WindowHandle;
}

void* FWindowsWindow::GetNativeWindowHandle()
{
	return WindowHandle;
}
