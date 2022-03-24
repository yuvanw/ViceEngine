#pragma once

#include "Messages.h"

// Interface that specifies how to handle OS messages.
class FGenericApplicationMessageHandler
{
public:
	FGenericApplicationMessageHandler() = default;
	virtual ~FGenericApplicationMessageHandler() = default;

	// Empty default implementations.
	virtual void Init() {};
	virtual void Shutdown() {};
	virtual void Update(float InDeltaTimeMilliseconds) {};

	// Message handling callbacks.
	virtual void OnKeyDown(const FKeyMessage& InKeyMessage) {}
	virtual void OnKeyUp(const FKeyMessage& InKeyMessage) {}
	virtual void OnMouseDown(const FMouseButtonMessage& InMouseButtonMessage) {}
	virtual void OnMouseUp(const FMouseButtonMessage& InMouseButtonMessage) {}
	virtual void OnMouseMove(const FMouseMoveMessage& InMouseMoveMessage) {}
	virtual void OnMouseScroll(const FMouseScrollMessage& InMouseScrollMessage) {};
	virtual void OnWindowResize(const FWindowResizeMessage& InWindowResizeMessage) {};
};
