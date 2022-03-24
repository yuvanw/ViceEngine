#include "WindowsPlatform.h"
#include "AssertionMacros.h"

// System include for LoadLibary, FreeLibary, and GetProcAddress.
#include "Windows.h"

#include <iostream>

void* FWindowsPlatform::GetDllHandle(const char* InFileName)
{
	ensure(InFileName);
	void* Handle = LoadLibrary(InFileName);
	return Handle;
}

void FWindowsPlatform::FreeDllHandle(void* InDllHandle)
{
	ensure(InDllHandle);
	FreeLibrary(reinterpret_cast<HMODULE>(InDllHandle));
}

void* FWindowsPlatform::GetDllExport(void* InDllHandle, const char* InProcName)
{
	ensure(InDllHandle);
	ensure(InProcName);
	return GetProcAddress(reinterpret_cast<HMODULE>(InDllHandle), InProcName);
}
