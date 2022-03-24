#pragma once

#ifdef GRAPHICS_API_OPENGL
	#include "OpenGL/RHIDefinitions.h"
#else
	#error Unknown graphics API.
#endif
