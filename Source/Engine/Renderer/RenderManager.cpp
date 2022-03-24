#include "RenderManager.h"
#include "ForwardRenderer.h"

/*static*/ TSharedPtr<FRenderer> FRenderManager::Renderer = nullptr;

/*static*/ void FRenderManager::Init(void* InNativeWindowHandle, const FViewport& InViewport, ERenderingPath InRenderingPath /* = ERenderingPath::Forward */)
{
	if (Renderer)
	{
		return;
	}

	if (InRenderingPath == ERenderingPath::Forward)
	{
		Renderer = TSharedPtr<FForwardRenderer>(new FForwardRenderer);
	}
	else if (InRenderingPath == ERenderingPath::Deferred)
	{
		// Deferred rendering not yet supported.
		ensure(false);
	}
	else
	{
		// Invalid rendering path.
		ensure(false);
	}

	Renderer->Init(InNativeWindowHandle, InViewport);
}

/*static*/ void FRenderManager::Shutdown()
{
	GetRenderer()->Shutdown();
}

/*static*/ void FRenderManager::Update(double InDeltaTimeMilliseconds)
{
	GetRenderer()->Update(InDeltaTimeMilliseconds);
}

/*static*/ TSharedPtr<FRenderer> FRenderManager::GetRenderer()
{
	if (!Renderer)
	{
		// This assert is triggered if Init() hasn't been called yet.
		ensure(false);
	}

	return Renderer;
}
