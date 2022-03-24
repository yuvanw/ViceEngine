#pragma once

#include "CoreMinimal.h"
#include "RHI/VertexFormats.h"
#include "RHIDefinitions.h"
#include "OpenGLApi.h"

/**
 * A vertex array consists of a vertex buffer and an optional index buffer.
 * These two buffers are used in conjunction to define the geometry of a mesh
 * (see FMesh).
 */
template <typename VertexFormat>
class TVertexArray
{
public:
	explicit TVertexArray(const TArray<VertexFormat>& InVertices, EBufferUsage InBufferUsage = EBufferUsage::Static);
	explicit TVertexArray(const TArray<VertexFormat>& InVertices, const TArray<uint32>& InIndices, EBufferUsage InBufferUsage = EBufferUsage::Static);
	~TVertexArray();

	// Non-copyable.
	TVertexArray(const TVertexArray&) = delete;
	TVertexArray& operator=(const TVertexArray&) = delete;

	// Non-movable.
	TVertexArray(TVertexArray&&) = delete;
	TVertexArray& operator=(TVertexArray&&) = delete;

	void Bind() const;

	// Getters.
	uint32 GetId() const
	{
		return VertexArrayId;
	}
	int32 GetNumVertices() const
	{
		return NumVertices;
	}	
	int32 GetNumIndices() const
	{
		return NumIndices;
	}

private:
	uint32 VertexArrayId;
	uint32 VertexBufferId;
	uint32 IndexBufferId;

	int32 NumVertices;
	int32 NumIndices;

	void BindVertexBuffer(const TArray<VertexFormat>& InVertices, EBufferUsage InBufferUsage);
	void BindIndexBuffer(const TArray<uint32>& InIndices, EBufferUsage InBufferUsage);
};

template <typename VertexFormat>
static void BindVertexAttributes();

template <typename VertexFormat>
TVertexArray<VertexFormat>::TVertexArray(const TArray<VertexFormat>& InVertices, EBufferUsage InBufferUsage = EBufferUsage::Static)
	: NumVertices(InVertices.GetSize())
	, NumIndices(0)
{
	glGenVertexArrays(1, &VertexArrayId);
	glGenBuffers(1, &VertexBufferId);

	glBindVertexArray(VertexArrayId);
	BindVertexBuffer(InVertices, InBufferUsage);
	glBindVertexArray(0);
}

template <typename VertexFormat>
TVertexArray<VertexFormat>::TVertexArray(const TArray<VertexFormat>& InVertices, const TArray<uint32>& InIndices, EBufferUsage InBufferUsage = EBufferUsage::Static)
	: NumVertices(InVertices.GetSize())
	, NumIndices(InIndices.GetSize())
{
	glGenVertexArrays(1, &VertexArrayId);
	glGenBuffers(1, &VertexBufferId);
	glGenBuffers(1, &IndexBufferId);

	glBindVertexArray(VertexArrayId);
	BindVertexBuffer(InVertices, InBufferUsage);
	BindIndexBuffer(InIndices, InBufferUsage);
	glBindVertexArray(0);
}

template <typename VertexFormat>
TVertexArray<VertexFormat>::~TVertexArray()
{
	glDeleteVertexArrays(1, &VertexArrayId);
	glDeleteBuffers(1, &VertexBufferId);
	glDeleteBuffers(1, &IndexBufferId);
}

template <typename VertexFormat>
void TVertexArray<VertexFormat>::Bind() const
{
	glBindVertexArray(VertexArrayId);
}

template <typename VertexFormat>
void TVertexArray<VertexFormat>::BindVertexBuffer(const TArray<VertexFormat>& InVertices, EBufferUsage InBufferUsage)
{
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferId);
	
	glBufferData(
		GL_ARRAY_BUFFER,
		// Size of the vertex attribute array.
		InVertices.GetSize() * sizeof(VertexFormat),
		// Pointer to the start of the vertex attribute array.
		InVertices.GetData(),
		// Expected usage pattern. See EBufferUsage for details.
		static_cast<GLenum>(InBufferUsage)
	);

	// Specify how OpenGL should parse each vertex's data.
	BindVertexAttributes<VertexFormat>();
}

template <typename VertexFormat>
void TVertexArray<VertexFormat>::BindIndexBuffer(const TArray<uint32>& InIndices, EBufferUsage InBufferUsage)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferId);
	
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		// Size of the index array.
		InIndices.GetSize() * sizeof(uint32),
		// Pointer to the start of the index array.
		InIndices.GetData(),
		// Expected usage pattern. See EBufferUsage for details.
		static_cast<GLenum>(InBufferUsage)
	);
}

template <>
static void BindVertexAttributes<FVertex1P1N1UV>()
{
	// Position attribute.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Texture coordinate attribute.
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}
