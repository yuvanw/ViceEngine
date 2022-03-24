#include "WindowsPlatformTime.h"

#include "Windows.h"

double FGenericPlatformTime::MicrosecondsPerCycle = 0.0;

bool FWindowsPlatformTime::Init()
{
	LARGE_INTEGER Frequency;
	bool IsHighResPerformanceCounterSupported = QueryPerformanceFrequency(&Frequency);
	if (!IsHighResPerformanceCounterSupported)
	{
		return false;
	}

	MicrosecondsPerCycle = 1000000.0 / Frequency.QuadPart;
	return true;
};

double FWindowsPlatformTime::CurrentTimeMicroseconds() 
{
	LARGE_INTEGER Cycles;
	QueryPerformanceCounter(&Cycles);
	return Cycles.QuadPart * MicrosecondsPerCycle;
};

double FWindowsPlatformTime::CurrentTimeMilliseconds()
{
	return CurrentTimeMicroseconds() * 0.001;
};
