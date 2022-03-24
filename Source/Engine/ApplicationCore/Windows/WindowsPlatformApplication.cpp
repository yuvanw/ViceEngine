#include "WindowsPlatformApplication.h"
#include "WindowsApplication.h"

#include <Windows.h>

TUniquePtr<FGenericApplication> FWindowsPlatformApplication::CreateApplication()
{
	// Get instance handle of the executable.
	HINSTANCE InstanceHandle = GetModuleHandle(NULL);
	return MakeUnique<FWindowsApplication>(InstanceHandle);
}

void FWindowsPlatformApplication::PumpMessages()
{
	MSG Message;
	while (PeekMessage(
		// Struct to store message info in.
		&Message,
		// Handle of window whose messages we want to receive. Set to nullptr
		// here so we retrieve messages from all windows owned by the current thread.
		nullptr, 
		// Message filtering options (unused).
		0, 0, 
		// Remove message from queue after calling PeekMessage.
		PM_REMOVE))
	{
		// Each message has a window it's associated with, and each window has a message handling
		// callback. We call DispatchMessage to process each message using said callback 
		// (for more details, see WindowsApplication.cpp).
		DispatchMessage(&Message);
	}
}
