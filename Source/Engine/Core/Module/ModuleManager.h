#pragma once

#include "CoreGlobals.h"
#include "SmartPointers/UniquePtr.h"
#include "SmartPointers/SharedPtr.h"

#include "Strings/StringId.h"
#include "Containers/Map.h"

class IModule;

/**
 * Singleton class that handles adding, loading, and unloading of all engine modules.
 */
class FModuleManager
{
public:
	~FModuleManager() = default;
	static FModuleManager& Get();
	
	/**
	 * Adds a module to our module map if it hasn't been added already.
	 * 
	 * @param InModuleName: Name of the module to add.
	 */
	void AddModule(const FStringId& InModuleName);

	/**
	 * Loads a specified module, which must be contained module map.
	 *
	 * @param InModuleName: Name of the module to load.
	 * @returns: A pointer to the module if it's contained in the module map, nullptr otherwise.
	 */
	IModule* LoadModule(const FStringId& InModuleName);

	/**
	 * Unloads a specified module, which must be contained module map.
	 *
	 * @param InModuleName: Name of the module to unload.
	 * @returns: true if the module was successfully unloaded, false otherwise.
	 */
	bool UnloadModule(const FStringId& InModuleName);

	/**
	 * Checks if a specified module is loaded.
	 *
	 * @param InModuleName: Name of the target module.
	 * @returns: true if the module is loaded, false otherwise.
	 */
	bool IsModuleLoaded(const FStringId& InModuleName) const;
	
	/**
	 * Gets a module from the module map.
	 *
	 * @param InModuleName: Name of the module to get.
	 * @returns: A pointer to the module if found, nullptr otherwise.
	 */
	template<typename ModuleType>
	ModuleType* GetModule(const FStringId& InModuleName) const;

private:
	FModuleManager() = default;
	
	// Info pertaining to a module in Vice Engine.
	struct FModuleInfo
	{
		FModuleInfo()
			: bReadyToUse(false)
			, ModuleLoadOrder(++LoadOrderCounter)
			, DllHandle(nullptr)
		{
		}

		bool bReadyToUse;
		// A unique Id for the module.
		int32 ModuleLoadOrder;
		void* DllHandle;
		// The owned module.
		TUniquePtr<IModule> Module;
		// Global counter of module Ids. Used to assign an Id to each loaded module.
		// Incremented everytime a new module info is created.
		static int32 LoadOrderCounter;
	};
	
	TMap<FStringId, TSharedPtr<FModuleManager::FModuleInfo>> ModuleInfoMap;
};

template<typename ModuleType>
ModuleType* FModuleManager::GetModule(const FStringId& InModuleName) const
{
	if (!ModuleInfoMap.IsKeyContained(InModuleName))
	{
		return nullptr;
	}

	const TSharedPtr<FModuleInfo>& ModuleInfo = *ModuleInfoMap.Find(InModuleName);
	return (ModuleType*)ModuleInfo->Module.Get();
}
