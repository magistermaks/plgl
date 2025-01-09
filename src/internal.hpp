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

		inline float normalize(float value) {
			return value / 255.0f;
		}

		inline int denormalize(float value) {
			return (int) std::min(std::floor(value * 1.003921568627451f), 255.0f);
		}

		inline void trigger(Event event) {
			user_event_handlers[(int) event]();
		}

		/**
		 * Starts PLGL, called each time open() is invoked,
		 * this functions internally checks if PLGL already was initialized
		 */
		void init();

		/**
		 * Functions used to translate backend
		 * events into PLGL events
		 */
		namespace event {

			void window_close_handle();
			void window_resize_handle(int w, int h);
			void window_focus_handle(bool focused);
			void mouse_move_handle(int x, int y);
			void mouse_scroll_handle(int scroll);
			void mouse_click_handle(int state, int button);
			void keybord_click_handle(int state, int keycode);

		}

	}

}
