cmake_minimum_required(VERSION 3.16)

add_library(ApplicationCore)

target_sources(ApplicationCore
	PUBLIC Messages.h
	PUBLIC GenericPlatform/GenericApplication.h
	PRIVATE GenericPlatform/GenericApplication.cpp
	PUBLIC GenericPlatform/GenericApplicationMessageHandler.h
	PUBLIC GenericPlatform/GenericPlatformApplication.h
	PUBLIC GenericPlatform/GenericWindow.h
	PUBLIC HAL/PlatformApplication.h
)

# Add platform specific files 
if(PLATFORM STREQUAL "Windows")
	target_sources(ApplicationCore
		PUBLIC Windows/WindowsApplication.h
		PUBLIC Windows/WindowsPlatformApplication.h
		PUBLIC Windows/WindowsWindow.h
		PRIVATE Windows/WindowsApplication.cpp
		PRIVATE Windows/WindowsPlatformApplication.cpp
		PRIVATE Windows/WindowsWindow.cpp
	)
endif()

target_include_directories(ApplicationCore
	PUBLIC ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(ApplicationCore 
	Core
)
