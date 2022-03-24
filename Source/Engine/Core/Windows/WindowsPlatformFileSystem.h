#pragma once

#include "GenericPlatform/GenericPlatformFileSystem.h"

class FWindowsPlatformFileSystem : public FGenericPlatformFileSystem
{
public:
	// Begin FGenericPlatformFileSystem interface.
	static bool IsValidPath(const ANSICHAR* InPath);
	static TArray<FStringId> GetAllFileNamesWithinDirectory(const ANSICHAR* InDirectoryPath);
	// End FGenericPlatformFileSystem interface.
};

using FPlatformFileSystem = FWindowsPlatformFileSystem;
