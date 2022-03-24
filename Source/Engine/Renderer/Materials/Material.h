#pragma once

#include "CoreMinimal.h"
#include "Math/Vector4D.h"
#include "RHI/Pipeline.h"
#include "RHI/Texture2D.h"

template <typename PropertyType>
struct FMaterialProperty
{
	explicit FMaterialProperty(const FStringId& InName, const PropertyType& InProperty)
		: Name(InName)
		, Property(InProperty)
	{
	}
	
	FStringId Name;
	PropertyType Property;
};

/**
 * A material consists of a shader pipeline and uniform data that is used in shader pipeline.
 * These uniform data are called material properties and can take the form of floats, colors,
 * or textures. The shader pipeline specifies how a mesh should be renderer, and the material
 * properties specify what data should be used when rendering the mesh.
 */
class FMaterial
{
public:
	explicit FMaterial(const FStringId& InMaterialFileName);
	// Default constructor necessary for using in container classes like TMap.
	FMaterial() = default;
	~FMaterial() = default;

	FMaterial(const FMaterial&) = default;
	FMaterial& operator=(const FMaterial&) = default;
	FMaterial(FMaterial&&) = default;
	FMaterial& operator=(FMaterial&&) = default;
	
	// Getters.
	const FStringId& GetName() const 
	{
		return MaterialFileName;
	}

	TSharedPtr<FPipeline> GetPipeline() 
	{ 
		return Pipeline; 
	}
	TSharedPtr<const FPipeline> GetPipeline() const 
	{ 
		return Pipeline; 
	}
	TArray<FMaterialProperty<float> >& GetFloatProperties() 
	{
		return FloatProperties; 
	};
	const TArray<FMaterialProperty<float> >& GetFloatProperties() const
	{ 
		return FloatProperties; 
	};
	TArray<FMaterialProperty<FColor> >& GetColorProperties() 
	{ 
		return ColorProperties; 
	};
	const TArray<FMaterialProperty<FColor> >& GetColorProperties() const 
	{ 
		return ColorProperties; 
	};
	TArray<FMaterialProperty<FTexture2D> >& GetTextureProperties()
	{ 
		return TextureProperties; 
	};
	const TArray<FMaterialProperty<FTexture2D> >& GetTextureProperties() const 
	{
		return TextureProperties; 
	};

	// Setters.
	void SetFloat(const FStringId& InPropertyName, float InFloat);
	void SetColor(const FStringId& InPropertyName, const FColor& InColor);
	void SetTexture(const FStringId& InPropertyName, const FTexture2D& InTexture);

private:
	FStringId MaterialFileName;

	TSharedPtr<FPipeline> Pipeline;
	
	TArray<FMaterialProperty<float> > FloatProperties;
	TArray<FMaterialProperty<FColor> > ColorProperties;
	TArray<FMaterialProperty<FTexture2D> > TextureProperties;
};
