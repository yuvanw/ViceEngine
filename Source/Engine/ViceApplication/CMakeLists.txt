cmake_minimum_required(VERSION 3.16)

add_library(ViceApplication)

target_sources(ViceApplication
	PUBLIC ViceApplication.h
	PRIVATE ViceApplication.cpp
)

target_include_directories(ViceApplication
	PUBLIC ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(ViceApplication 
	Core
	ApplicationCore
	InputCore
	GameCore
)
