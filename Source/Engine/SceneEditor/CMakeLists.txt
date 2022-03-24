cmake_minimum_required(VERSION 3.16)

add_library(SceneEditor)

target_sources(SceneEditor
	PUBLIC SceneEditor.h
	PRIVATE SceneEditor.cpp

	PUBLIC Camera/CameraController.h
	PRIVATE Camera/CameraController.cpp
	
	PUBLIC UI/InspectorUI.h
	PRIVATE UI/InspectorUI.cpp
	PUBLIC UI/OutlinerUI.h
	PRIVATE UI/OutlinerUI.cpp
	PUBLIC UI/SceneUI.h
	PRIVATE UI/SceneUI.cpp

	PUBLIC UI/ImGui/ImGuiUtilities.h
	PRIVATE UI/ImGui/ImGuiUtilities.cpp
	PUBLIC UI/ImGui/GenericPlatform/GenericPlatformImGui.h
	PUBLIC UI/ImGui/HAL/PlatformImGui.h
	PUBLIC UI/ImGui/RHI/ImGuiRHI.h
)

if(PLATFORM STREQUAL "Windows")
	target_sources(SceneEditor
		PRIVATE UI/ImGui/Windows/WindowsPlatformImGui.h
		PRIVATE UI/ImGui/Windows/WindowsPlatformImGui.cpp
	)
endif()

if(GRAPHICS_API STREQUAL "OpenGL")
	target_sources(SceneEditor
		PUBLIC UI/ImGui/RHI/OpenGL/ImGuiRHI.h
		PUBLIC UI/ImGui/RHI/OpenGL/ImGuiRHI.cpp
	)
endif()

target_include_directories(SceneEditor
	PUBLIC ${CMAKE_CURRENT_LIST_DIR}
	# Private include directory to allow HAL/PlatformImGui.h to include the appropriate platform-specific ImGui header file.
	PRIVATE ${CMAKE_CURRENT_LIST_DIR}/UI/ImGui
)

target_link_libraries(SceneEditor
	Core
	GameCore
	Renderer
	imgui
)
