#pragma once

#include "CoreMinimal.h"
#include "RHIDefinitions.h"

/**
 * A program that runs on the GPU and represents one stage
 * of the graphics pipeline. Vertex, fragment, geometry, and
 * compute shaders are supported.
 */
class FShader
{
public:
	FShader(const FStringId& InShaderFileName, EShaderType InShaderType);
	~FShader();

	// Non-copyable.
	FShader(const FShader&) = delete;
	FShader& operator=(const FShader&) = delete;

	// Non-movable.
	FShader(const FShader&&) = delete;
	FShader& operator=(const FShader&&) = delete;

	uint32 GetId() const
	{
		return Id;
	}

private:
	uint32 Id;
};
