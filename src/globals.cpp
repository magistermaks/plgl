
#include "render/renderer.hpp"
#include "event.hpp"
#include "globals.hpp"

bool plgl::opened = false;
bool plgl::focused = false;
bool plgl::should_close = false;
long plgl::width;
long plgl::height;
long plgl::frame_count = 0;
long plgl::frame_rate = 0;
long plgl::sound_count = 0;

long plgl::mouse_scroll;
long plgl::mouse_x;
long plgl::mouse_y;
long plgl::prev_mouse_x;
long plgl::prev_mouse_y;
bool plgl::mouse_pressed = false;
long plgl::mouse_button;

long plgl::key;
long plgl::key_code;
bool plgl::key_pressed = false;

std::string plgl::last_error = "";
plgl::Renderer* plgl::renderer = nullptr;
plgl::SoundSystem* plgl::sound_system = nullptr;
plgl::EventHandler plgl::impl::user_event_handlers[impl::EVENT_COUNT] = {};
