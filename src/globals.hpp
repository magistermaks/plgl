#pragma once

#include "event.hpp"

namespace plgl {

	class Renderer;
	class SoundSystem;

	extern bool opened;
	extern bool focused;
	extern bool should_close;
	extern long width;
	extern long height;
	extern long frame_count;
	extern long frame_rate;
	extern long sound_count;

	extern long mouse_scroll;
	extern long mouse_x;
	extern long mouse_y;
	extern long prev_mouse_x;
	extern long prev_mouse_y;
	extern bool mouse_pressed;
	extern long mouse_button;

	extern long key;
	extern long key_code;
	extern bool key_pressed;

	extern std::string last_error;
	extern Renderer* renderer;
	extern SoundSystem* sound_system;

	namespace impl {
		extern EventHandler user_event_handlers[impl::EVENT_COUNT];
	}

}
