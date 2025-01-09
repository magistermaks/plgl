
#include "internal.hpp"

namespace plgl::impl {

	bool initialized = false;

	void init() {
		if (!initialized) {
			for (int i = 0; i < impl::EVENT_COUNT; i++) {
				impl::user_event_handlers[i] = UNSET_HANDLER;
			}

			initialized = true;
		}
	}

	namespace event {

		void window_close_handle() {
			plgl::should_close = true;
			trigger(WINDOW_CLOSE);
		}

		void window_resize_handle(int w, int h) {
			glViewport(0, 0, w, h);
			plgl::width = w;
			plgl::height = h;
			trigger(WINDOW_RESIZE);
		}

		void window_focus_handle(bool focused) {
			plgl::focused = focused;
			trigger(WINDOW_FOCUS);
		}

		void mouse_move_handle(int x, int y) {
			prev_mouse_x = mouse_x;
			prev_mouse_y = mouse_y;
			mouse_x = x;
			mouse_y = y;
			trigger(MOUSE_MOVED);
		}

		void mouse_scroll_handle(int scroll) {
			mouse_scroll = scroll;
			trigger(MOUSE_SCROLLED);
		}

		void mouse_click_handle(int state, int button) {
			mouse_button = button;

			if (state == WINX_PRESSED) {
				mouse_pressed = true;
				trigger(MOUSE_PRESSED);
			}

			if (state == WINX_RELEASED) {
				mouse_pressed = false;
				trigger(MOUSE_RELEASED);
			}
		}

		void keybord_click_handle(int state, int keycode) {
			key_code = keycode;

			if (state == WINX_PRESSED) {
				key_pressed = true;
				trigger(KEY_PRESSED);
			}

			if (state == WINX_RELEASED) {
				key_pressed = false;
				trigger(KEY_RELEASED);
			}
		}

	}

}