#include "Mac/MacPlatform.h"
#include "AssertionMacros.h"

// @TODOLog: Replace this once logging it is implemented.
#include <iostream>
// Mac platform includes.
#include <dlfcn.h>

void* FMacPlatform::GetDllHandle(const char* InFileName)
{
	if (!InFileName)
	{
		std::cout << "Null filename passed in. Invalid." << std::endl;
		return nullptr;
	}

	// According to Apple Docs, RTLD_NOLOAD will not load the dylib, if it is not already loaded.
	// It will return the handle of the dylib if it is already open. 
	// This gives us a way to check if the dylib is already open.
	void* DylibHandle = dlopen(InFileName, RTLD_NOLOAD | RTLD_LAZY | RTLD_LOCAL);
	if (!DylibHandle)
	{
		// Not loaded yet, we'll actually load it.
		DylibHandle = dlopen(InFileName, RTLD_LAZY | RTLD_LOCAL);
	}
	ensure(DylibHandle);
	return DylibHandle;
}

void FMacPlatform::FreeDllHandle(void* InDllHandle)
{
	ensure(InDllHandle);
	dlclose(InDllHandle);
}

void* FMacPlatform::GetDllExport(void* InDllHandle, const char* InProcName)
{
	ensure(InDllHandle);
	ensure(InProcName);
	return dlsym( InDllHandle, InProcName);
}
