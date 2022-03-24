#pragma once

#include "GenericWindow.h"
#include "GenericApplicationMessageHandler.h"

class FGenericApplication
{
public:
	FGenericApplication();
	virtual ~FGenericApplication() = default;

	// Non-copyable.
	FGenericApplication(const FGenericApplication&) = delete;
	FGenericApplication& operator=(const FGenericApplication&) = delete;

	// Non-movable.
	FGenericApplication(FGenericApplication&&) = delete;
	FGenericApplication& operator=(FGenericApplication&&) = delete;

	virtual TUniquePtr<FGenericWindow> MakeWindow(const ANSICHAR* InWindowTitle, int32 InWidth, int32 InHeight);
	void SetApplicationMessageHandler(const TSharedPtr<FGenericApplicationMessageHandler>& InApplicationMessageHandler);
	bool IsRunning() const;

protected:
	// Handler used to process messages received from the OS.
	TSharedPtr<FGenericApplicationMessageHandler> MessageHandler;
	bool bIsRunning;
};
