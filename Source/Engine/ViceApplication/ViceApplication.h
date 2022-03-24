#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericApplicationMessageHandler.h"
#include "Game.h"

/**
 * Intermediary layer between ApplicationCore and GameCore used to translate OS messages
 * into input events and pass them to the currently running game. Used as the message handler
 * for the application message pump.
 */
class FViceApplication : public FGenericApplicationMessageHandler
{
public:
	explicit FViceApplication(TUniquePtr<FGame>&& InGame);
	~FViceApplication() = default;

	// Begin FGenericApplicationMessageHandler interface.
	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void Update(float InDeltaTimeMilliseconds) override;

	virtual void OnKeyDown(const FKeyMessage& InKeyMessage) override;
	virtual void OnKeyUp(const FKeyMessage& InKeyMessage) override;
	virtual void OnMouseDown(const FMouseButtonMessage& InMouseButtonMessage) override;
	virtual void OnMouseUp(const FMouseButtonMessage& InMouseButtonMessage) override;
	virtual void OnMouseMove(const FMouseMoveMessage& InMouseMoveMessage) override;
	virtual void OnMouseScroll(const FMouseScrollMessage& InMouseScrollMessage) override;
	virtual void OnWindowResize(const FWindowResizeMessage& InWindowResizeMessage) override;
	// End FGenericApplicationMessageHandler interface.

private:
	TUniquePtr<FGame> Game;
};
