#include "ModelFile.h"
#include "RendererFileSystem.h"

// Includes used for JSON parsing.
#include <fstream>
#include <string>
#include "json/json.hpp"

namespace
{
	// Fixed key names used in model JSON files.
	struct FModelFileKeys
	{
		static constexpr const ANSICHAR* Meshes = "Meshes";
		static constexpr const ANSICHAR* Geometry = "Geometry";
		static constexpr const ANSICHAR* Material = "Material";
	};
}

FModelFile::FModelFile(const FStringId& InModelFileName)
{
	FStringId ModelFilePath = FRendererFileSystem::GetModelFilePath(InModelFileName);
	
	// Initialize JSON file object.
	std::ifstream ModelFileInputStream(ModelFilePath.GetString().GetData());
	nlohmann::json ModelFileJson;
	ModelFileInputStream >> ModelFileJson;

	for (const auto& Mesh : ModelFileJson[FModelFileKeys::Meshes])
	{
		ensure(!Mesh.is_null());
		ensure(Mesh.contains(FModelFileKeys::Geometry) && !Mesh[FModelFileKeys::Geometry].is_null());
		ensure(Mesh.contains(FModelFileKeys::Material) && !Mesh[FModelFileKeys::Material].is_null());

		// Parse mesh file name.
		std::string MeshFile = Mesh[FModelFileKeys::Geometry];
		FStringId MeshFileName = FStringId(MeshFile.c_str());
		MeshFileNames.Add(MeshFileName);

		// Parse material file name.
		std::string MaterialFile = Mesh[FModelFileKeys::Material];
		FStringId MaterialFileName = FStringId(MaterialFile.c_str());
		MaterialFileNames.Add(MaterialFileName);
	}
}
