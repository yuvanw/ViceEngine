cmake_minimum_required(VERSION 3.16)

add_library(Renderer)

target_sources(Renderer
	PUBLIC RenderManager.h
	PRIVATE RenderManager.cpp
	PUBLIC Renderer.h
	PRIVATE Renderer.cpp
	PRIVATE ForwardRenderer.h
	PRIVATE ForwardRenderer.cpp
	PRIVATE RendererFileSystem.h
	PRIVATE RendererFileSystem.cpp
	PUBLIC Viewport.h

	PUBLIC Camera/Camera.h
	PRIVATE Camera/Camera.cpp
	PUBLIC Camera/Frustum.h
	PRIVATE Camera/Frustum.cpp

	PUBLIC Geometry/Mesh.h
	PRIVATE Geometry/Mesh.cpp
	PUBLIC Geometry/Model.h
	PRIVATE Geometry/Model.cpp
	PRIVATE Geometry/ModelFile.h
	PRIVATE Geometry/ModelFile.cpp
	PRIVATE Geometry/WavefrontObj.h
	PRIVATE Geometry/WavefrontObj.cpp

	PUBLIC Lights/DirectionalLight.h
	PRIVATE Lights/DirectionalLight.cpp
	PUBLIC Lights/PointLight.h
	PRIVATE Lights/PointLight.cpp

	PUBLIC Materials/Material.h
	PRIVATE Materials/Material.cpp
	PRIVATE Materials/MaterialFile.h
	PRIVATE Materials/MaterialFile.cpp
	PRIVATE Materials/MaterialRegistry.h

	PUBLIC Scene/Scene.h
	PRIVATE Scene/Scene.cpp
	PRIVATE Scene/SceneFile.h
	PRIVATE Scene/SceneFile.cpp
	PUBLIC Scene/Skybox.h
	PUBLIC Scene/Skybox.cpp
	
	PRIVATE RHI/RHI.h
	PRIVATE	RHI/RHIDefinitions.h
	PRIVATE RHI/Shader.h
	PRIVATE RHI/Pipeline.h
	PRIVATE RHI/FrameBuffer.h
	PRIVATE RHI/VertexArray.h
	PRIVATE RHI/Texture2D.h
	PRIVATE RHI/TextureRegistry.h
	PRIVATE RHI/Cubemap.h
	PRIVATE RHI/VertexFormats.h
)

target_include_directories(Renderer
	PUBLIC ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(Renderer
	Core
	OpenGL
	stb
	json
)

if(GRAPHICS_API STREQUAL "OpenGL")
	find_package(OpenGL REQUIRED)

	if (OPENGL_FOUND)
		target_link_libraries(Renderer OpenGL::GL)
	else()
		message(FATAL_ERROR "Error: OpenGL library not found.")
	endif ()

	target_compile_definitions(Renderer PUBLIC GRAPHICS_API_OPENGL)

	target_sources(Renderer
		PRIVATE RHI/OpenGL/RHI.h
		PRIVATE RHI/OpenGL/RHI.cpp
		PRIVATE	RHI/OpenGL/RHIDefinitions.h
		PRIVATE RHI/OpenGL/Shader.h
		PRIVATE RHI/OpenGL/Shader.cpp
		PRIVATE RHI/OpenGL/Pipeline.h
		PRIVATE RHI/OpenGL/Pipeline.cpp
		PRIVATE RHI/OpenGL/FrameBuffer.h
		PRIVATE RHI/OpenGL/FrameBuffer.cpp
		PRIVATE RHI/OpenGL/VertexArray.h
		PRIVATE RHI/OpenGL/Texture2D.h
		PRIVATE RHI/OpenGL/Texture2D.cpp
		PRIVATE RHI/OpenGL/Cubemap.h
		PRIVATE RHI/OpenGL/Cubemap.cpp
		PRIVATE RHI/OpenGL/OpenGLApi.h
		PRIVATE RHI/OpenGL/GenericPlatform/GenericPlatformOpenGL.h
		PRIVATE RHI/OpenGL/HAL/PlatformOpenGL.h
	)

	if(PLATFORM STREQUAL "Windows")
		target_sources(Renderer
			PRIVATE RHI/OpenGL/Windows/WindowsPlatformOpenGL.h
			PRIVATE RHI/OpenGL/Windows/WindowsPlatformOpenGL.cpp
		)
	endif()
endif()

target_compile_definitions(Renderer PUBLIC ROOT_FOLDER=${PROJECT_SOURCE_DIR}/..)
