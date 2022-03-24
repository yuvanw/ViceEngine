cmake_minimum_required(VERSION 3.16)

add_library(imgui)

target_sources(imgui
	PUBLIC imgui/imconfig.h
	PUBLIC imgui/imgui.h
	PRIVATE imgui/imgui.cpp
	PRIVATE imgui/imgui_demo.cpp
	PRIVATE imgui/imgui_draw.cpp
	PUBLIC imgui/imgui_internal.h
	PRIVATE imgui/imgui_tables.cpp
	PRIVATE imgui/imgui_widgets.cpp
	PUBLIC imgui/imstb_rectpack.h
	PUBLIC imgui/imstb_textedit.h
	PUBLIC imgui/imstb_truetype.h
	PUBLIC imgui/backends/imgui_impl_win32.h
	PRIVATE imgui/backends/imgui_impl_win32.cpp
	PUBLIC imgui/backends/imgui_impl_opengl3.h
	PRIVATE imgui/backends/imgui_impl_opengl3.cpp
	PUBLIC imgui/backends/imgui_impl_opengl3_loader.h
)

target_include_directories(imgui
	PUBLIC ${CMAKE_CURRENT_LIST_DIR}
)
