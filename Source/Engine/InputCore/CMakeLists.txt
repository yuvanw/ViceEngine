cmake_minimum_required(VERSION 3.16)

add_library(InputCore)

target_sources(InputCore
	PUBLIC Keys.h
	PRIVATE Keys.cpp
	PUBLIC Events.h
	PUBLIC GenericPlatform/GenericPlatformInput.h
	PUBLIC HAL/PlatformInput.h
)

# Add platform specific files 
if(PLATFORM STREQUAL "Windows")
	target_sources(InputCore
		PUBLIC Windows/WindowsPlatformInput.h
		PRIVATE Windows/WindowsPlatformInput.cpp
	)
endif()

target_include_directories(InputCore
	PUBLIC ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(InputCore
	Core
)
