cmake_minimum_required(VERSION 3.16)

add_library(stb)

target_sources(stb
	PUBLIC stb/stb_image.h
	PRIVATE stb/stb_image.cpp
)

target_include_directories(stb
	PUBLIC ${CMAKE_CURRENT_LIST_DIR}
)
