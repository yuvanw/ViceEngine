#include "WavefrontObj.h"
#include "RendererFileSystem.h"

#include <fstream>
#include <string>
#include <sstream>

namespace
{
	struct FWavefrontObjElements
	{
		static constexpr const ANSICHAR* Position = "v";
		static constexpr const ANSICHAR* Normal = "vn";
		static constexpr const ANSICHAR* TextureCoordinates = "vt";
		static constexpr const ANSICHAR* Face = "f";
	};
}

static TArray<std::string> GetElementComponents(const std::string& InLine);
static FVector2D ParseVector2D(const std::string& InX, const std::string& InY);
static FVector3D ParseVector3D(const std::string& InX, const std::string& InY, const std::string& InZ);
static TArray<int32> ParseFaceIndices(const std::string& InFace);

FWavefrontObj::FWavefrontObj(const FStringId& InWavefrontObjFileName)
	: Vertices()
	, Indices()
{
	FStringId WavefrontObjFilePath = FRendererFileSystem::GetMeshFilePath(InWavefrontObjFileName);
	std::ifstream WavefrontObjInputStream(WavefrontObjFilePath.GetString().GetData());

	// Vertex attributes.
	TArray<FVector3D> Positions;
	TArray<FVector3D> Normals;
	TArray<FVector2D> TextureCoordinates;

	// Vertex attribute indices.
	TArray<int32> PositionIndices;
	TArray<int32> TextureCoordinateIndices;
	TArray<int32> NormalIndices;

	std::string CurrentElement;
	while (std::getline(WavefrontObjInputStream, CurrentElement))
	{
		TArray<std::string> ElementComponents = GetElementComponents(CurrentElement);
		std::string ElementName = ElementComponents[0];
		if (ElementName == FWavefrontObjElements::Position)
		{
			FVector3D Position = ParseVector3D(ElementComponents[1], ElementComponents[2], ElementComponents[3]);
			Positions.Add(Position);
		}
		else if (ElementName == FWavefrontObjElements::Normal)
		{
			FVector3D Normal = ParseVector3D(ElementComponents[1], ElementComponents[2], ElementComponents[3]);
			Normals.Add(Normal);
		}
		else if (ElementName == FWavefrontObjElements::TextureCoordinates)
		{
			FVector2D TextureCoordinate = ParseVector2D(ElementComponents[1], ElementComponents[2]);
			TextureCoordinates.Add(TextureCoordinate);
		}
		else if (ElementName == FWavefrontObjElements::Face)
		{
			for (int Index = 1; Index < ElementComponents.GetSize(); ++Index)
			{
				TArray<int32> FaceIndices = ParseFaceIndices(ElementComponents[Index]);
				PositionIndices.Add(FaceIndices[0]);
				TextureCoordinateIndices.Add(FaceIndices[1]);
				NormalIndices.Add(FaceIndices[2]);
			}
		}
	}

	int32 NumIndices = PositionIndices.GetSize();
	for (int32 Index = 0; Index < NumIndices; ++Index)
	{
		// Create vertex using indices specified per face.
		int32 PositionIndex = PositionIndices[Index];
		int32 NormalIndex = NormalIndices[Index];
		int32 TextureCoordinateIndex = TextureCoordinateIndices[Index];
		FVertex1P1N1UV Vertex(Positions[PositionIndex], Normals[NormalIndex], TextureCoordinates[TextureCoordinateIndex]);

		// Store the vertex if we haven't encountered it already.
		int32 VertexIndex = Vertices.GetIndexOf(Vertex);
		if (VertexIndex == InvalidIndex)
		{
			Vertices.Add(Vertex);
			VertexIndex = Vertices.GetSize() - 1;
		}
	
		Indices.Add(VertexIndex);
	}
}

static TArray<std::string> GetElementComponents(const std::string& InLine)
{
	TArray<std::string> ElementComponents;
	std::string Component;
	std::stringstream LineStream(InLine);

	// Element components are split using spaces.
	while (std::getline(LineStream, Component, ' '))
	{
		ElementComponents.Add(Component);
	}
	return ElementComponents;
}

static FVector2D ParseVector2D(const std::string& InX, const std::string& InY)
{
	float X = std::atof(InX.c_str());
	float Y = std::atof(InY.c_str());
	return FVector2D(X, Y);
}

static FVector3D ParseVector3D(const std::string& InX, const std::string& InY, const std::string& InZ)
{
	float X = std::atof(InX.c_str());
	float Y = std::atof(InY.c_str());
	float Z = std::atof(InZ.c_str());
	return FVector3D(X, Y, Z);
}

static TArray<int32> ParseFaceIndices(const std::string& InFace)
{
	TArray<int32> FaceIndices;
	std::string IndexString;
	std::stringstream FaceStream(InFace);

	while (std::getline(FaceStream, IndexString, '/'))
	{
		// Index numbers in Wavefront OBJ files start from 1.
		int32 Index = std::atoi(IndexString.c_str()) - 1;
		FaceIndices.Add(Index);
	}

	ensure(FaceIndices.GetSize() == 3);
	return FaceIndices;
}
