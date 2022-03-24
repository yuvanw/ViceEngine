#pragma once

#include "CoreMinimal.h"
#include "Viewport.h"

enum class EDrawingMode : uint16;

/**
 * The RHI (Render Hardware Interface) is an abstraction layer to allow for graphics API-independent rendering code.
 */
class FRHI
{
public:
	static void Init(void* InNativeWindowHandle);
	static void Shutdown();
	static void SwapBuffers();
	static void SetViewport(const FViewport& InViewport);
	static void SetDrawingMode(EDrawingMode InDrawingMode);
	static void SetPointSize(float InPointSize = 1.0f);
	static void EnableDepthTesting();
	static void DisableDepthTesting();
	static void EnableDepthBufferWriting();
	static void DisableDepthBufferWriting();
	static void ClearColorBuffer(const FColor& InColor = FColor::Black);
	// Default depth buffer clear value is 1.0f, which is the depth of the far clipping plane.
	static void ClearDepthBuffer(float InDepth = 1.0f);
	static void Draw(int32 InNumVertices);
	static void DrawIndexed(int32 InNumIndices);
};
