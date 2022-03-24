#pragma once

#ifdef GRAPHICS_API_OPENGL
	#include "OpenGL/FrameBuffer.h"
#else
	#error Unknown graphics API.
#endif
