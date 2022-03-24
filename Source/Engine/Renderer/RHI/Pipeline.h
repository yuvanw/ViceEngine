#pragma once

#ifdef GRAPHICS_API_OPENGL
	#include "OpenGL/Pipeline.h"
#else
	#error Unknown graphics API.
#endif
