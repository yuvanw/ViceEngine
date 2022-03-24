#pragma once

#include "CoreGlobals.h"
#include "HAL/Platform.h"
#include "Memory/MemoryManager.h"
#include "Memory/ArenaAllocator.h"

class IModule;

#define PER_MODULE_OPERATOR_NEW_AND_DELETE \
	/* For now, each module is given an allocator with 5 mb of memory. */ \
	static int32 SizeBytes = 5 * 1024 * 1024; \
	static void* MemoryStart = FMemoryManager::Get().GetArenaAllocator().Allocate(SizeBytes); \
	static FArenaAllocator Allocator(MemoryStart, SizeBytes); \
	void* operator new(size_t InSize) { return Allocator.Allocate(InSize); } \
	void* operator new[](size_t InSize) { return Allocator.Allocate(InSize); } \
	void operator delete(void* Ptr) { Allocator.Deallocate(Ptr); } \
	void operator delete[](void* Ptr) { Allocator.Deallocate(Ptr); } \
	void* operator new(size_t InSize, EMemoryAlignment InAlignment) { return Allocator.Allocate(InSize, InAlignment); } \
	void* operator new[](size_t InSize, EMemoryAlignment InAlignment) { return Allocator.Allocate(InSize, InAlignment); }

#define IMPLEMENT_MODULE( ModuleImplClass ) \
	/* This function uses extern C so that we avoid name mangling. */ \
	/* When we link, we need to look for a function with this exact name in each module's DLL, which is why we need to avoid name mangling. */ \
	extern "C" DLLEXPORT IModule* InitializeModule() \
	{ \
		return new ModuleImplClass(); \
	} \
    /* that IMPLEMENT_MODULE is being used (i.e. Unreal's IMPLEMENT_MODULE_##ModuleName function) */ \
    PER_MODULE_OPERATOR_NEW_AND_DELETE
