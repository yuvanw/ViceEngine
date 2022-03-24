cmake_minimum_required(VERSION 3.16)
project(ViceEngine LANGUAGES C CXX)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
	set(PLATFORM "Windows")
	# Specify the platform name through a preprocessor definition so that the
	# correct platform-specific header files are included during preprocessing.
	# (See: PLATFORM_HEADER macro in PreprocessorHelpers.h for details.)
	add_compile_definitions(PLATFORM_NAME=Windows)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
	set(PLATFORM "Mac")
	add_compile_definitions(PLATFORM_NAME=Mac)
	# We need to enable Objective-C for Apple systems.
	# This also allows files with a .cpp extension to have Objective-C code.
	set(CMAKE_CXX_FLAGS "-x objective-c++")
endif()

# Use C++14
set(CMAKE_CXX_STANDARD 14)

#we have executables, libraries and libs in the same directory to make loading DLLs easier
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )

# we remove the prepended lib to static/shared libraries to make finding them during runtime easier 
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_PREFIX "")

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")
set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED ON)
#we default all libraries to be built as shared to support a module system 
# set(BUILD_SHARED_LIBS ON)

# Temporary workaround for Windows until we create export macros
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
# Set default Visual Studio project to Engine.
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Engine)

include(CTest)
enable_testing()

add_subdirectory(Engine)
add_subdirectory(Test)
add_subdirectory(ThirdParty)
