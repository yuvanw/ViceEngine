#pragma once

#include "CoreGlobals.h"

class FGenericPlatform
{
public:
	/**
	 * Gets the handle of a specified DLL.
	 * 
	 * @param InFileName: Name of the DLL.
	 * @returns: The DLL handle if found, nullptr otherwise.
	 */
    static void* GetDllHandle(const ANSICHAR* InFileName)
    { 
        return nullptr; 
    }
    
	/**
	 * Frees a specified DLL handle.
	 *
	 * @param InDllHandle: The DLL handle to free.
	 */
	static void FreeDllHandle(void* InDllHandle)
    {
    }
    
	/**
	 * Gets an exported function from a DLL.
	 *
	 * @param InDllHandle: The handle of the DLL to get the exported function from.
	 * @param InProcName: The name of the exported function.
	 * @returns: A pointer to the exported function if found, nullptr otherwise.
	 */
	static void* GetDllExport(void* InDllHandle, const ANSICHAR* InProcName)
    {
        return nullptr;
    }
};
