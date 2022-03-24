#pragma once

#ifdef GRAPHICS_API_OPENGL
	#include "OpenGL/VertexArray.h"
#else
	#error Unknown graphics API.
#endif
