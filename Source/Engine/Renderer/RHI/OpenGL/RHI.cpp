#include "RHI.h"
#include "RHIDefinitions.h"
#include "OpenGLApi.h"
#include "HAL/PlatformOpenGL.h"

void FRHI::Init(void* InNativeWindowHandle)
{
	FPlatformOpenGL::Init(InNativeWindowHandle);
}

void FRHI::Shutdown()
{
	FPlatformOpenGL::Shutdown();
}

void FRHI::SwapBuffers()
{
	FPlatformOpenGL::SwapBuffers();
}

void FRHI::SetViewport(const FViewport& InViewport)
{
	glViewport(InViewport.X, InViewport.Y, InViewport.Width, InViewport.Height);
}

void FRHI::SetDrawingMode(EDrawingMode InDrawingMode)
{
	GLenum PolygonMode = static_cast<GLenum>(InDrawingMode);
	glPolygonMode(GL_FRONT_AND_BACK, PolygonMode);
}

void FRHI::SetPointSize(float InPointSize /* = 1.0f */)
{
	glPointSize(InPointSize);
}

void FRHI::EnableDepthTesting()
{
	glEnable(GL_DEPTH_TEST);
}

void FRHI::DisableDepthTesting()
{
	glDisable(GL_DEPTH_TEST);
}

void FRHI::EnableDepthBufferWriting()
{
	glDepthMask(GL_TRUE);
}

void FRHI::DisableDepthBufferWriting()
{
	glDepthMask(GL_FALSE);
}

void FRHI::ClearColorBuffer(const FColor& InColor /* = FColor::Black */)
{
	glClearColor(InColor.R, InColor.G, InColor.B, InColor.A);
	glClear(GL_COLOR_BUFFER_BIT);
}

void FRHI::ClearDepthBuffer(float InDepth /* = 1.0f */)
{
	glClearDepth(InDepth);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void FRHI::Draw(int32 InNumVertices)
{
	glDrawArrays(GL_TRIANGLES, 0, InNumVertices);
}

void FRHI::DrawIndexed(int32 InNumIndices)
{
	glDrawElements(GL_TRIANGLES, InNumIndices, GL_UNSIGNED_INT, 0);
}

