cmake_minimum_required(VERSION 3.16)

if(PLATFORM STREQUAL "Windows")
	add_executable(Engine WIN32 EntryPoint.cpp)
else()
	add_executable(Engine EntryPoint.cpp)
endif()

add_subdirectory(Core)
add_subdirectory(ApplicationCore)
add_subdirectory(EngineCore)
add_subdirectory(GameCore)
add_subdirectory(InputCore)
add_subdirectory(ViceApplication)
add_subdirectory(Renderer)
add_subdirectory(SceneEditor)

target_link_libraries(Engine
	EngineCore
)
