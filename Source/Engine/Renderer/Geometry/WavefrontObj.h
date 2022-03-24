#pragma once

#include "CoreMinimal.h"
#include "RHI/VertexFormats.h"

/**
 * Wavefront OBJ file parser.
 */
class FWavefrontObj
{
public:
	explicit FWavefrontObj::FWavefrontObj(const FStringId& InWavefrontObjFileName);
	~FWavefrontObj() = default;

	FWavefrontObj(const FWavefrontObj&) = default;
	FWavefrontObj& operator=(const FWavefrontObj&) = default;
	FWavefrontObj(FWavefrontObj&&) = default;
	FWavefrontObj& operator=(FWavefrontObj&&) = default;

	const TArray<FVertex1P1N1UV>& GetVertices() const 
	{ 
		return Vertices; 
	};
	const TArray<uint32>& GetIndices() const 
	{
		return Indices; 
	};

private:
	TArray<FVertex1P1N1UV> Vertices;
	TArray<uint32> Indices;
};
