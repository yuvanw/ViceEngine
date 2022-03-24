#pragma once

#include "CoreMinimal.h"
#include "Scene/Scene.h"
#include "Camera/Camera.h"
#include "Viewport.h"
#include "RHI/FrameBuffer.h"

/**
 * Renders the current scene from the viewpoint of the camera.
 * Rendering occurs every Update() call. The render target can
 * be configured in order to render into a frame buffer other 
 * than the default frame buffer (see FFrameBuffer).
 */
class FRenderer
{
public:
	FRenderer() = default;
	virtual ~FRenderer() = default;

	// Non-copyable.
	FRenderer(const FRenderer&) = delete;
	FRenderer& operator=(const FRenderer&) = delete;

	// Non-movable.
	FRenderer(const FRenderer&&) = delete;
	FRenderer& operator=(const FRenderer&&) = delete;

	void Init(void* InNativeWindowHandle, const FViewport& InViewport);
	void Shutdown();
	void Update(double InDeltaTimeMilliseconds);

	// Getters.
	void* GetNativeWindowHandle() 
	{ 
		return WindowHandle; 
	}
	const void* GetNativeWindowHandle() const
	{ 
		return WindowHandle; 
	}
	const FViewport& GetViewport() const 
	{
		return Viewport; 
	}
	TSharedPtr<FScene> GetScene() 
	{ 
		return Scene; 
	};
	TSharedPtr<const FScene> GetScene() const
	{
		return Scene; 
	};
	TSharedPtr<FCamera> GetCamera() 
	{ 
		return Camera;
	};
	TSharedPtr<const FCamera> GetCamera() const 
	{ 
		return Camera;
	};
	TSharedPtr<const FFrameBuffer> GetRenderTarget() const
	{
		return RenderTarget;
	}

	// Setters.
	void SetViewport(const FViewport& InViewport);
	void SetScene(const FStringId& InSceneFile)
	{
		Scene = MakeShared<FScene>(InSceneFile);
	}
	void SetScene(const TSharedPtr<FScene>& InScene)
	{ 
		Scene = InScene;
	}
	void SetCamera(const TSharedPtr<FCamera>& InCamera) 
	{ 
		Camera = InCamera; 
	}
	void SetRenderTarget(const TSharedPtr<FFrameBuffer>& InRenderTarget) 
	{ 
		RenderTarget = InRenderTarget; 
	}

protected:
	void* WindowHandle;
	FViewport Viewport;
	TSharedPtr<FScene> Scene;
	TSharedPtr<FCamera> Camera;
	// Frame buffer rendered to in the rendering loop.
	TSharedPtr<FFrameBuffer> RenderTarget;

private:
	/**
	 * Template method called within FRenderer::Update() to allow for derived class specialization 
	 * of rendering behavior (e.g. FForwardRenderer::Render() vs. FDeferredRenderer::Render()).
	 */
	virtual void Render() = 0;
};
