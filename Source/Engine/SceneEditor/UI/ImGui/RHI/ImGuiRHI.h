#pragma once

#ifdef GRAPHICS_API_OPENGL
	#include "OpenGL/ImGuiRHI.h"
#else
	#error Unknown graphics API.
#endif
