#include "Cubemap.h"

#include "RendererFileSystem.h"
#include "RHI/TextureRegistry.h"
#include "OpenGLApi.h"

#include "stb/stb_image.h"

static void LoadTexture(const FStringId& InTextureFileName, int32 InTextureIndex);

FCubemap::FCubemap(
	const FStringId& InRightTextureFileName,
	const FStringId& InLeftTextureFileName,
	const FStringId& InTopTextureFileName,
	const FStringId& InBottomTextureFileName,
	const FStringId& InBackTextureFileName,
	const FStringId& InFrontTextureFileName
)
{
	glGenTextures(1, &Id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Id);

	// Load textures for each face of the cubemap.
	LoadTexture(InRightTextureFileName, 0);
	LoadTexture(InLeftTextureFileName, 1);
	LoadTexture(InTopTextureFileName, 2);
	LoadTexture(InBottomTextureFileName, 3);
	LoadTexture(InBackTextureFileName, 4);
	LoadTexture(InFrontTextureFileName, 5);

	// Set texture filtering and wrapping parameters.
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void FCubemap::Bind() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Id);
}

static void LoadTexture(const FStringId& InTextureFileName, int32 InTextureIndex)
{
	FStringId TextureFilePath = FRendererFileSystem::GetTextureFilePath(InTextureFileName);
	stbi_set_flip_vertically_on_load(false);

	int32 Width;
	int32 Height;
	int32 TextureComponents;
	uint8* Data = stbi_load(TextureFilePath.GetString().GetData(), &Width, &Height, &TextureComponents, 0);
	ensure(Data);

	GLenum TextureFormat;
	switch (TextureComponents)
	{
	case 1:
		TextureFormat = GL_RED;
		break;
	case 3:
		TextureFormat = GL_RGB;
		break;
	case 4:
		TextureFormat = GL_RGBA;
		break;
	default:
		// Texture format not supported.
		ensure(false);
	}

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + InTextureIndex, 0, TextureFormat, Width, Height, 0, TextureFormat, GL_UNSIGNED_BYTE, Data);

	// Release the image data once we've created the texture.
	stbi_image_free(Data);
}
