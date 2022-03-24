#pragma once

#ifdef GRAPHICS_API_OPENGL
	#include "OpenGL/Texture2D.h"
#else
	#error Unknown graphics API.
#endif
