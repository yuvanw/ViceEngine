#include "CoreMinimal.h"
#include "WindowsApplication.h"
#include "WindowsWindow.h"

// System include for GET_X_LPARAM and GET_Y_LPARAM macros.
#include <WindowsX.h>

// Sole instance of the Windows application.
static FWindowsApplication* WindowsApplication = nullptr;

static FKeyMessage CreateKeyMessage(WPARAM wParam);

FWindowsApplication::FWindowsApplication(HINSTANCE InInstanceHandle)
	: FGenericApplication()
	, InstanceHandle(InInstanceHandle)
{
	/**
	 * Register the window class used to create windows for our application.
	 * A window class contains settings the Win32 API uses as a template
	 * for creating a window. For more details on window classes, see:
	 * https://docs.microsoft.com/en-us/windows/win32/winmsg/about-window-classes
	 */
	WNDCLASS wc = { 0 };
	// Callback used to handle incoming messages.
	wc.lpfnWndProc = WindowProc;
	// Instance handle and window class name used together to identify the window class uniquely.
	// For details on why both are needed, see: https://devblogs.microsoft.com/oldnewthing/20050418-59/?p=35873
	wc.hInstance = InInstanceHandle;
	wc.lpszClassName = FWindowsWindow::GetWindowClassName().GetString().GetData();
	RegisterClass(&wc);

	WindowsApplication = this;
}

TUniquePtr<FGenericWindow> FWindowsApplication::MakeWindow(const ANSICHAR* InWindowTitle, int32 InWidth, int32 InHeight)
{
	return MakeUnique<FWindowsWindow>(InWindowTitle, InWidth, InHeight, InstanceHandle);
}

LRESULT CALLBACK FWindowsApplication::WindowProc(HWND hwnd, uint32 msg, WPARAM wParam, LPARAM lParam)
{
	// Any messages we don't handle get processed by the default window procedure.
	if (!WindowsApplication->ProcessMessage(hwnd, msg, wParam, lParam))
	{
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

bool FWindowsApplication::ProcessMessage(HWND hwnd, uint32 msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_KEYDOWN:
		{
			FKeyMessage KeyMessage = CreateKeyMessage(wParam);
			MessageHandler->OnKeyDown(KeyMessage);
			break;
		}
		case WM_KEYUP:
		{
			FKeyMessage KeyMessage = CreateKeyMessage(wParam);
			MessageHandler->OnKeyUp(KeyMessage);
			break;
		}
		case WM_LBUTTONDOWN:
		{
			FMouseButtonMessage MouseButtonMessage = FMouseButtonMessage(EMouseButton::Left, hwnd);
			MessageHandler->OnMouseDown(MouseButtonMessage);
			break;
		}
		case WM_RBUTTONDOWN:
		{
			FMouseButtonMessage MouseButtonMessage = FMouseButtonMessage(EMouseButton::Right, hwnd);
			MessageHandler->OnMouseDown(MouseButtonMessage);
			break;
		}
		case WM_MBUTTONDOWN:
		{
			FMouseButtonMessage MouseButtonMessage = FMouseButtonMessage(EMouseButton::Middle, hwnd);
			MessageHandler->OnMouseDown(MouseButtonMessage);
			break;
		}
		case WM_LBUTTONUP:
		{
			FMouseButtonMessage MouseButtonMessage = FMouseButtonMessage(EMouseButton::Left, hwnd);
			MessageHandler->OnMouseUp(MouseButtonMessage);
			break;
		}
		case WM_RBUTTONUP:
		{
			FMouseButtonMessage MouseButtonMessage = FMouseButtonMessage(EMouseButton::Right, hwnd);
			MessageHandler->OnMouseUp(MouseButtonMessage);
			break;
		}
		case WM_MBUTTONUP:
		{
			FMouseButtonMessage MouseButtonMessage = FMouseButtonMessage(EMouseButton::Middle, hwnd);
			MessageHandler->OnMouseUp(MouseButtonMessage);
			break;
		}
		case WM_MOUSEMOVE:
		{
			float MousePositionX = static_cast<float>(GET_X_LPARAM(lParam));
			float MousePositionY = static_cast<float>(GET_Y_LPARAM(lParam));
			FMouseMoveMessage MouseMoveMessage = FMouseMoveMessage({ MousePositionX, MousePositionY });
			MessageHandler->OnMouseMove(MouseMoveMessage);
			break;
		}
		case WM_MOUSEWHEEL:
		{
			float ScrollDelta = static_cast<float>(GET_WHEEL_DELTA_WPARAM(wParam));
			FMouseScrollMessage MouseScrollMessage = FMouseScrollMessage(ScrollDelta);
			MessageHandler->OnMouseScroll(MouseScrollMessage);
			break;
		}
		case WM_SIZE:
		{
			int32 ResizedWidth = LOWORD(lParam);
			int32 ResizedHeight = HIWORD(lParam);
			FWindowResizeMessage WindowResizeMessage = FWindowResizeMessage(ResizedWidth, ResizedHeight);
			MessageHandler->OnWindowResize(WindowResizeMessage);
			break;
		}
		case WM_CLOSE:
		{
			bIsRunning = false;
			::PostQuitMessage(0);
			break;
		}
		default:
		{
			// We don't handle the type of message received.
			return false;
		}
	}

	// Message handled successfully.
	return true;
}

static FKeyMessage CreateKeyMessage(WPARAM wParam)
{
	int32 VirtualKeyCode = wParam;
	return FKeyMessage(VirtualKeyCode);
}
