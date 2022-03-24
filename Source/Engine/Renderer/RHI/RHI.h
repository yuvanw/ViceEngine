#pragma once

#ifdef GRAPHICS_API_OPENGL
	#include "OpenGL/RHI.h"
#else
	#error Unknown graphics API.
#endif
