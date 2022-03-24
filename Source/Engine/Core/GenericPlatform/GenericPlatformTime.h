#pragma once

class FGenericPlatformTime
{
public:
	/**
	 * @returns true if initialization was successful, false otherwise.
	 */
	static bool Init() 
	{
		return false;
	};

	/**
	 * @returns Epoch time in microseconds.
	 */
	static double CurrentTimeMicroseconds() 
	{ 
		return -1.0;
	};

	/**
	 * @returns Epoch time in milliseconds.
	 */
	static double CurrentTimeMilliseconds()
	{
		return -1.0;
	};

protected:
	static double MicrosecondsPerCycle;
};
