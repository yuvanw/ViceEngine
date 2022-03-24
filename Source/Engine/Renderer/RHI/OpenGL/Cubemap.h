#pragma once

#include "CoreMinimal.h"

/**
 * A texture consisting of six 2D textures, each of which corresponds
 * to the face of a cube. Cubemaps are used to texture skyboxes (see FSkybox).
 */
class FCubemap
{
public:
	explicit FCubemap(
		const FStringId& InRightTextureFileName,
		const FStringId& InLeftTextureFileName,
		const FStringId& InTopTextureFileName,
		const FStringId& InBottomTextureFileName,
		const FStringId& InBackTextureFileName,
		const FStringId& InFrontTextureFileName
	);
	~FCubemap() = default;

	FCubemap(const FCubemap&) = default;
	FCubemap& operator=(const FCubemap&) = default;
	FCubemap(FCubemap&&) = default;
	FCubemap& operator=(FCubemap&&) = default;

	void Bind() const;

	uint32 GetId() const
	{ 
		return Id; 
	}

private:
	uint32 Id;
};
