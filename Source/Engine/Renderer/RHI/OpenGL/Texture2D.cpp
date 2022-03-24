#include "Texture2D.h"
#include "RendererFileSystem.h"
#include "OpenGLApi.h"
#include "RHI/TextureRegistry.h"

#include "stb/stb_image.h"

static constexpr const ANSICHAR* DefaultTextureFileName = "Black.png";

FTexture2D::FTexture2D()
	: TextureFileName(FStringId::Null())
	// "The value zero is reserved to represent the default texture for each texture target."
	// See: https://docs.gl/gl4/glBindTexture
	, Id(0)
	, Width(0)
	, Height(0)
	, TextureUnit(ETextureUnit::Zero)
{
}

FTexture2D::FTexture2D(const FStringId& InTextureFileName, ETextureUnit InTextureUnit /* = ETextureUnit::Zero */)
	: TextureFileName(InTextureFileName)
	, TextureUnit(InTextureUnit)
{
	// Fetch the texture if it has been loaded previously.
	TMap<FStringId, FTexture2D>& Textures = FTextureRegistry::Get().Textures;
	if (Textures.IsKeyContained(TextureFileName))
	{
		const FTexture2D& Texture = *Textures.Find(TextureFileName);
		Id = Texture.GetId();
		Width = Texture.GetWidth();
		Height = Texture.GetHeight();
		return;
	}

	// Load the texture if this is the first time encountering it.
	glGenTextures(1, &Id);
	glBindTexture(GL_TEXTURE_2D, Id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	FStringId TextureFilePath = FRendererFileSystem::GetTextureFilePath(InTextureFileName);
	stbi_set_flip_vertically_on_load(true);

	int32 TextureComponents;
	uint8* Data = stbi_load(TextureFilePath.GetString().GetData(), &Width, &Height, &TextureComponents, 0);
	ensure(Data);

	GLenum TextureFormat;
	switch (TextureComponents)
	{
		case 1:
			TextureFormat = GL_RED;
			break;
		case 2:
			TextureFormat = GL_RG;
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

	glTexImage2D(GL_TEXTURE_2D, 0, TextureFormat, Width, Height, 0, TextureFormat, GL_UNSIGNED_BYTE, Data);
	glGenerateMipmap(GL_TEXTURE_2D);

	Textures.Add(TextureFileName, *this);

	// Release the image data once we've created the texture.
	stbi_image_free(Data);
}

FTexture2D::~FTexture2D()
{
	// glDeleteTextures(1, &Id);
}

void FTexture2D::Bind() const
{
	glActiveTexture(static_cast<GLenum>(TextureUnit));
	glBindTexture(GL_TEXTURE_2D, Id);
}

/*static*/ FTexture2D FTexture2D::Default()
{
	return FTexture2D(DefaultTextureFileName);
}
