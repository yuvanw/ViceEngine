cmake_minimum_required(VERSION 3.16)

# @TODOGameCore: GameCore is specified as an INTERFACE library 
# to avoid linker errors due to it being header-only right now.
# We should remove the INTERFACE option once .cpp files are added to GameCore.
add_library(GameCore INTERFACE)

target_sources(GameCore
	INTERFACE Game.h
)

target_include_directories(GameCore
	INTERFACE ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(GameCore 
	INTERFACE Core
	INTERFACE InputCore
)
