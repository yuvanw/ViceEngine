#include "WindowsPlatformFileSystem.h"

// System include for FindFirstFile, FindNextFile, and FindClose.
#include "Windows.h"
// System include for PathFileExists.
#include "shlwapi.h"

#include <string>

bool FWindowsPlatformFileSystem::IsValidPath(const ANSICHAR* InPath)
{
	return PathFileExistsA(InPath);
}

/**
* Implementation referenced from MSDN:
* https://docs.microsoft.com/en-us/windows/win32/fileio/listing-the-files-in-a-directory
*/
TArray<FStringId> FWindowsPlatformFileSystem::GetAllFileNamesWithinDirectory(const ANSICHAR* InDirectoryPath)
{
	TArray<FStringId> FileNames;

	// Search for all files under the input directory, no matter the file name or extension.
	std::string SearchPattern = std::string(InDirectoryPath) + "/*.*";
	
	WIN32_FIND_DATA FindData;
	HANDLE FileHandle = FindFirstFile(SearchPattern.c_str(), &FindData);
	if (FileHandle != INVALID_HANDLE_VALUE) 
	{
		do
		{
			if (!(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				std::string FileName = FindData.cFileName;
				FileNames.Add(FileName.c_str());
			}
		} while (FindNextFile(FileHandle, &FindData) != 0);

		FindClose(FileHandle);
	}

	return FileNames;
}
