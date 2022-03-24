#include "Material.h"
#include "MaterialFile.h"

FMaterial::FMaterial(const FStringId& InMaterialFileName)
	: MaterialFileName(InMaterialFileName)
{
	// Parse .mat file.
	FMaterialFile MaterialFile(InMaterialFileName);

	// Create shader pipeline.
	const ANSICHAR* VertexShaderFileName = MaterialFile.VertexShaderFileName.GetString().GetData();
	const ANSICHAR* FragmentShaderFileName = MaterialFile.FragmentShaderFileName.GetString().GetData();
	const ANSICHAR* GeometryShaderFileName = nullptr;
	if (MaterialFile.GeometryShaderFileName != FStringId::Null())
	{
		GeometryShaderFileName = MaterialFile.GeometryShaderFileName.GetString().GetData();
	}

	FShader VertexShader(VertexShaderFileName, EShaderType::Vertex);
	FShader FragmentShader(FragmentShaderFileName, EShaderType::Fragment);
	if (!GeometryShaderFileName)
	{
		Pipeline = MakeShared<FPipeline>(VertexShader, FragmentShader);
	}
	else
	{
		FShader GeometryShader(GeometryShaderFileName, EShaderType::Geometry);
		Pipeline = MakeShared<FPipeline>(VertexShader, FragmentShader, GeometryShader);
	}

	// Store material properties.
	for (int32 Index = 0; Index < MaterialFile.FloatPropertyKeys.GetSize(); ++Index)
	{
		FStringId FloatPropertyName = MaterialFile.FloatPropertyKeys[Index];
		float FloatPropertyValue = MaterialFile.FloatPropertyValues[Index];
		FloatProperties.Emplace(FloatPropertyName, FloatPropertyValue);
	}

	for (int32 Index = 0; Index < MaterialFile.ColorPropertyKeys.GetSize(); ++Index)
	{
		FStringId ColorPropertyName = MaterialFile.ColorPropertyKeys[Index];
		FColor ColorPropertyValue = MaterialFile.ColorPropertyValues[Index];
		ColorProperties.Emplace(ColorPropertyName, ColorPropertyValue);
	}

	ETextureUnit CurrentTextureUnit = ETextureUnit::Zero;
	for (int32 Index = 0; Index < MaterialFile.TexturePropertyKeys.GetSize(); ++Index)
	{
		FStringId TexturePropertyName = MaterialFile.TexturePropertyKeys[Index];
		FStringId TextureFileName = MaterialFile.TexturePropertyValues[Index];
		
		FTexture2D Texture;
		if (TextureFileName != FStringId::Null())
		{
			FTexture2D Texture = FTexture2D(TextureFileName.GetString().GetData(), CurrentTextureUnit);
			TextureProperties.Emplace(TexturePropertyName, Texture);
		}
		else
		{
			// Use default texture as a fallback if a texture wasn't specified in the material file.
			FTexture2D Texture = FTexture2D::Default();
			Texture.SetTextureUnit(CurrentTextureUnit);
			TextureProperties.Emplace(TexturePropertyName, Texture);
		}

		++CurrentTextureUnit;
	}
}

void FMaterial::SetFloat(const FStringId& InPropertyName, float InFloat)
{
	for (FMaterialProperty<float>& FloatProperty : FloatProperties)
	{
		if (FloatProperty.Name == InPropertyName)
		{
			FloatProperty.Property = InFloat;
			return;
		}
	}
}

void FMaterial::SetColor(const FStringId& InPropertyName, const FColor& InColor)
{
	for (FMaterialProperty<FColor>& ColorProperty : ColorProperties)
	{
		if (ColorProperty.Name == InPropertyName)
		{
			ColorProperty.Property = InColor;
			return;
		}
	}
}

void FMaterial::SetTexture(const FStringId& InPropertyName, const FTexture2D& InTexture)
{
	for (FMaterialProperty<FTexture2D>& TextureProperty : TextureProperties)
	{
		if (TextureProperty.Name == InPropertyName)
		{
			ETextureUnit TextureUnit = TextureProperty.Property.GetTextureUnit();
			TextureProperty.Property = InTexture;
			TextureProperty.Property.SetTextureUnit(TextureUnit);
			return;
		}
	}
}
