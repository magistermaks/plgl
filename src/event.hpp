#pragma once

#include "external.hpp"

#define UNSET_HANDLER ([](){})

namespace plgl {

	enum Event {
		KEY_PRESSED    = 0,
		KEY_RELEASED   = 1,
		KEY_TYPED      = 2,
		MOUSE_CLICKED  = 3,
		MOUSE_MOVED    = 4,
		MOUSE_PRESSED  = 5,
		MOUSE_RELEASED = 6,
		MOUSE_SCROLLED = 7,
		WINDOW_RESIZE  = 8,
		WINDOW_CLOSE   = 9,
		WINDOW_DRAW    = 10,
		WINDOW_FOCUS   = 11
	};

	using EventHandler = std::function<void()>;

	namespace impl {
		constexpr int EVENT_COUNT = 12;
	}

}
