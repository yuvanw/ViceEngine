cmake_minimum_required(VERSION 3.16)

add_library(EngineCore)

target_sources(EngineCore
	PUBLIC Engine.h
	PRIVATE Engine.cpp
	PUBLIC GenericPlatform/GenericPlatformEntryPoint.h
	PUBLIC HAL/PlatformEntryPoint.h
)

# Add platform specific files 
if(PLATFORM STREQUAL "Windows")
	target_sources(EngineCore
		PUBLIC Windows/WindowsPlatformEntryPoint.h
	)
endif()

target_include_directories(EngineCore
	PUBLIC ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(EngineCore
	Core
	ApplicationCore
	Renderer
	ViceApplication
	SceneEditor
)
