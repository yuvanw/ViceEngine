#include "MaterialFile.h"
#include "RendererFileSystem.h"

// Includes used for JSON parsing.
#include <fstream>
#include <string>
#include "json/json.hpp"

namespace
{
	// Fixed key names used in material JSON files.
	struct FMaterialFileKeys
	{
		/**
		* We're using both constexpr and const here because when constexpr is applied
		* to pointer types (i.e. constexpr T*), it acts like a const pointer (i.e. T* const)
		* known at compile-time. In order to get a const-pointer-to-const, we need to apply
		* the const keyword before the asterisk as well.
		*/
		static constexpr const ANSICHAR* Name = "Name";

		static constexpr const ANSICHAR* Pipeline = "Pipeline";
		static constexpr const ANSICHAR* VertexShader = "Vertex";
		static constexpr const ANSICHAR* FragmentShader = "Fragment";
		static constexpr const ANSICHAR* GeometryShader = "Geometry";
		
		static constexpr const ANSICHAR* Properties = "Properties";
		static constexpr const ANSICHAR* Floats = "Floats";
		static constexpr const ANSICHAR* Colors = "Colors";
		static constexpr const ANSICHAR* Textures = "Textures";
	};
}

static FColor ParseColor(const nlohmann::json& InJsonArray);

FMaterialFile::FMaterialFile(const FStringId& InMaterialFileName)
{
	FStringId MaterialFilePath = FRendererFileSystem::GetMaterialFilePath(InMaterialFileName);

	// Initialize JSON file object.
	std::ifstream MaterialFileInputStream(MaterialFilePath.GetString().GetData());
	nlohmann::json MaterialFileJson;
	MaterialFileInputStream >> MaterialFileJson;

	// Parse shader file names.
	const auto& Pipeline = MaterialFileJson[FMaterialFileKeys::Pipeline];
	ensure(!Pipeline.is_null());
	ensure(Pipeline.contains(FMaterialFileKeys::VertexShader) && !Pipeline[FMaterialFileKeys::VertexShader].is_null()); 
	ensure(Pipeline.contains(FMaterialFileKeys::FragmentShader) && !Pipeline[FMaterialFileKeys::FragmentShader].is_null());
	
	std::string VertexShader = Pipeline[FMaterialFileKeys::VertexShader];
	std::string FragmentShader = Pipeline[FMaterialFileKeys::FragmentShader];
	VertexShaderFileName = FStringId(VertexShader.c_str());
	FragmentShaderFileName = FStringId(FragmentShader.c_str());
	
	if (Pipeline.contains(FMaterialFileKeys::GeometryShader) && !Pipeline[FMaterialFileKeys::GeometryShader].is_null())
	{
		std::string GeometryShader = Pipeline[FMaterialFileKeys::GeometryShader];
		GeometryShaderFileName = FStringId(GeometryShader.c_str());
	}

	const auto& Properties = MaterialFileJson[FMaterialFileKeys::Properties];
	ensure(!Properties.is_null());
	
	// Parse float properties.
	if (Properties.contains(FMaterialFileKeys::Floats))
	{
		for (const auto& FloatProperty : Properties[FMaterialFileKeys::Floats].items())
		{
			FStringId Key = FloatProperty.key().c_str();
			FloatPropertyKeys.Add(Key);

			if (!FloatProperty.value().is_null())
			{
				float Value = FloatProperty.value().get<float>();
				FloatPropertyValues.Add(Value);
			}
			else
			{
				// Assign default value if float property value isn't specified.
				FloatPropertyValues.Add(0.0f);
			}
		}
	}

	// Parse color properties.
	if (Properties.contains(FMaterialFileKeys::Colors))
	{
		for (const auto& ColorProperty : Properties[FMaterialFileKeys::Colors].items())
		{
			FStringId Key = ColorProperty.key().c_str();
			ColorPropertyKeys.Add(Key);

			if (!ColorProperty.value().is_null())
			{
				ensure(ColorProperty.value().is_array());
				FColor Color = ParseColor(ColorProperty.value());
				ColorPropertyValues.Add(Color);
			}
			else
			{
				// Assign default value if color property value isn't specified.
				ColorPropertyValues.Add(FColor::White);
			}
		}
	}

	// Parse texture properties.
	if (Properties.contains(FMaterialFileKeys::Textures))
	{
		for (const auto& TextureProperty : Properties[FMaterialFileKeys::Textures].items())
		{
			FStringId Key = TextureProperty.key().c_str();
			TexturePropertyKeys.Add(Key);

			if (!TextureProperty.value().is_null())
			{
				FStringId Value = TextureProperty.value().get<std::string>().c_str();
				TexturePropertyValues.Add(Value);
			}
			else
			{
				// Assign default value if texture property value isn't specified.
				TexturePropertyValues.Add(FStringId::Null());
			}
		}
	}
}

static FColor ParseColor(const nlohmann::json& InJsonArray)
{
	ensure(InJsonArray[0].is_number());
	ensure(InJsonArray[1].is_number());
	ensure(InJsonArray[2].is_number());
	return FColor(InJsonArray[0], InJsonArray[1], InJsonArray[2]);
}
