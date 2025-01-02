
project(PLGL)

cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE, Debug)

set(OpenGL_GL_PREFERENCE GLVND)

find_package(Threads REQUIRED)
find_package(OpenGL REQUIRED)
find_package(X11 REQUIRED)

if (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(FATAL_ERROR "Select another Build Directory!")
endif()

if (CMAKE_SOURCE_DIR MATCHES " ")
	message(WARNING "Source Directory path contains spaces!")
endif()

if (CMAKE_BINARY_DIR MATCHES " ")
	message(WARNING "Build Directory path contains spaces!")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

add_library(PLGL
	src/external.cpp
	src/globals.cpp
	src/random.cpp
	src/window.cpp
	src/math.cpp
)

add_subdirectory (lib)

include_directories(
	src/
	.
)

add_subdirectory (test)
