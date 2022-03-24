#pragma once

#include "CoreGlobals.h"
#include "Containers/Array.h"
#include "Strings/StringId.h"

class FGenericPlatformFileSystem
{
public:
	/**
	 * Checks whether a specified file or directory path exists.
	 *
	 * @param InPath: Absolute path to the file or directory, with forward slash (/) as the directory separator character.
	 * @returns true if the specified file or directory path exists, false otherwise.
	 */
	static bool IsValidPath(const ANSICHAR* InPath)
	{
		return false;
	}

	/**
	 * Finds the names of all files located within a specified directory.
	 * 
	 * @param InDirectoryPath: Absolute path to the directory, with forward slash (/) as the directory separator character.
	 * @returns The names of all files located under the input directory if it exists, and an empty TArray otherwise.
	 */
	static TArray<FStringId> GetAllFileNamesWithinDirectory(const ANSICHAR* InDirectoryPath)
	{
		return TArray<FStringId>();
	}
};
