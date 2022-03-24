#pragma once

#include "CoreMinimal.h"
#include "Texture2D.h"

/**
 * A frame buffer consists of the color, depth, and stencil buffers
 * that are rendered to in the rendering loop. By default, the renderer
 * uses the frame buffer created by the OS upon window creation, which 
 * controls what is shown on-screen. However, by creating our own
 * frame buffers, we can use the output of the renderer in interesting 
 * ways. For example, we can save the output of the renderer as a texture,
 * and display that texture as a panel in an editor.
 */
class FFrameBuffer
{
public:
	explicit FFrameBuffer(int32 InWidth, int32 InHeight);
	~FFrameBuffer();

	// Non-copyable.
	FFrameBuffer(const FFrameBuffer&) = delete;
	FFrameBuffer& operator=(const FFrameBuffer&) = delete;

	// Non-movable.
	FFrameBuffer(FFrameBuffer&&) = delete;
	FFrameBuffer& operator=(FFrameBuffer&&) = delete;

	// Gets the default frame buffer created by the OS upon window creation.
	static TSharedPtr<FFrameBuffer> Default();

	void Bind() const;
	void Unbind() const;
	void Resize(int32 InWidth, int32 InHeight);

	// Getters.
	uint32 GetId() const 
	{ 
		return FrameBufferId; 
	}
	uint32 GetTextureId() const 
	{ 
		return TextureId; 
	}

	int32 GetWidth() const 
	{ 
		return Width;
	}
	int32 GetHeight() const 
	{
		return Height;
	}

private:
	// Default constructor used to create the default frame buffer.
	FFrameBuffer();

	static const uint32 DefaultFrameBufferId /* = 0 */;

	uint32 FrameBufferId;
	
	// Attachment IDs
	uint32 TextureId;
	uint32 RenderBufferId;

	int32 Width;
	int32 Height;
};
