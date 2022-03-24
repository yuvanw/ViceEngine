cmake_minimum_required(VERSION 3.16)

# We're using the INTERFACE option here, because this is a header-only library.
# INTERFACE libraries don't compile their sources or produce a library on disk,
# but they can be used as an argument to target_link_libraries() like any other target.
add_library(json INTERFACE)

target_sources(json
	INTERFACE json/json.hpp
)

target_include_directories(json
	INTERFACE ${CMAKE_CURRENT_LIST_DIR}
)
