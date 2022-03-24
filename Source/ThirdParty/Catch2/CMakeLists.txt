cmake_minimum_required(VERSION 3.16)

# We're using the INTERFACE option here, because this is a header-only library.
# INTERFACE libraries don't compile their sources or produce a library on disk,
# but they can be used as an argument to target_link_libraries() like any other target.
add_library(Catch2 INTERFACE)

target_sources(Catch2
	INTERFACE catch/catch.hpp
)

target_include_directories(Catch2
	INTERFACE ${CMAKE_CURRENT_LIST_DIR}
)
