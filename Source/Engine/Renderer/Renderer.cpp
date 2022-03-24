#include "Renderer.h"
#include "Camera/Camera.h"
#include "RHI/RHI.h"

void FRenderer::Init(void* InNativeWindowHandle, const FViewport& InViewport)
{
	WindowHandle = InNativeWindowHandle;
	FRHI::Init(WindowHandle);
	FRHI::SetPointSize(3.0f);

	RenderTarget = FFrameBuffer::Default();
	Camera = MakeShared<FCamera>();

	SetViewport(InViewport);
}

void FRenderer::Shutdown()
{
	FRHI::Shutdown();
}

void FRenderer::Update(double InDeltaTimeMilliseconds)
{
	Render();
}

void FRenderer::SetViewport(const FViewport& InViewport)
{
	Viewport = InViewport;
	FRHI::SetViewport(Viewport);

	RenderTarget->Resize(Viewport.Width, Viewport.Height);
	Camera->SetFrustumWidth(Viewport.Width);
	Camera->SetFrustumHeight(Viewport.Height);
}
