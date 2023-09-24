#pragma once

#include "globals.hpp"
#include "util.hpp"

namespace plgl {

	namespace impl {

		inline float remapx(float x) {
			return x / plgl::width * 2 - 1;
		}

		inline float remapy(float y) {
			return y / plgl::height * -2 + 1;
		}

		inline float norm(float value) {
			return value / 255.0f;
		}

		inline int renorm(float value) {
			return (int) std::min(std::floor(value * 1.003921568627451f), 255.0f);
		}

		inline void trigger(Event event) {
			user_event_handlers[(int) event]();
		}

		template<class... Args>
		[[noreturn]] inline void fatal(const char* format_str, Args&&... args) {
			last_error = format(format_str, args...);
			trigger(WINDOW_ERROR);
			exit(1);
		}

		void init() {
			static bool initialized = false;

			if (!initialized) {
				for (int i = 0; i < impl::EVENT_COUNT; i ++) {
					impl::user_event_handlers[i] = UNSET_HANDLER;
				}

				impl::user_event_handlers[(int) WINDOW_ERROR] = [] () {
					printf("%s\n", last_error.c_str());
				};

				initialized = true;
			}
		}

		namespace event {

			inline void window_close_handle() {
				plgl::should_close = true;
				trigger(WINDOW_CLOSE);
			}

			inline void window_resize_handle(int w, int h) {
				glViewport(0, 0, w, h);
				plgl::width = w;
				plgl::height = h;
				trigger(WINDOW_RESIZE);
			}

			inline void window_focus_handle(bool focused) {
				plgl::focused = focused;
				trigger(WINDOW_FOCUS);
			}

			inline void mouse_move_handle(int x, int y) {
				prev_mouse_x = mouse_x;
				prev_mouse_y = mouse_y;
				mouse_x = x;
				mouse_y = y;
				trigger(MOUSE_MOVED);
			}

			inline void mouse_scroll_handle(int scroll) {
				mouse_scroll = scroll;
				trigger(MOUSE_SCROLLED); 
			}

			inline void mouse_click_handle(int state, int button) {
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

			inline void keybord_click_handle(int state, int keycode) {
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

}

