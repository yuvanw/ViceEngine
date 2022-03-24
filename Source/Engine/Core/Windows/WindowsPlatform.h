#pragma once

#include "CoreGlobals.h"
#include "GenericPlatform/GenericPlatform.h"

#define DLLEXPORT __declspec(dllexport)
#define DLLIMPORT __declspec(dllimport)

class FWindowsPlatform : public FGenericPlatform
{
public:
	static void* GetDllHandle(const char* InFileName);
	static void FreeDllHandle(void* InDllHandle);
	static void* GetDllExport(void* InDllHandle, const char* InProcName);
};

using FPlatform = FWindowsPlatform;
