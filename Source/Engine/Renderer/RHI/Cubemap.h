#pragma once

#ifdef GRAPHICS_API_OPENGL
	#include "OpenGL/Cubemap.h"
#else
	#error Unknown graphics API.
#endif
