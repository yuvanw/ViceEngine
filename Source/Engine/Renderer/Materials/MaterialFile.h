#pragma once

#include "CoreMinimal.h"

/**
 * A material file (.mat) specifies the shaders that make up the material's shader
 * pipeline, as well as the material's float, color, and texture properties.
 */
class FMaterialFile
{
public:
	explicit FMaterialFile(const FStringId& InMaterialFileName);
	~FMaterialFile() = default;

	FMaterialFile(const FMaterialFile&) = default;
	FMaterialFile& operator=(const FMaterialFile&) = default;
	FMaterialFile(FMaterialFile&&) = default;
	FMaterialFile& operator=(FMaterialFile&&) = default;

private:
	// Shader file paths.
	FStringId VertexShaderFileName;
	FStringId FragmentShaderFileName;
	FStringId GeometryShaderFileName;

	// Float properties.
	TArray<FStringId> FloatPropertyKeys;
	TArray<float> FloatPropertyValues;

	// Color properties.
	TArray<FStringId> ColorPropertyKeys;
	TArray<FColor> ColorPropertyValues;

	// Texture properties.
	TArray<FStringId> TexturePropertyKeys;
	TArray<FStringId> TexturePropertyValues;

	friend class FMaterial;
};
