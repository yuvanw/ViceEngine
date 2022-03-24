cmake_minimum_required(VERSION 3.16)

add_library(Core)

target_sources(Core
	PUBLIC AssertionMacros.h
	PUBLIC CoreGlobals.h
	PUBLIC CoreMinimal.h

	PUBLIC Containers/Array.h
	PUBLIC Containers/KeyOperationsPolicyBase.h
	PUBLIC Containers/Map.h
	PUBLIC Containers/Set.h

	PUBLIC HAL/PreprocessorHelpers.h
	PUBLIC HAL/Platform.h
	PUBLIC HAL/PlatformFileSystem.h
	PUBLIC HAL/PlatformTime.h

	PUBLIC Hash/PrimitiveTypeHash.h
	PUBLIC Hash/Crc64.h
	PRIVATE Hash/Crc64.cpp

	PUBLIC GenericPlatform/GenericPlatform.h
	PUBLIC GenericPlatform/GenericPlatformFileSystem.h
	PUBLIC GenericPlatform/GenericPlatformTime.h

	PUBLIC Math/MathUtilities.h
	PRIVATE Math/MathUtilities.cpp
	PUBLIC Math/Vector2D.h
	PUBLIC Math/Vector3D.h
	PUBLIC Math/Vector4D.h
	PUBLIC Math/Color.h
	PUBLIC Math/Transform4D.h
	PUBLIC Math/Matrix4D.h
	PRIVATE Math/Scalar/Vector2D.h
	PRIVATE Math/Scalar/Vector3D.h
	PRIVATE Math/Scalar/Vector4D.h
	PRIVATE Math/Scalar/Color.h
	PRIVATE Math/Scalar/Color.cpp
	PRIVATE Math/Scalar/Transform4D.h
	PRIVATE Math/Scalar/Matrix4D.h
	PRIVATE Math/Scalar/Vector2D.cpp
	PRIVATE Math/Scalar/Vector3D.cpp
	PRIVATE Math/Scalar/Vector4D.cpp
	PRIVATE Math/Scalar/Transform4D.cpp
	PRIVATE Math/Scalar/Matrix4D.cpp

	PRIVATE Memory/AlignmentUtilities.h
	PRIVATE Memory/ArenaAllocator.cpp
	PUBLIC Memory/ArenaAllocator.h
	PUBLIC Memory/MemoryManager.h
	PRIVATE Memory/MemoryManager.cpp
	PRIVATE Memory/NewDeleteAllocator.h
	PUBLIC Memory/Alignment.h
	PUBLIC Memory/IAllocator.h

	PUBLIC Module/ModuleManager.h
	PUBLIC Module/ModuleMacros.h
	PUBLIC Module/DefaultModule.h
	INTERFACE Module/IModule.h
	PRIVATE Module/ModuleManager.cpp

	PUBLIC SmartPointers/UniquePtr.h
	PUBLIC SmartPointers/SharedPtr.h
	PUBLIC SmartPointers/WeakPtr.h
	PRIVATE SmartPointers/ControlBlock.h

	PUBLIC Strings/String.h
	PUBLIC Strings/StringId.h
	PRIVATE Strings/StringId.cpp
	PRIVATE Strings/StringIdRegistry.h

	PUBLIC Templates/TemplateFunctionLibrary.h
	PUBLIC Templates/TypeTraits/AndOrNot.h
	PUBLIC Templates/TypeTraits/CallTraits.h
	PUBLIC Templates/TypeTraits/IsArithmeticType.h
	PUBLIC Templates/TypeTraits/IsPODType.h
	PUBLIC Templates/TypeTraits/IsPointerType.h
	PUBLIC Templates/TypeTraits/AreTypesEqual.h
	PUBLIC Templates/TypeTraits/Identity.h
	PUBLIC Templates/TypeTraits/IsConstType.h
	PUBLIC Templates/TypeTraits/IsLValueReferenceType.h   
	PUBLIC Templates/TypeTraits/IsRValueReferenceType.h
	PUBLIC Templates/TypeTraits/RemoveReference.h 
	PUBLIC Templates/TypeTraits/RemoveReference.h
	PUBLIC Templates/TypeTraits/IsDefaultConstructable.h
	PUBLIC Templates/TypeTraits/IsTriviallyDestructable.h
	PUBLIC Templates/TypeTraits/TRValueToLValueReference.h 
)

target_include_directories(Core
	PUBLIC ${CMAKE_CURRENT_LIST_DIR}
)

# Add preprocessor definition to choose between scalar and SIMD implementations of math classes.
if(USE_SSE)
	target_compile_definitions(Core PUBLIC MATH_USE_SSE)
endif()

# Add platform specific files 
if(PLATFORM STREQUAL "Windows")
	target_sources(Core
		PUBLIC Windows/WindowsPlatform.h
		PRIVATE Windows/WindowsPlatform.cpp
		PUBLIC Windows/WindowsPlatformFileSystem.h
		PRIVATE Windows/WindowsPlatformFileSystem.cpp
		PUBLIC Windows/WindowsPlatformTime.h
		PRIVATE Windows/WindowsPlatformTime.cpp
	)

	target_link_libraries(Core
		# Library that includes PathFileExists function used in the engine file system.
		Shlwapi.dll
	)
elseif(PLATFORM STREQUAL "Mac")
	target_sources(Core
		PUBLIC Mac/MacPlatform.h
		PRIVATE Mac/MacPlatform.cpp
	)

	# Link to cocoa for Mac 
	target_link_libraries(Core
		"-framework cocoa"
	)
endif()
