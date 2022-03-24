#pragma once

#include "CoreMinimal.h"
#include "Renderer.h"

enum class ERenderingPath : uint8
{
	Forward,
	Deferred
};

/**
 * Singleton class that maintains and provides access to the engine renderer.
 */
class FRenderManager
{
public:
	FRenderManager() = delete;

	static void Init(void* InNativeWindowHandle, const FViewport& InViewport, ERenderingPath InRenderingPath = ERenderingPath::Forward);
	static void Shutdown();
	static void Update(double InDeltaTimeMilliseconds);
	static TSharedPtr<FRenderer> GetRenderer();

private:
	static TSharedPtr<FRenderer> Renderer;
};
