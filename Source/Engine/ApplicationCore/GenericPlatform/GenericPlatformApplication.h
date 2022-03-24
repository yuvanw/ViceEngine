#pragma once

#include "GenericApplication.h"

class FGenericPlatformApplication
{
public:
	// Default implementations.
	static TUniquePtr<FGenericApplication> CreateApplication() 
	{ 
		return nullptr; 
	}
	
	static void PumpMessages() 
	{
	}
};
