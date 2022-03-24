#pragma once

#ifdef GRAPHICS_API_OPENGL
	#include "OpenGL/Shader.h"
#else
	#error Unknown graphics API.
#endif
