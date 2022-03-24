#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericApplication.h"

#include <Windows.h>

class FWindowsApplication : public FGenericApplication
{
public:
	explicit FWindowsApplication(HINSTANCE InInstanceHandle);
	~FWindowsApplication() = default;

	// Non-copyable.
	FWindowsApplication(const FWindowsApplication&) = delete;
	FWindowsApplication& operator=(const FWindowsApplication&) = delete;

	// Non-movable.
	FWindowsApplication(FWindowsApplication&&) = delete;
	FWindowsApplication& operator=(FWindowsApplication&&) = delete;

	// Begin FGenericApplication interface.
	virtual TUniquePtr<FGenericWindow> MakeWindow(const ANSICHAR* InWindowTitle, int32 InWidth, int32 InHeight) override;
	// End FGenericApplication interface

private:
	// Message handling callback that Windows dispatches messages to.
	static LRESULT CALLBACK WindowProc(HWND hwnd, uint32 msg, WPARAM wParam, LPARAM lParam);
	bool ProcessMessage(HWND hwnd, uint32 msg, WPARAM wParam, LPARAM lParam);

	// Handle that identifies the module (DLL) that created this application.
	HINSTANCE InstanceHandle;
};
