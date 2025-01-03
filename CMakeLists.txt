cmake_minimum_required(VERSION 3.20)

project(PLGL)
include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(FETCHCONTENT_QUIET OFF)

set(OpenGL_GL_PREFERENCE GLVND)

find_package(Threads REQUIRED)
find_package(OpenGL REQUIRED)
find_package(X11 REQUIRED)

# Try: cmake . -B build && cmake --build build/
if (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
	message(FATAL_ERROR "In source building is deprecated, select another build directory!")
endif()

# Use the mold linker automatically for GCC if available
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	find_program(MOLD_PROGRAM mold)

	if (MOLD_PROGRAM)
		message(STATUS "The MOLD linker was found: ${MOLD_PROGRAM}")
		set(CMAKE_EXE_LINKER_FLAGS "-fuse-ld=mold")
		set(CMAKE_SHARED_LINKER_FLAGS "-fuse-ld=mold")
	else()
		message(STATUS "The MOLD linker was NOT found!")
	endif()
endif()

#FetchContent_Declare(
#		openal
#		GIT_REPOSITORY https://github.com/kcat/openal-soft
#		GIT_TAG 1.23.1
#		GIT_PROGRESS TRUE
#)

FetchContent_Declare(
		stb
		GIT_REPOSITORY https://github.com/nothings/stb
		GIT_TAG 5736b15f7ea0ffb08dd38af21067c314d6a3aae9
		GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(stb)

include_directories(
		.
		lib/
		src/
		${stb_SOURCE_DIR}                        # Headers in ./
)

file(GLOB_RECURSE PLGL_SOURCES
		"src/*.cpp"
)

#add_library(external
#		lib/stb_image.cpp
#		lib/stb_image_write.cpp
#		lib/stb_vorbis.cpp
#)

add_library(PLGL
		${PLGL_SOURCES}
)

set(PLGL_LIBS
		Threads::Threads
#		external
)

target_link_libraries(PLGL PRIVATE ${PLGL_LIBS})
target_compile_definitions(PLGL PRIVATE "SOURCE_ROOT=\"${CMAKE_SOURCE_DIR}\"")

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

	# GCC debugging doodads
	if (CMAKE_BUILD_TYPE STREQUAL "Debug")
		message(STATUS "Compiling with debugging symbols")
		target_compile_options(PLGL PRIVATE -g)
	endif()

	set(PLGL_WARNING_FLAGS
			-Wno-volatile
			-Wall
			-Werror=return-type
			-Werror=return-local-addr
	)

#	target_compile_options(PLGL PRIVATE ${PLGL_WARNING_FLAGS})
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")

#	# we4715 - treat missing return statement as error
#	target_compile_options(main PRIVATE /W4 /we4715)
endif()

add_subdirectory (lib)
add_subdirectory (test)
