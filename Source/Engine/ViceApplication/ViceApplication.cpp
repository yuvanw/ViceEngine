#include "ViceApplication.h"
#include "HAL/PlatformInput.h"
#include "Events.h"

static FKey GetKeyFromMouseButton(EMouseButton InButton);

FViceApplication::FViceApplication(TUniquePtr<FGame>&& InGame)
	: Game(MoveTemp(InGame))
{
}

void FViceApplication::Init()
{
	Game->Init();
}

void FViceApplication::Shutdown()
{
	Game->Shutdown();
}

void FViceApplication::Update(float InDeltaTimeMilliseconds)
{
	Game->Update(InDeltaTimeMilliseconds);
}

/**
 * Message handling callbacks translate OS messages to their corresponding input event
 * and then pass them to the game instance.
 */
void FViceApplication::OnKeyDown(const FKeyMessage& InKeyMessage)
{
	int32 VirtualKeyCode = InKeyMessage.VirtualKeyCode;
	FKey Key = FPlatformInput::GetKeyFromVirtualKeyCode(VirtualKeyCode);
	FKeyEvent KeyEvent = FKeyEvent(Key, VirtualKeyCode);
	Game->OnKeyDown(KeyEvent);
}

void FViceApplication::OnKeyUp(const FKeyMessage& InKeyMessage)
{
	int32 VirtualKeyCode = InKeyMessage.VirtualKeyCode;
	FKey Key = FPlatformInput::GetKeyFromVirtualKeyCode(VirtualKeyCode);
	FKeyEvent KeyEvent = FKeyEvent(Key, VirtualKeyCode);
	Game->OnKeyUp(KeyEvent);
}

void FViceApplication::OnMouseDown(const FMouseButtonMessage& InMouseButtonMessage)
{
	FKey Key = GetKeyFromMouseButton(InMouseButtonMessage.Button);
	FMouseButtonEvent MouseButtonEvent = FMouseButtonEvent(Key, InMouseButtonMessage.WindowHandle);
	Game->OnMouseDown(MouseButtonEvent);
}

void FViceApplication::OnMouseUp(const FMouseButtonMessage& InMouseButtonMessage)
{
	FKey Key = GetKeyFromMouseButton(InMouseButtonMessage.Button);
	FMouseButtonEvent MouseButtonEvent = FMouseButtonEvent(Key, InMouseButtonMessage.WindowHandle);
	Game->OnMouseUp(MouseButtonEvent);
}

void FViceApplication::OnMouseMove(const FMouseMoveMessage& InMouseMoveMessage)
{
	FMouseMoveEvent MouseMoveEvent = FMouseMoveEvent(InMouseMoveMessage.MousePosition);
	Game->OnMouseMove(MouseMoveEvent);
}

void FViceApplication::OnMouseScroll(const FMouseScrollMessage& InMouseScrollMessage)
{
	FMouseScrollEvent MouseScrollEvent = FMouseScrollEvent(InMouseScrollMessage.ScrollDelta);
	Game->OnMouseScroll(MouseScrollEvent);
}

void FViceApplication::OnWindowResize(const FWindowResizeMessage& InWindowResizeMessage)
{
	FWindowResizeEvent MouseScrollEvent = FWindowResizeEvent(InWindowResizeMessage.Width, InWindowResizeMessage.Height);
	Game->OnWindowResize(MouseScrollEvent);
}

static FKey GetKeyFromMouseButton(EMouseButton InButton)
{
	switch (InButton)
	{
		case EMouseButton::Left:   return EKeys::LeftMouseButton;
		case EMouseButton::Right:  return EKeys::RightMouseButton;
		case EMouseButton::Middle: return EKeys::MiddleMouseButton;
		default:                   return EKeys::None;
	}
}
