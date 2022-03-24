#pragma once

#include "CoreGlobals.h"
#include "GenericPlatform/GenericPlatform.h"

class FMacPlatform : public FGenericPlatform
{
public:
	static void* GetDllHandle(const ANSICHAR* InFileName);
	static void FreeDllHandle(void* InDllHandle);
	static void* GetDllExport(void* InDllHandle, const ANSICHAR* InProcName);
};

using FPlatform = FMacPlatform;
