#pragma once

#include "GenericPlatform/GenericPlatformTime.h"

class FWindowsPlatformTime : public FGenericPlatformTime
{
public:
	// Begin FGenericPlatformTime interface.
	static bool Init();
	static double CurrentTimeMicroseconds();
	static double CurrentTimeMilliseconds();
	// End FGenericPlatformTime interface.
};

using FPlatformTime = FWindowsPlatformTime;
