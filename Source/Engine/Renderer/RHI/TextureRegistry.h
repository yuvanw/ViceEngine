#pragma once

#include "CoreMinimal.h"
#include "Texture2D.h"

/**
 * A registry of texture file names represented as hashed string IDs
 * to FTexture2Ds for fast retrieval and single storage. Only textures 
 * loaded from texture files are added to the registry (i.e. empty
 * textures used in FFrameBuffers are not added to the registry).
 */
class FTextureRegistry
{
public:
	static FTextureRegistry& Get()
	{
		static FTextureRegistry TextureRegistry;
		return TextureRegistry;
	}

private:
	FTextureRegistry() = default;
	~FTextureRegistry() = default;

	// Texture file name to FTexture2D map.
	TMap<FStringId, FTexture2D> Textures;

	friend class FTexture2D;
};
