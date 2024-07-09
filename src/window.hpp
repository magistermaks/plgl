#pragma once

#include "event.hpp"
#include "internal.hpp"
#include "color.hpp"
#include "render/renderer.hpp"

namespace plgl {

	namespace impl {
		extern WinxCursor* null_cursor;
	}

	inline void open(const char* title, int width, int height) {
		impl::init();

		if (opened) {
			impl::fatal("There can only be one window!");
		}

		stbi_flip_vertically_on_write(true);
		stbi_set_flip_vertically_on_load(true);

		winxHint(WINX_HINT_VSYNC, WINX_VSYNC_ENABLED);
		winxHint(WINX_HINT_MULTISAMPLES, 4);

		if (!winxOpen(width, height, title)) {
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
		winxSetKeybordEventHandle(impl::event::keybord_click_handle);
		winxSetScrollEventHandle(impl::event::mouse_scroll_handle);
		winxSetFocusEventHandle(impl::event::window_focus_handle);

		// create WINX empty cursor icon
		impl::null_cursor = winxCreateNullCursorIcon();
	}

	inline void listen(Event event, EventHandler callback) {
		impl::user_event_handlers[(int) event] = callback;
	}

	inline void close() {
		winxClose();
		plgl::opened = false;
		plgl::should_close = false;

		delete plgl::renderer;
		plgl::renderer = nullptr;
	}

	inline void title(const char* title) {
		winxSetTitle(title);
	}

	inline void background(float r, float g, float b) {
		glClearColor(impl::norm(r), impl::norm(g), impl::norm(b), 1.0);
	}

	inline void background(const impl::Color& color) {
		RGBA rgba = color.as_rgba();
		glClearColor(impl::norm(rgba.red()), impl::norm(rgba.green()), impl::norm(rgba.blue()), 1.0);
	}

	inline void swap() {
		impl::trigger(WINDOW_DRAW);
		renderer->flush();
		winxSwapBuffers();
		winxPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);
		frame_count ++;
	}

	inline void cursor_capture(bool capture) {
		winxSetCursorCapture(capture);
	}

	inline void cursor_hide(bool hidden) {
		if (hidden) {
			winxSetCursorIcon(impl::null_cursor);
		} else {
			winxSetCursorIcon(WINX_ICON_DEFAULT);
		}
	}

}
