#pragma once

#include "GenericPlatform/GenericPlatformApplication.h"	

class FWindowsPlatformApplication : public FGenericPlatformApplication
{
public:
	static TUniquePtr<FGenericApplication> CreateApplication();

	// Retrieves and dispatches all messages in the current thread's message queue.
	static void PumpMessages();
};

using FPlatformApplication = FWindowsPlatformApplication;
