
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
