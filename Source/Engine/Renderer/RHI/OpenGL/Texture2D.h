#pragma once

#include "CoreMinimal.h"
#include "RHIDefinitions.h"

/**
 * A 2D image used as a lookup table. Conventionally used
 * to add detail to an object (e.g. diffuse maps, normal maps).
 */
class FTexture2D
{
public:
	// Default constructor to allow for usage with container classes.
	FTexture2D();
	explicit FTexture2D(const FStringId& InTextureFileName, ETextureUnit InTextureUnit = ETextureUnit::Zero);
	~FTexture2D();

	FTexture2D(const FTexture2D&) = default;
	FTexture2D& operator=(const FTexture2D&) = default;
	FTexture2D(FTexture2D&&) = default;
	FTexture2D& operator=(FTexture2D&&) = default;

	void Bind() const;

	// Getters.
	const FStringId& GetName() const 
	{
		return TextureFileName; 
	}
	uint32 GetId() const
	{ 
		return Id;
	}
	int32 GetWidth() const 
	{ 
		return Width; 
	}
	int32 GetHeight() const
	{
		return Height; 
	}
	ETextureUnit GetTextureUnit() const
	{
		return TextureUnit; 
	}

	// Setters.
	void SetTextureUnit(ETextureUnit InTextureUnit) 
	{
		TextureUnit = InTextureUnit;
	}

	static FTexture2D Default();

private:
	FStringId TextureFileName;
	uint32 Id;
	int32 Width;
	int32 Height;
	ETextureUnit TextureUnit;
};
