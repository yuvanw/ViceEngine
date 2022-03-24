#pragma once

#include "CoreMinimal.h"
#include "Material.h"

/**
 * A registry of material file names represented as hashed string IDs 
 * to FMaterials for fast retrieval and single storage.
 */
class FMaterialRegistry
{
public:
	static FMaterialRegistry& Get()
	{
		static FMaterialRegistry MaterialRegistry;
		return MaterialRegistry;
	}

	void AddMaterial(const FStringId& InMaterialName)
	{
		FMaterial Material(InMaterialName.GetString().GetData());
		MaterialRegistry.Add(InMaterialName, Material);
	}

	FMaterial* GetMaterial(const FStringId& InMaterialName)
	{
		return MaterialRegistry.Find(InMaterialName);
	}

	bool HasMaterial(const FStringId& InMaterialName)
	{
		return MaterialRegistry.IsKeyContained(InMaterialName);
	}

private:
	FMaterialRegistry() = default;

	// Material file name to FMaterial map.
	TMap<FStringId, FMaterial> MaterialRegistry;

	friend class FMaterial;
};
