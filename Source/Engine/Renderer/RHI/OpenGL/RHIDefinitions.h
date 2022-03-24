#pragma once

#include "CoreMinimal.h"
#include "OpenGLApi.h"

enum class EShaderType : uint16
{
	Vertex = GL_VERTEX_SHADER,
	Fragment = GL_FRAGMENT_SHADER,
	Geometry = GL_GEOMETRY_SHADER,
	Compute = GL_COMPUTE_SHADER
};

enum class EBufferUsage : uint16
{
	// Buffer data will be modified once and used at most a few times.
	Stream = GL_STREAM_DRAW,
	// Buffer data will be will be modified once and used many times.
	Static = GL_STATIC_DRAW,
	// Buffer data will be be modified repeatedly and used many times.
	Dynamic = GL_DYNAMIC_DRAW
};

// Polygon rasterization mode.
enum class EDrawingMode : uint16
{
	Points = GL_POINT,
	Wireframe = GL_LINE,
	Filled = GL_FILL
};

// Texture location. By assigning different textures to different
// texture units, we can use multiple textures in a single shader.
enum class ETextureUnit : uint16
{
	// OpenGL offers at least 16 texture units, but we're only using five for now.
	Zero  = GL_TEXTURE0,
	One   = GL_TEXTURE1,
	Two   = GL_TEXTURE2,
	Three = GL_TEXTURE3,
	Four  = GL_TEXTURE4,
	Five  = GL_TEXTURE5
};

static ETextureUnit& operator++(ETextureUnit& InTextureUnit)
{
	switch (InTextureUnit)
	{
		case ETextureUnit::Zero:
			return InTextureUnit = ETextureUnit::One;
			break;
		case ETextureUnit::One:
			return InTextureUnit = ETextureUnit::Two;
			break;
		case ETextureUnit::Two:
			return InTextureUnit = ETextureUnit::Three;
			break;
		case ETextureUnit::Three:
			return InTextureUnit = ETextureUnit::Four;
			break;
		case ETextureUnit::Four:
			return InTextureUnit = ETextureUnit::Five;
			break;
		default:
			// Assert if there are no more texture units available.
			ensure(false);
			break;
	}

	// Return statement here to get rid of compiler warnings.
	return InTextureUnit;
}

static ETextureUnit operator++(ETextureUnit& InTextureUnit, int)
{
	ETextureUnit TextureUnit = InTextureUnit;
	++InTextureUnit;
	return TextureUnit;
}

struct FShaderFiles
{
	// Name of the folder that OpenGL shaders are located under the "Assets/Shaders/" folder.
	static constexpr const ANSICHAR* DirectoryName = "Shaders/OpenGL";
	// GLSL version.
	static constexpr const ANSICHAR* LanguageVersion = "#version 330";
};

// Names of uniform variables used in OpenGL shaders.
struct FUniformNames
{
	// Matrix uniforms
	static constexpr const ANSICHAR* ModelMatrix = "Matrices.Model";
	static constexpr const ANSICHAR* ViewMatrix = "Matrices.View";
	static constexpr const ANSICHAR* ProjectionMatrix = "Matrices.Projection";

	// Material uniforms
	static constexpr const ANSICHAR* Material = "Material";
	
	// Light uniforms
	static constexpr const ANSICHAR* NumDirectionalLights = "NumDirectionalLights";
	static constexpr const ANSICHAR* DirectionalLights = "DirectionalLights";
	static constexpr const ANSICHAR* NumPointLights = "NumPointLights";
	static constexpr const ANSICHAR* PointLights = "PointLights";
	
	// Camera uniforms
	static constexpr const ANSICHAR* CameraPosition = "CameraPosition";
};
