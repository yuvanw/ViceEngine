#pragma once

#include "CoreMinimal.h"
#include "Camera/Camera.h"
#include "Events.h"

/**
 * Mouse look camera controller.
 * 
 * Controls:
 * - Rotate with left mouse button.
 * - Pan with middle or right mouse button.
 * - Zoom with scroll wheel.
 */
class FCameraController
{
public:
	explicit FCameraController(const TSharedPtr<FCamera>& InCamera);
	~FCameraController() = default;

	// Non-copyable.
	FCameraController(const FCameraController&) = delete;
	FCameraController& operator=(const FCameraController&) = delete;

	// Non-movable.
	FCameraController(FCameraController&&) = delete;
	FCameraController& operator=(FCameraController&&) = delete;

	void Update(float InDeltaTimeMilliseconds);
	void OnMouseDown(const FMouseButtonEvent& InMouseButtonEvent);
	void OnMouseUp(const FMouseButtonEvent& InMouseButtonEvent);
	void OnMouseMove(const FMouseMoveEvent& InMouseMoveEvent);
	void OnMouseScroll(const FMouseScrollEvent& InMouseScrollEvent);

private:
	void Rotate();
	void Translate(float InDeltaTimeMilliseconds);
	void Zoom(float InDeltaTimeMilliseconds);

	// Camera being controlled.
	TSharedPtr<FCamera> Camera;
	// Mouse button currently pressed (if any).
	FKey PressedButton;
	// Mouse position of the previous frame.
	FVector2D PreviousMousePosition;
	// Mouse position of the current frame.
	FVector2D CurrentMousePosition;
	// Amount scroll wheel shifted from previous frame.
	// Positive if scrolled forward, negative if scrolled backward.
	float ScrollDelta;
};
