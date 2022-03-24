#pragma once

#include "Events.h"

/**
 * Interface that defines game initialization/shutdown behavior and the core game loop.
 * Event handling callbacks are used to specify how to handle player interaction.
 */
class FGame
{
public:
	FGame() = default;
	virtual ~FGame() = default;

	// Non-copyable.
	FGame(const FGame&) = default;
	FGame& operator=(const FGame&) = default;

	// Non-movable.
	FGame(FGame&&) = default;
	FGame& operator=(FGame&&) = default;

	// Empty default implementations.
	virtual void Init() {};
	virtual void Shutdown() {};
	virtual void Update(float InDeltaTimeMilliseconds) {};

	// Event handling callbacks.
	virtual void OnKeyDown(const FKeyEvent& InKeyEvent) {};
	virtual void OnKeyUp(const FKeyEvent& InKeyEvent) {};
	virtual void OnMouseDown(const FMouseButtonEvent& InMouseButtonEvent) {};
	virtual void OnMouseUp(const FMouseButtonEvent& InMouseButtonEvent) {};
	virtual void OnMouseMove(const FMouseMoveEvent& InMouseMoveEvent) {};
	virtual void OnMouseScroll(const FMouseScrollEvent& InMouseScrollEvent) {};
	virtual void OnWindowResize(const FWindowResizeEvent& InWindowResizeEvent) {};
};
