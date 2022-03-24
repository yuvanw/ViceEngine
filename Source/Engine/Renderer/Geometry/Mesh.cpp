#include "Mesh.h"
#include "WavefrontObj.h"
#include "Materials/MaterialRegistry.h"

FMesh::FMesh(const FStringId& InMeshFileName, const FStringId& InMaterialFileName)
	: MeshFileName(InMeshFileName)
	, MaterialFileName(InMaterialFileName)
	, VertexArray(nullptr)
{
	if (!FMaterialRegistry::Get().HasMaterial(MaterialFileName))
	{
		FMaterialRegistry::Get().AddMaterial(MaterialFileName);
	}

	FWavefrontObj WavefrontObj(MeshFileName.GetString().GetData());
	VertexArray = MakeShared<TVertexArray<FVertex1P1N1UV> >(WavefrontObj.GetVertices(), WavefrontObj.GetIndices());
}

FMaterial& FMesh::GetMaterial()
{
	FMaterial* Material = FMaterialRegistry::Get().GetMaterial(MaterialFileName);
	ensure(Material);
	return *Material;
}

const FMaterial& FMesh::GetMaterial() const
{
	FMaterial* Material = FMaterialRegistry::Get().GetMaterial(MaterialFileName);
	ensure(Material);
	return *Material;
}

void FMesh::SetMaterial(const FStringId& InMaterialFileName)
{ 
	MaterialFileName = InMaterialFileName;
	if (!FMaterialRegistry::Get().HasMaterial(MaterialFileName))
	{
		FMaterialRegistry::Get().AddMaterial(MaterialFileName);
	}
}
