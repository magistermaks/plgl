#pragma once

#include "event.hpp"

namespace plgl {

	class Renderer;

	bool opened = false;
	bool focused = false;
	bool should_close = false;
	long width;
	long height;
	long frame_count = 0;
	long frame_rate = 0;

	long mouse_scroll;
	long mouse_x;
	long mouse_y;
	long prev_mouse_x;
	long prev_mouse_y;
	bool mouse_pressed = false;
	long mouse_button;

	long key;
	long key_code;
	bool key_pressed = false;

	std::string last_error = "";
	Renderer* renderer = nullptr;

	namespace impl {
		EventHandler user_event_handlers[impl::EVENT_COUNT] = {};
	}

}
