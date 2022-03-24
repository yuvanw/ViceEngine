#pragma once

#include "CoreMinimal.h"
#include "Shader.h"

class FVector2D;
class FVector3D;
class FVector4D;
class FMatrix4D;

/**
 * A pipeline is the set of user-configurable shaders used in the graphics pipeline.
 * The user must provide a vertex and fragment shader and can optionally provide
 * a geometry shader. While the stages of the graphics pipeline remain constant,
 * different combinations of vertex, fragment, and geometry shaders can be used 
 * to control how rendering is done.
 */
class FPipeline
{
public:
	explicit FPipeline(const FShader& InVertexShader, const FShader& InFragmentShader);
	explicit FPipeline(const FShader& InVertexShader, const FShader& InFragmentShader, const FShader& InGeometryShader);
	~FPipeline();

	// Non-copyable.
	FPipeline(const FPipeline&) = delete;
	FPipeline& operator=(const FPipeline&) = delete;

	// Non-movable.
	FPipeline(const FPipeline&&) = delete;
	FPipeline& operator=(const FPipeline&&) = delete;

	void Bind() const;

	void SetBool(const FStringId& InUniformName, bool InBool);
	void SetInt(const FStringId& InUniformName, int32 InInt);
	void SetFloat(const FStringId& InUniformName, float InFloat);
	void SetVector2D(const FStringId& InUniformName, const FVector2D& InVector2D);
	void SetVector3D(const FStringId& InUniformName, const FVector3D& InVector3D);
	void SetVector4D(const FStringId& InUniformName, const FVector4D& InVector4D);
	void SetMatrix4D(const FStringId& InUniformName, const FMatrix4D& InMatrix4D);
	
	uint32 GetId() const 
	{ 
		return Id; 
	}

private:
	uint32 Id;
};
