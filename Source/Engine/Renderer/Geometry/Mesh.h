#pragma once

#include "CoreMinimal.h"
#include "Materials/Material.h"
#include "RHI/VertexFormats.h"
#include "RHI/VertexArray.h"

/**
 * A mesh consists of two things: a vertex array and a material.
 * 
 * The vertex array contains the vertex data that describes the mesh's geometry,
 * while the material specifies how to render the vertex data. A mesh's material 
 * can be swapped or modified at runtime, which can decrease iteration times as
 * well as allows for interesting runtime effects.
 */
class FMesh
{
public:
	explicit FMesh(const FStringId& InMeshFileName, const FStringId& InMaterialFileName);
	~FMesh() = default;

	FMesh(const FMesh&) = default;
	FMesh& operator=(const FMesh&) = default;
	FMesh(FMesh&&) = default;
	FMesh& operator=(FMesh&&) = default;

	// Getters.
	FMaterial& GetMaterial();
	const FMaterial& GetMaterial() const;

	const FStringId& GetName() const 
	{
		return MeshFileName; 
	}
	TSharedPtr<TVertexArray<FVertex1P1N1UV> > GetVertexArray()
	{
		return VertexArray; 
	};
	TSharedPtr<const TVertexArray<FVertex1P1N1UV> > GetVertexArray() const 
	{ 
		return VertexArray; 
	};

	// Setters.
	void SetMaterial(const FStringId& InMaterialFileName);

private:
	FStringId MeshFileName;
	FStringId MaterialFileName;
	TSharedPtr<TVertexArray<FVertex1P1N1UV> > VertexArray;
};
