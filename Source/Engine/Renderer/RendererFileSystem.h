#pragma once

#include "CoreMinimal.h"

/**
 * File system that provides access to files in the "Assets" folder that the renderer uses.
 * 
 * Note: This is a temporary solution for accessing renderer assets until the official file
 * system and asset manager is implemented in the Core module.
 */
class FRendererFileSystem
{
public:
	static FStringId GetSceneFilePath(const FStringId& InSceneFileName);
	static FStringId GetModelFilePath(const FStringId& InModelFileName);
	static FStringId GetMeshFilePath(const FStringId& InMeshFileName);
	static FStringId GetMaterialFilePath(const FStringId& InMaterialFileName);
	static FStringId GetShaderFilePath(const FStringId& InShaderFileName);
	static FStringId GetTextureFilePath(const FStringId& InTextureFileName);

	static TArray<FStringId> GetAllSceneFileNames();
	static TArray<FStringId> GetAllModelFileNames();
	static TArray<FStringId> GetAllMeshFileNames();
	static TArray<FStringId> GetAllMaterialFileNames();
	static TArray<FStringId> GetAllShaderFileNames();
	static TArray<FStringId> GetAllTextureFileNames();
};
