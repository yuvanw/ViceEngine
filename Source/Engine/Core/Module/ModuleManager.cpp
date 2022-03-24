#include "Module/ModuleManager.h"
#include "Module/IModule.h"
#include "HAL/Platform.h"
#include "SmartPointers/SharedPtr.h"
#include "SmartPointers/UniquePtr.h"

int32 FModuleManager::FModuleInfo::LoadOrderCounter = 0;

using InitializeModuleFunctionPtr = IModule* (*)();

FModuleManager& FModuleManager::Get()
{
	static FModuleManager ModuleManager;
	return ModuleManager;
}

void FModuleManager::AddModule(const FStringId& InModuleName)
{
	if (!ModuleInfoMap.IsKeyContained(InModuleName))
	{
		ModuleInfoMap.Add(InModuleName, MakeShared<FModuleManager::FModuleInfo>());
	}
}

IModule* FModuleManager::LoadModule(const FStringId& InModuleName)
{
	if (!ModuleInfoMap.IsKeyContained(InModuleName))
	{
		return nullptr;
	}

	void* DllHandle = FPlatform::GetDllHandle(InModuleName.GetString().GetData());
	InitializeModuleFunctionPtr InitializeModule = (InitializeModuleFunctionPtr)FPlatform::GetDllExport(DllHandle, "InitializeModule");

	TSharedPtr<FModuleInfo>& ModuleInfo = *ModuleInfoMap.Find(InModuleName);
	ModuleInfo->DllHandle = DllHandle;
	ModuleInfo->Module = TUniquePtr<IModule>(InitializeModule());
	ModuleInfo->bReadyToUse = true;

	return ModuleInfo->Module.Get();
}

bool FModuleManager::UnloadModule(const FStringId& InModuleName)
{
	if (!ModuleInfoMap.IsKeyContained(InModuleName))
	{
		TSharedPtr<FModuleInfo>& ModuleInfo = *ModuleInfoMap.Find(InModuleName);
		if (ModuleInfo->Module.IsValid())
		{
			ModuleInfo->bReadyToUse = false;
			ModuleInfo->Module->OnShutdown();
			ModuleInfo->Module.Reset();
			if (ModuleInfo->DllHandle != nullptr)
			{
				FPlatform::FreeDllHandle(ModuleInfo->DllHandle);
				ModuleInfo->DllHandle = nullptr;
			}

			return true;
		}
	}

	return false;
}

bool FModuleManager::IsModuleLoaded(const FStringId& InModuleName) const
{
	if (!ModuleInfoMap.IsKeyContained(InModuleName))
	{
		return false;
	}

	const TSharedPtr<FModuleInfo>& ModuleInfo = *ModuleInfoMap.Find(InModuleName);
	return ModuleInfo->bReadyToUse;
}
