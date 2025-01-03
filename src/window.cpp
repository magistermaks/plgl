
#include "window.hpp"

#define UNTITLED_DEFAULT "Untitled"
static WinxCursor* null_cursor = nullptr;

void plgl::open(const std::string& title, int width, int height) {
	impl::init();

	if (opened) {
		impl::fatal("There can only be one window!");
	}

	stbi_flip_vertically_on_write(true);
	stbi_set_flip_vertically_on_load(true);

	winxHint(WINX_HINT_VSYNC, WINX_VSYNC_ENABLED);
	winxHint(WINX_HINT_MULTISAMPLES, 4);

	if (!winxOpen(width, height, title.c_str())) {
		impl::fatal("Failed to open window, WINX error: %s!", winxGetError());
	}

	// use GLAD to load OpenGL functions
	gladLoadGL();

	// load deafult values into opengl
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glEnable(GL_SCISSOR_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	plgl::opened = true;
	plgl::should_close = false;
	plgl::width = width;
	plgl::height = height;
	plgl::renderer = new Renderer();
	plgl::focused = winxGetFocus();

	// setup WINX event handlers
	winxSetCloseEventHandle(impl::event::window_close_handle);
	winxSetResizeEventHandle(impl::event::window_resize_handle);
	winxSetCursorEventHandle(impl::event::mouse_move_handle);
	winxSetButtonEventHandle(impl::event::mouse_click_handle);
	winxSetKeyboardEventHandle(impl::event::keybord_click_handle);
	winxSetScrollEventHandle(impl::event::mouse_scroll_handle);
	winxSetFocusEventHandle(impl::event::window_focus_handle);

	// create WINX empty cursor icon
	null_cursor = winxCreateNullCursorIcon();
}

void plgl::open(int width, int height) {
	open(UNTITLED_DEFAULT, width, height);
}

void plgl::open(const std::string& title) {
	open(title, 200, 200);
}

void plgl::open() {
	open(UNTITLED_DEFAULT, 200, 200);
}

void plgl::listen(Event event, EventHandler callback) {
	impl::user_event_handlers[(int) event] = callback;
}

void plgl::close() {
	winxClose();
	plgl::opened = false;
	plgl::should_close = false;

	delete plgl::renderer;
	plgl::renderer = nullptr;
}

void plgl::title(const std::string& title) {
	winxSetTitle(title.c_str());
}

void plgl::background(float r, float g, float b) {
	glClearColor(impl::norm(r), impl::norm(g), impl::norm(b), 1.0);
}

void plgl::background(const impl::Color& color) {
	RGBA rgba = color.as_rgba();
	glClearColor(impl::norm(rgba.red()), impl::norm(rgba.green()), impl::norm(rgba.blue()), 1.0);
}

void plgl::swap() {
	impl::trigger(WINDOW_DRAW);
	renderer->flush();
	winxSwapBuffers();
	winxPollEvents();
	glClear(GL_COLOR_BUFFER_BIT);
	frame_count ++;
}

void plgl::cursor_capture(bool capture) {
	winxSetCursorCapture(capture);
}

void plgl::cursor_hide(bool hidden) {
	if (hidden) {
		winxSetCursorIcon(null_cursor);
	} else {
		winxSetCursorIcon(WINX_ICON_DEFAULT);
	}
}