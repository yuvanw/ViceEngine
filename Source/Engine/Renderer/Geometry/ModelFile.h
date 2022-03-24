#pragma once

#include "CoreMinimal.h"

/**
 * A model file (.mdl) specifies the meshes that make up a model,
 * as well as the material used per mesh.
 */
class FModelFile
{
public:
	explicit FModelFile(const FStringId& InModelFileName);
	~FModelFile() = default;

	FModelFile(const FModelFile&) = default;
	FModelFile& operator=(const FModelFile&) = default;
	FModelFile(FModelFile&&) = default;
	FModelFile& operator=(FModelFile&&) = default;

private:
	TArray<FStringId> MeshFileNames;
	TArray<FStringId> MaterialFileNames;

	friend class FModel;
};
