#include "RendererFileSystem.h"
#include "RHI/RHIDefinitions.h"
#include "HAL/PreprocessorHelpers.h"
#include "HAL/PlatformFileSystem.h"

#include <string>
#include <fstream>

namespace
{
	struct FDirectoryNames
	{
		static constexpr const ANSICHAR* Root      = TO_STRING(ROOT_FOLDER);
		static constexpr const ANSICHAR* Assets    = "Assets";
		static constexpr const ANSICHAR* Scenes    = "Scenes";
		static constexpr const ANSICHAR* Models    = "Models";
		static constexpr const ANSICHAR* Meshes    = "Meshes";
		static constexpr const ANSICHAR* Materials = "Materials";
		static constexpr const ANSICHAR* Shaders   = FShaderFiles::DirectoryName;
		static constexpr const ANSICHAR* Textures  = "Textures";
	};
}

static FStringId GetDirectoryPath(const ANSICHAR* InDirectoryName)
{
	std::string DirectoryPath = std::string(FDirectoryNames::Root) + "/" + FDirectoryNames::Assets + "/" + InDirectoryName;
	ensure(FPlatformFileSystem::IsValidPath(DirectoryPath.c_str()));
	return FStringId(DirectoryPath.c_str());
}

static FStringId GetFilePath(const ANSICHAR* InDirectoryName, const ANSICHAR* InFileName)
{
	std::string FilePath = std::string(FDirectoryNames::Root) + "/" + FDirectoryNames::Assets + "/" + InDirectoryName + "/" + InFileName;
	ensure(FPlatformFileSystem::IsValidPath(FilePath.c_str()));
	return FStringId(FilePath.c_str());
}

FStringId FRendererFileSystem::GetSceneFilePath(const FStringId& InSceneFileName)
{
	return GetFilePath(FDirectoryNames::Scenes, InSceneFileName.GetString().GetData());
}

FStringId FRendererFileSystem::GetMaterialFilePath(const FStringId& InMaterialFileName)
{
	return GetFilePath(FDirectoryNames::Materials, InMaterialFileName.GetString().GetData());
}

FStringId FRendererFileSystem::GetModelFilePath(const FStringId& InModelFileName)
{
	return GetFilePath(FDirectoryNames::Models, InModelFileName.GetString().GetData());
}

FStringId FRendererFileSystem::GetMeshFilePath(const FStringId& InMeshFileName)
{
	return GetFilePath(FDirectoryNames::Meshes, InMeshFileName.GetString().GetData());
}

FStringId FRendererFileSystem::GetTextureFilePath(const FStringId& InTextureFileName)
{
	return GetFilePath(FDirectoryNames::Textures, InTextureFileName.GetString().GetData());
}

FStringId FRendererFileSystem::GetShaderFilePath(const FStringId& InShaderFileName)
{
	return GetFilePath(FDirectoryNames::Shaders, InShaderFileName.GetString().GetData());
}

TArray<FStringId> FRendererFileSystem::GetAllSceneFileNames()
{
	FStringId DirectoryPath = GetDirectoryPath(FDirectoryNames::Scenes);
	return FPlatformFileSystem::GetAllFileNamesWithinDirectory(DirectoryPath.GetString().GetData());
}

TArray<FStringId> FRendererFileSystem::GetAllModelFileNames()
{
	FStringId DirectoryPath = GetDirectoryPath(FDirectoryNames::Models);
	return FPlatformFileSystem::GetAllFileNamesWithinDirectory(DirectoryPath.GetString().GetData());
}

TArray<FStringId> FRendererFileSystem::GetAllMeshFileNames()
{
	FStringId DirectoryPath = GetDirectoryPath(FDirectoryNames::Meshes);
	return FPlatformFileSystem::GetAllFileNamesWithinDirectory(DirectoryPath.GetString().GetData());
}

TArray<FStringId> FRendererFileSystem::GetAllMaterialFileNames()
{
	FStringId DirectoryPath = GetDirectoryPath(FDirectoryNames::Materials);
	return FPlatformFileSystem::GetAllFileNamesWithinDirectory(DirectoryPath.GetString().GetData());
}

TArray<FStringId> FRendererFileSystem::GetAllShaderFileNames()
{
	FStringId DirectoryPath = GetDirectoryPath(FDirectoryNames::Shaders);
	return FPlatformFileSystem::GetAllFileNamesWithinDirectory(DirectoryPath.GetString().GetData());
}

TArray<FStringId> FRendererFileSystem::GetAllTextureFileNames()
{
	FStringId DirectoryPath = GetDirectoryPath(FDirectoryNames::Textures);
	return FPlatformFileSystem::GetAllFileNamesWithinDirectory(DirectoryPath.GetString().GetData());
}
