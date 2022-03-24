cmake_minimum_required(VERSION 3.16)

add_executable(Test
	main.cpp	
	ArrayTests.cpp
	ANSIStringTests.cpp
	ArenaAllocatorTests.cpp
	MapTests.cpp
	MathUtilitiesTests.cpp
	SetTests.cpp
	SharedPtrTests.cpp
	StringIdTests.cpp
	Vector2DTests.cpp
	Vector3DTests.cpp
	Vector4DTests.cpp
	Transform4DTests.cpp
	UniquePtrTests.cpp
	WeakPtrTests.cpp
)

target_link_libraries(Test
	Core
	Catch2
)
