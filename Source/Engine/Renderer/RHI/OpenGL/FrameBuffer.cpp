#include "FrameBuffer.h"
#include "OpenGLApi.h"

const uint32 FFrameBuffer::DefaultFrameBufferId = 0;

FFrameBuffer::FFrameBuffer(int32 InWidth, int32 InHeight)
	: Width(InWidth)
	, Height(InHeight)
{
	glGenFramebuffers(1, &FrameBufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferId);

	// Add color texture attachment.
	glGenTextures(1, &TextureId);
	glBindTexture(GL_TEXTURE_2D, TextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TextureId, 0);

	// Add depth/stencil RenderBuffer attachment.
	glGenRenderbuffers(1, &RenderBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, RenderBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Width, Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RenderBufferId);
	
	// Check that FrameBuffer is complete.
	ensure(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FFrameBuffer::FFrameBuffer()
	: FrameBufferId(DefaultFrameBufferId)
	// Dummy values.
	, TextureId(0)
	, RenderBufferId(0)
	, Width(0)
	, Height(0)
{
}

FFrameBuffer::~FFrameBuffer()
{
	glDeleteFramebuffers(1, &FrameBufferId);
}

void FFrameBuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferId);
}

void FFrameBuffer::Unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, DefaultFrameBufferId);
}

void FFrameBuffer::Resize(int32 InWidth, int32 InHeight)
{
	// You cannot resize the default frame buffer through OpenGL.
	if (FrameBufferId == DefaultFrameBufferId)
	{
		return;
	}

	Width = InWidth;
	Height = InHeight;

	// Resize attached texture.
	glBindTexture(GL_TEXTURE_2D, TextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Resize attached RenderBuffer.
	glBindRenderbuffer(GL_RENDERBUFFER, RenderBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Width, Height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

/* static */ TSharedPtr<FFrameBuffer> FFrameBuffer::Default()
{
	static TSharedPtr<FFrameBuffer> DefaultFrameBuffer(new FFrameBuffer());
	return DefaultFrameBuffer;
}
