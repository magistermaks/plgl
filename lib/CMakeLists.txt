
add_library(WINX STATIC
    winx.c
)

target_link_libraries(WINX
	Threads::Threads
	X11::X11 
	X11::Xcursor 
	OpenGL::GLX 
	OpenGL::OpenGL
)

add_library(GLAD STATIC
    glad.c
)

target_link_libraries(GLAD
	Threads::Threads
	OpenGL::GLX 
	OpenGL::OpenGL
)
