#pragma once

#include "RHI/OpenGL/GenericPlatform/GenericPlatformOpenGL.h"

class FWindowsPlatformOpenGL : public FGenericPlatformOpenGL
{
public:
	// Begin FGenericPlatformOpenGL interface.
	static void Init(void* InNativeWindowHandle);
	static void Shutdown();
	static void SwapBuffers();
	// End FGenericPlatformOpenGL interface.
};

using FPlatformOpenGL = FWindowsPlatformOpenGL;
