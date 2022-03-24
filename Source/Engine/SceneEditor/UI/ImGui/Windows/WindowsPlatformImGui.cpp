#include "WindowsPlatformImGui.h"

// System include for GetKeyboardState, ToAscii, and Windows message types.
#include "Windows.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_win32.h"

// ImGui callback function for processing Windows messages.
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool FWindowsPlatformImGui::Init(void* InNativeWindowHandle)
{
	return ImGui_ImplWin32_Init(InNativeWindowHandle);
}

void FWindowsPlatformImGui::Shutdown()
{
	ImGui_ImplWin32_Shutdown();
}

void FWindowsPlatformImGui::NewFrame()
{
	ImGui_ImplWin32_NewFrame();
}

void FWindowsPlatformImGui::OnKeyDown(const FKeyEvent& InKeyEvent)
{
	HWND DummyWindowHandle = nullptr;
	UINT Message = WM_SYSKEYDOWN;
	WPARAM VirtualKeyCode = InKeyEvent.GetVirtualKeyCode();
	LPARAM DummyScanCode = -1;
	ImGui_ImplWin32_WndProcHandler(DummyWindowHandle, Message, VirtualKeyCode, DummyScanCode);

	// Translate virtual key code to an ASCII character if possible.
	BYTE Characters[2];
	BYTE KeyboardState[256];
	GetKeyboardState(KeyboardState);
	int32 Result = ToAscii(
		// Virtual key code to translate from.
		VirtualKeyCode,
		// Scan code.
		MapVirtualKey(VirtualKeyCode, MAPVK_VK_TO_VSC),
		// Status of the 256 virtual keys on the keyboard.
		KeyboardState, 
		// Output character buffer.
		(WORD*)Characters, 
		// Flags (unused).
		0
	);

	// Notify ImGui if translation to an ASCII character was successful.
	if (Result == 1)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddInputCharacter(Characters[0]);
	}
}

void FWindowsPlatformImGui::OnKeyUp(const FKeyEvent& InKeyEvent)
{
	HWND DummyWindowHandle = nullptr;
	UINT Message = WM_SYSKEYUP;
	WPARAM VirtualKeyCode = InKeyEvent.GetVirtualKeyCode();
	LPARAM DummyScanCode = -1;
	ImGui_ImplWin32_WndProcHandler(DummyWindowHandle, Message, VirtualKeyCode, DummyScanCode);
}

void FWindowsPlatformImGui::OnMouseDown(const FMouseButtonEvent& InMouseButtonEvent)
{
	// Translate EKey to Windows message type.
	UINT Message = 0;
	const FKey& MouseButton = InMouseButtonEvent.GetKey();
	if (MouseButton == EKeys::LeftMouseButton)
	{
		Message = WM_LBUTTONDOWN;
	}
	else if (MouseButton == EKeys::RightMouseButton)
	{
		Message = WM_RBUTTONDOWN;
	}
	else if (MouseButton == EKeys::MiddleMouseButton)
	{
		Message = WM_MBUTTONUP;
	}

	HWND WindowHandle = (HWND)InMouseButtonEvent.GetNativeWindowHandle();
	WPARAM DummyVirtualKeyCode = -1;
	LPARAM DummyScanCode = -1;
	ImGui_ImplWin32_WndProcHandler(WindowHandle, Message, DummyVirtualKeyCode, DummyScanCode);
}

void FWindowsPlatformImGui::OnMouseUp(const FMouseButtonEvent& InMouseButtonEvent)
{
	// Translate EKey to Windows message type.
	UINT Message = 0;
	const FKey& MouseButton = InMouseButtonEvent.GetKey();
	if (MouseButton == EKeys::LeftMouseButton)
	{
		Message = WM_LBUTTONUP;
	}
	else if (MouseButton == EKeys::RightMouseButton)
	{
		Message = WM_RBUTTONUP;
	}
	else if (MouseButton == EKeys::MiddleMouseButton)
	{
		Message = WM_MBUTTONUP;
	}

	HWND WindowHandle = (HWND)InMouseButtonEvent.GetNativeWindowHandle();
	WPARAM DummyVirtualKeyCode = -1;
	LPARAM DummyScanCode = -1;
	ImGui_ImplWin32_WndProcHandler(WindowHandle, Message, DummyVirtualKeyCode, DummyScanCode);
}
